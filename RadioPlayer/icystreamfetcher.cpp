#include "icystreamfetcher.h"
#include <QRegularExpression>
#include <QDebug>

IcyStreamFetcher::IcyStreamFetcher(QObject *parent)
    : QObject(parent),
    currentUrl(QUrl()),
    currentStreamTitle("")
{
    manager = new QNetworkAccessManager(this);
}

void IcyStreamFetcher::fetch(const QUrl &url)
{
    if (url == currentUrl)
        return;

    currentUrl = url;

    if (currentReply) {
        currentReply->disconnect();
        currentReply->abort();
        currentReply->deleteLater();
        currentReply = nullptr;
    }

    QNetworkRequest request(url);
    request.setRawHeader("Icy-MetaData", "1");

    currentReply = manager->get(request);

    connect(currentReply, &QNetworkReply::readyRead, this, [=]() {
        QByteArray data = currentReply->readAll();

        if (data.contains("StreamTitle='")) {
            QString title = parseStreamTitle(data);
            if (!title.isEmpty() && title != currentStreamTitle) {
                currentStreamTitle = title;
                emit streamTitleChanged(currentStreamTitle);
            }
        }
    });

    connect(currentReply, &QNetworkReply::errorOccurred, this, [=](QNetworkReply::NetworkError code) {
        QString errorMsg = QString("Network error: %1").arg(currentReply->errorString());
        qWarning() << errorMsg;
        emit streamError(errorMsg);
    });

    connect(currentReply, &QNetworkReply::finished, this, [=]() {
        currentReply->deleteLater();
        currentReply = nullptr;
    });
}

QString IcyStreamFetcher::getStreamTitle() const
{
    return currentStreamTitle;
}

QString IcyStreamFetcher::parseStreamTitle(const QByteArray &metaData)
{
    QRegularExpression regex("StreamTitle='(.*?)';");
    QRegularExpressionMatch match = regex.match(metaData);

    if (match.hasMatch()) {
        return match.captured(1);
    }

    return "";
}
