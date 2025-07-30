#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "icystreamfetcher.h"

#include <QMediaMetaData>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , movie(new QMovie(this))
    , retryTimer(new QTimer(this))
    , player(new QMediaPlayer(this))
    , audioOutput(new QAudioOutput(this))
    , icyFetcher(new IcyStreamFetcher(this))
    , opacityEffect(new QGraphicsOpacityEffect(this))
    , fadeAnim(new QPropertyAnimation(opacityEffect, "opacity", this))
{
    ui->setupUi(this);

    trackListModel = new QStringListModel(this);
    ui->listViewHistory->setModel(trackListModel);

    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(0.5);
    ui->sliderVolume->setValue(50);

    connect(ui->sliderVolume, &QSlider::valueChanged, this, &MainWindow::on_sliderVolume_valueChanged);
    connect(player, &QMediaPlayer::metaDataChanged, this, &MainWindow::onMetaDataChanged);
    connect(icyFetcher, &IcyStreamFetcher::streamError, this, [=](const QString &error) {
        ui->labelNowPlaying->setStyleSheet("color: red; font-weight: bold;");
        ui->labelNowPlaying->setText("⚠️ Hata: " + error);
    });


    retryTimer->setInterval(5000);
    retryTimer->setSingleShot(false);
    connect(retryTimer, &QTimer::timeout, this, [=]() {
        if (!lastTriedUrl.isEmpty()) {
            qDebug() << "[Retry] Tekrar deneniyor:" << lastTriedUrl;
            player->setSource(lastTriedUrl);
            player->play();
        }
    });

    ui->labelGif->setMovie(movie);
    ui->labelGif->setScaledContents(true);
    ui->labelGif->setFixedSize(300, 200);

    ui->labelGif->setGraphicsEffect(opacityEffect);
    opacityEffect->setOpacity(1.0);

    fadeAnim->setDuration(500);
    fadeAnim->setStartValue(0.0);
    fadeAnim->setEndValue(1.0);

    movie->setFileName(":/gifs/saddog.gif");
    movie->start();

    connect(player, &QMediaPlayer::mediaStatusChanged, this, [=](QMediaPlayer::MediaStatus status) {
        QString gifPath;

        if (status == QMediaPlayer::BufferedMedia || status == QMediaPlayer::LoadedMedia) {
            ui->labelDurum->setText("Çalıyor...");
            gifPath = ":/gifs/djcats.gif";
            retryTimer->stop();
        } else if (status == QMediaPlayer::NoMedia || status == QMediaPlayer::EndOfMedia) {
            ui->labelDurum->setText("Bekleniyor");
            gifPath = ":/gifs/saddog.gif";
            retryTimer->stop();
        } else if (status == QMediaPlayer::InvalidMedia) {
            ui->labelDurum->setText("Bağlantı Hatası - Yeniden deneniyor...");
            gifPath = ":/gifs/saddog.gif";
            retryTimer->start();
        } else {
            return;
        }

        fadeAnim->stop();
        opacityEffect->setOpacity(0.0);
        movie->stop();
        movie->setFileName(gifPath);
        movie->start();
        fadeAnim->start();
    });

    connect(icyFetcher, &IcyStreamFetcher::streamTitleChanged, this, [=](const QString &title) {
        ui->labelNowPlaying->setText("🎵 " + title);
        onMetaDataChanged();
    });
    connect(icyFetcher, &IcyStreamFetcher::streamError, this, [=](const QString &error) {
        ui->labelNowPlaying->setStyleSheet("color: red; font-weight: bold;");
        ui->labelNowPlaying->setText("⚠️ Hata: " + error);
    });


    QTimer *icyUpdateTimer = new QTimer(this);
    icyUpdateTimer->setInterval(10000);
    connect(icyUpdateTimer, &QTimer::timeout, this, [=]() {
        if (!lastTriedUrl.isEmpty()) {
            icyFetcher->fetch(lastTriedUrl);
        }
    });
    icyUpdateTimer->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::playRadio(const QUrl &url)
{
    retryTimer->stop();
    lastTriedUrl = url;
    player->setSource(url);
    player->play();
    icyFetcher->fetch(url);
}

void MainWindow::on_sliderVolume_valueChanged(int value)
{
    audioOutput->setVolume(value / 100.0);
}

void MainWindow::on_btnYeniRap_clicked()
{
    playRadio(QUrl("https://28513.live.streamtheworld.com/SC018_SO1AAC.aac"));
}

void MainWindow::on_btnVirgin_clicked()
{
    playRadio(QUrl("https://28933.live.streamtheworld.com/VIRGIN_RADIO128AAC.aac"));
}

void MainWindow::on_btnJoyTurk_clicked()
{
    playRadio(QUrl("https://28513.live.streamtheworld.com/JOY_TURK128AAC.aac"));
}

void MainWindow::on_btnPlayManual_clicked()
{
    QString url = ui->lineEditUrl->text();
    playRadio(QUrl(url));
}

void MainWindow::on_btnStop_clicked()
{
    player->stop();
    ui->labelDurum->setText("Durdu");

    fadeAnim->stop();
    opacityEffect->setOpacity(0.0);
    movie->stop();
    movie->setFileName(":/gifs/saddog.gif");
    movie->start();
    fadeAnim->start();
}

void MainWindow::onMetaDataChanged()
{
    QString currentTitle = icyFetcher->getStreamTitle();
    if (currentTitle.isEmpty()) {
        currentTitle = player->metaData().stringValue(QMediaMetaData::Title);
        QString artist = player->metaData().stringValue(QMediaMetaData::ContributingArtist);
        if (!artist.isEmpty())
            currentTitle = artist + " - " + currentTitle;
    }

    if (!currentTitle.isEmpty() && (previousTracks.isEmpty() || previousTracks.first() != currentTitle)) {
        previousTracks.prepend(currentTitle);
        while (previousTracks.size() > 20)
            previousTracks.removeLast();

        trackListModel->setStringList(previousTracks);
    }


    QString streamTitle = icyFetcher->getStreamTitle();
    if (!streamTitle.isEmpty()) {
        ui->labelNowPlaying->setStyleSheet("");
        ui->labelNowPlaying->setText("🎵 " + streamTitle);
    } else {
        QString title = player->metaData().stringValue(QMediaMetaData::Title);
        QString artist = player->metaData().stringValue(QMediaMetaData::ContributingArtist);

        if (!title.isEmpty() && !artist.isEmpty()) {
            ui->labelNowPlaying->setText("🎵 " + artist + " - " + title);
        } else if (!title.isEmpty()) {
            ui->labelNowPlaying->setText("🎵 " + title);
        } else if (!artist.isEmpty()) {
            ui->labelNowPlaying->setText("🎵 " + artist);
        } else {
            ui->labelNowPlaying->setText("🎶 Şarkı bilgisi alınamadı.");
        }
    }
}
