#ifndef ICYSTREAMFETCHER_H
#define ICYSTREAMFETCHER_H

#include <QObject>
#include <QUrl>
#include <QString>
#include <QByteArray>
#include <QNetworkReply>
#include <QNetworkAccessManager>

class IcyStreamFetcher : public QObject
{
    Q_OBJECT

public:
    explicit IcyStreamFetcher(QObject *parent = nullptr);

    void fetch(const QUrl &url);
    QString getStreamTitle() const;

signals:
    void streamTitleChanged(const QString &title);
    void streamError(const QString &error);

private:
    QString parseStreamTitle(const QByteArray &metaData);

    QUrl currentUrl;
    QString currentStreamTitle;
    QNetworkReply *currentReply = nullptr;
    QNetworkAccessManager *manager = nullptr;
};

#endif // ICYSTREAMFETCHER_H

