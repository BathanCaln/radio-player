#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QMovie>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QStringList>
#include "icystreamfetcher.h"
#include <QTimer>
#include <QStringList>
#include <QStringListModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnYeniRap_clicked();
    void on_btnVirgin_clicked();
    void on_btnJoyTurk_clicked();
    void on_btnPlayManual_clicked();
    void on_sliderVolume_valueChanged(int value);
    void on_btnStop_clicked();


private:
    Ui::MainWindow *ui;
    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    QMovie *movie;
    IcyStreamFetcher *icyFetcher;

    QGraphicsOpacityEffect *opacityEffect;
    QPropertyAnimation *fadeAnim;

    QStringList historyList;

    QTimer *retryTimer;
    QUrl lastTriedUrl;

    QStringList previousTracks;
    QStringListModel *trackListModel;

    void playRadio(const QUrl &url);
    void onMetaDataChanged();
    void fetchStreamTitle(const QString &url);
};
#endif // MAINWINDOW_H
