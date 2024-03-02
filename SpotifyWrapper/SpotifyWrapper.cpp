#include "SpotifyWrapper.h"

#include <QSettings>
#include <QtNetworkAuth>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDesktopServices>

SpotifyWrapper::SpotifyWrapper(QObject *parent)
    : QObject{parent}
{
  init();
}

SpotifyWrapper::~SpotifyWrapper()
{
  saveInfo();
}

void SpotifyWrapper::init()
{
  loadInfo();
  setInfo();
}

QNetworkReply* SpotifyWrapper::get(QUrl url)
{
  if(QTime().msecsSinceStartOfDay()>tokenExpireTime)refreshToken();
  return wrapper.get(url);
}


QNetworkReply* SpotifyWrapper::post(QUrl url)
{
  if(QTime().msecsSinceStartOfDay()>tokenExpireTime)refreshToken();
  return wrapper.post(url);
}


QNetworkReply *SpotifyWrapper::put(QUrl url)
{
  if(QTime().msecsSinceStartOfDay()>tokenExpireTime)refreshToken();
  return wrapper.put(url);
}

void SpotifyWrapper::forceUpdateToken()
{
  refreshToken();
}

bool SpotifyWrapper::loadInfo()
{
  QSettings info(QCoreApplication::applicationDirPath()+"\\clientInfo.ini",QSettings::Format::IniFormat);
  info.beginGroup("ClientInfo");
  clientID=info.value("clientID").toString();
  clientSecret=info.value("clientSecret").toString();
  clientRefreshToken=info.value("clientRefreshToken").toString();
  info.endGroup();


  return !clientID.isEmpty() &&
         !clientSecret.isEmpty() &&
         !clientRefreshToken.isEmpty();
}

void SpotifyWrapper::setInfo()
{
  //создаем oauth запрощик для работы с спотычем
  if(clientID.isEmpty() || clientSecret.isEmpty())
    return;
  auto replyHandler = new QOAuthHttpServerReplyHandler(8080, this);
  wrapper.setReplyHandler(replyHandler);
  wrapper.setAuthorizationUrl(QUrl("https://accounts.spotify.com/authorize"));
  wrapper.setAccessTokenUrl(QUrl("https://accounts.spotify.com/api/token"));
  wrapper.setClientIdentifier(clientID);
  wrapper.setClientIdentifierSharedKey(clientSecret);
  wrapper.setScope("user-read-currently-playing user-modify-playback-state");
  if(clientRefreshToken.isEmpty())//если нет токена для обновления доступа, авторизируемся и подтягиваем его
    procceedAuth();
  else
  {
    wrapper.setRefreshToken(clientRefreshToken);
    refreshToken();
  }

}

void SpotifyWrapper::procceedAuth()
{
  connect(&wrapper,&QOAuth2AuthorizationCodeFlow::authorizeWithBrowser,this,&QDesktopServices::openUrl);
  connect(&wrapper,&QOAuth2AuthorizationCodeFlow::granted,
          [=](){clientRefreshToken=wrapper.refreshToken();refreshToken();});
  wrapper.grant();
}

void SpotifyWrapper::refreshToken()
{
  //так как обновление токена это не oauth, делаем обычный пост запрос
  //https://developer.spotify.com/documentation/web-api/tutorials/refreshing-tokens
  QNetworkAccessManager *manager=new QNetworkAccessManager(this);
  QUrl url("https://accounts.spotify.com/api/token");
  QNetworkRequest request(url);
  QString header="Basic "+QByteArray(QString(clientID+":"+clientSecret).toUtf8()).toBase64();
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
  request.setRawHeader("Authorization",header.toUtf8());
  //добавляем параметры к запросу
  QUrlQuery params;
  params.addQueryItem("grant_type", "refresh_token");
  params.addQueryItem("refresh_token", clientRefreshToken);
  //когда получили реквест, парсим и вітягиваем из него что нужно
  connect(manager,&QNetworkAccessManager::finished,
          [=](QNetworkReply* reply){
            if(reply->error())
            {
              emit responseText( reply->errorString());
              reply->deleteLater();
              return;
            }
            QJsonDocument doc=QJsonDocument::fromJson(reply->readAll());
            QJsonObject obj=doc.object();
            clientToken=obj["access_token"].toString();
            tokenExpireTime=QTime().msecsSinceStartOfDay()+obj["expires_in"].toInt();
            wrapper.setToken(clientToken);
            reply->deleteLater();
          });
  manager->post(request,params.query().toUtf8());

}

void SpotifyWrapper::saveInfo()
{
  QSettings info("clientInfo.ini",QSettings::Format::IniFormat);
  info.beginGroup("ClientInfo");
  info.setValue("clientID",clientID);
  info.setValue("clientSecret",clientSecret);
  info.setValue("clientRefreshToken",clientRefreshToken);
  info.endGroup();
}
