#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "GUI/CoverViewer.h"
#include <QUrlQuery>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  setMouseTracking(true);
  ui->responses->setVisible(false);
  setupUiElems();
  setStyleSheet(baseStyle+ctrlButtonsStyle);
  // updateSliderView(Qt::black);
  setAttribute(Qt::WA_TranslucentBackground);
  setWindowFlags(Qt::FramelessWindowHint |  Qt::WindowStaysOnBottomHint | Qt::Tool);



  scene=new QGraphicsScene();
  albumCover=new QGraphicsPixmapItem();
  // ui->coverView->installEventFilter(this);
  ui->coverView->setScene(scene);
  ui->coverView->setContentsMargins(QMargins(0,0,0,0));
  ui->coverView->viewport()->setFocusProxy(0);

  scene->addItem(albumCover);


  initTray();
  initProperties();
  initConnections();

  updateTimer.setInterval(500);

  updateTimer.start();
  loadPosition();
    // setGeometry(QRect(100,100,800,300));
}

MainWindow::~MainWindow()
{
  savePosition();
  delete ui;
}

void MainWindow::startDrag(QPoint pos)
{
  delta=mapFromGlobal(pos);
  is_draggable=true;
}

void MainWindow::drag(QPoint pos)
{
  if(!is_draggable)
    return;
  QPoint newPos=pos-delta;
  setGeometry(newPos.x(),newPos.y(),width(),height());}

void MainWindow::stopDrag()
{
  is_draggable=false;
}

void MainWindow::changeWidth(int delta)
{
  Qt::KeyboardModifiers modifiers = QGuiApplication::keyboardModifiers();
  bool ctrlMod=modifiers & Qt::ControlModifier;
  setGeometry(x(),y(),width()+(ctrlMod ? delta/5:delta),height());
}

void MainWindow::addToStartup(bool add)
{
  QString appName=QCoreApplication::applicationName();
  QString appPath=QCoreApplication::applicationFilePath();
  QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
  add? settings.setValue(appName,appPath): settings.remove(appName);
}

//spotify responses
void MainWindow::showAccInfo(QJsonDocument doc)
{
  QJsonObject obj = doc.object();
}
void MainWindow::showSongInfo(QJsonDocument doc)
{
  QJsonObject base=doc.object();
  is_playing=base["is_playing"].toBool();
  QJsonObject song =base["item"].toObject();
  ui->ppButton->setIcon(QPixmap(is_playing? ":/pause.png":":/play.png"));

  QString currSongName=song["name"].toString().toUpper();
  if(currSongName.contains('('))
    currSongName=currSongName.mid(0,currSongName.indexOf('('));
  QString authors;
  for (auto artist: song["artists"].toArray()) {
    authors+=','+artist.toObject()["name"].toString();
  }
  authors.remove(0,1);
  authors=authors.toUpper();
  if(prevSongName!=currSongName || prevAuthors!=authors)
  {
    int songLenth=song["duration_ms"].toInt();
    prevSongName=currSongName;
    prevAuthors=authors;
    ui->SongProgress->setMaximum(songLenth/1000);
    ui->songLength->setText(QTime::fromMSecsSinceStartOfDay(songLenth).toString("mm:ss"));
    ui->SongName->setText(currSongName);
    ui->Authors->setText(authors);

    QUrl url(song["album"].toObject()["images"].toArray()[1].toObject()["url"].toString());
    auto reply = mngr.get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished, [=]() {
      QPixmap pixmap;
      pixmap.loadFromData(reply->readAll());
      ui->SongProgress->updateSliderColors(pixmap);
      albumCover->setPixmap(pixmap);
      ui->coverView->fitInView(scene->itemsBoundingRect(),Qt::KeepAspectRatio);
      reply->deleteLater();
    });
  }

  int currProgress=base["progress_ms"].toInt();

  if(!ui->SongProgress->isBusy() && (!timeChanged || (timeChanged && abs(currProgress-ui->SongProgress->value()*1000)<2000)))
  // {sliderUpdateSkips=0;}
  // if(sliderUpdateSkips<3)
  //   sliderUpdateSkips++;
  // else
  {
    timeChanged=false;
    ui->SongProgress->setValue(currProgress/1000);
    ui->currSongTime->setText(QTime::fromMSecsSinceStartOfDay(currProgress).toString("mm:ss"));
  }

}

void MainWindow::playPause()
{

  spotifyRequest(is_playing? QUrl("https://api.spotify.com/v1/me/player/pause"):
                     QUrl("https://api.spotify.com/v1/me/player/play"),PUT,nullptr);
}

void MainWindow::setNewTime(int time)
{
  QUrl url("https://api.spotify.com/v1/me/player/seek");
  QUrlQuery query;
  query.addQueryItem("position_ms",QString::number(time*1000));
  url.setQuery(query);
  spotifyRequest(url,PUT,nullptr);
  timeChanged=true;
}

void MainWindow::initTray()
{
  trayIcon = new QSystemTrayIcon(QIcon(":/play.png"), this);

  QMenu *trayMenu = new QMenu(this);
  QAction *quitAction = new QAction("Выход", this);
  QAction *runWithSystem=new QAction("Запуск с системой",this);
  runWithSystem->setCheckable(true);
  QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
  runWithSystem->setChecked(settings.contains(QCoreApplication::applicationName()));

  connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);
  connect(runWithSystem,&QAction::triggered,this,&MainWindow::addToStartup);

  trayMenu->addAction(runWithSystem);
  trayMenu->addAction(quitAction);

  trayIcon->setContextMenu(trayMenu);

  trayIcon->show();
}

void MainWindow::loadPosition()
{
  QSettings settings("ui.ini",QSettings::IniFormat);
  settings.beginGroup("UI_Settings");
  setGeometry((settings.value("pos").value<QRect>()));
  settings.endGroup();
}
void MainWindow::savePosition()
{

  QSettings settings("ui.ini",QSettings::IniFormat);
  settings.beginGroup("UI_Settings");
  settings.setValue("pos",QVariant::fromValue(QRect(x(),y(),width(),height())));
  settings.endGroup();
}

void MainWindow::setupUiElems()
{
  ui->responses->setProperty("responseWindow",true);
  ui->SongProgress->setProperty("songProgressSlider",true);
  ui->backButton->setIcon(QPixmap(":/rewind.png"));
  ui->backButton->setProperty("ctrlButton",true);
  ui->ppButton  ->setIcon(QPixmap(":/play.png"));
  ui->ppButton  ->setProperty("ctrlButton",true);
  ui->ppButton  ->setProperty("playButton",true);
  ui->forwButton->setIcon(QPixmap(":/fast-forward.png"));
  ui->forwButton->setProperty("ctrlButton",true);
  ui->loopButton->setIcon(QPixmap(":/arrow.png"));
  ui->loopButton->setProperty("ctrlButton",true);
  ui->shuffleButton->setIcon(QPixmap(":/shuffle.png"));
  ui->shuffleButton->setProperty("ctrlButton",true);
  ui->openSpotifyButton->setIcon(QPixmap(":/spotify.png"));
  ui->openSpotifyButton->setProperty("spotifyButton",true);

}


void MainWindow::updateData()
{
  spotifyRequest(QUrl("https://api.spotify.com/v1/me/player/currently-playing"),GET,&MainWindow::showSongInfo);
}
void MainWindow::processSpotifyRequestClick()
{
  QVector<QVariant> info=sender()->property("info").value<QVector<QVariant>>();
  spotifyRequest(info[0].toUrl(),
                 info[1].value<ReqType>(),
                 info[2].value<responsePtr>());
}

void MainWindow::spotifyRequest(QUrl url,ReqType type, responsePtr func)
{
  auto reply=type==GET? spotify.get(url):type==POST? spotify.post(url):spotify.put(url);
  connect(reply, &QNetworkReply::finished, [=]() {
    if(!reply->error())
    {
      if(func) (this->*func)(QJsonDocument::fromJson(reply->readAll()));
    }else
    {
      ui->responses->appendPlainText(reply->errorString());
    }
    reply->deleteLater(); 
  });
}

void MainWindow::initProperties()
{
  ui->forwButton->setProperty("info",QVector<QVariant>{QUrl("https://api.spotify.com/v1/me/player/next"),
                                                      POST,
                                                      QVariant().fromValue(nullptr)});
  ui->backButton->setProperty("info",QVector<QVariant>{QUrl("https://api.spotify.com/v1/me/player/previous"),
                                                        POST,
                                                        QVariant().fromValue(nullptr)});
}
void MainWindow::initConnections()
{
  connect(&updateTimer,&QTimer::timeout,this,&MainWindow::updateData);

  connect(ui->SongProgress,&ProgressSlider::previewTime,[=](int value){ui->currSongTime->setText(QTime::fromMSecsSinceStartOfDay(value*1000).toString("mm:ss"));});
  connect(ui->SongProgress,&ProgressSlider::newTime,this,&MainWindow::setNewTime);

  connect(ui->coverView,&CoverViewer::clicked,this,&MainWindow::startDrag);
  connect(ui->coverView,&CoverViewer::dragged,this,&MainWindow::drag);
  connect(ui->coverView,&CoverViewer::released,this,&MainWindow::stopDrag);
  connect(ui->coverView,&CoverViewer::wheelScroll,this,&MainWindow::changeWidth);

  connect(ui->openSpotifyButton,&QPushButton::clicked,[=](){system("start spotify");});

  connect(ui->forwButton,&QPushButton::clicked,this,&MainWindow::processSpotifyRequestClick);
  connect(ui->backButton,&QPushButton::clicked,this,&MainWindow::processSpotifyRequestClick);
  connect(ui->ppButton,&QPushButton::clicked,this,&MainWindow::playPause);

}


