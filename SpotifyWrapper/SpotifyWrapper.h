#ifndef SPOTIFYWRAPPER_H
#define SPOTIFYWRAPPER_H

#include <QNetworkReply>
#include <QOAuth2AuthorizationCodeFlow>
#include <QObject>
#include <QUrl>
enum ReqType { GET, POST, PUT};
class SpotifyWrapper : public QObject
{
  Q_OBJECT
  public:
  explicit SpotifyWrapper(QObject *parent = nullptr);
      ~SpotifyWrapper();
  public:
  typedef void (SpotifyWrapper::*responsePtr)(QJsonDocument);

  void init();
  QNetworkReply* get (QUrl url);
  QNetworkReply* get (QNetworkRequest url);
  QNetworkReply* post(QUrl url);
  QNetworkReply* post(QNetworkRequest url);
  QNetworkReply* put (QUrl url);
  QNetworkReply* put (QNetworkRequest url);
  private:
  bool loadInfo();
  void setInfo();
  void procceedAuth();
  void refreshToken();
  void saveInfo();

  private:
  QOAuth2AuthorizationCodeFlow wrapper;
    QString clientID,
        clientSecret,
        clientRefreshToken,
        clientToken;
  signals:
    void responseText(QString);
};

#endif // SPOTIFYWRAPPER_H
