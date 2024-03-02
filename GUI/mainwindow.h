#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QSettings>
#include <QTimer>
#include "SpotifyWrapper/SpotifyWrapper.h"
#include <QSystemTrayIcon>
#include <QMenu>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

enum type{Spotify,Raw};

class MainWindow : public QMainWindow
{
    Q_OBJECT
  typedef void (MainWindow::*responsePtr)(QJsonDocument);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void startDrag(QPoint pos);
    void drag(QPoint pos);
    void stopDrag();
    void changeWidth(int delta);
    void addToStartup(bool add);
private:
    void showAccInfo(QJsonDocument doc);
    void showSongInfo(QJsonDocument doc);


    void playPause();
    void setNewTime(int time);
    void setNewVolume(int volume);
    void updateLoopMode(bool nextMode=true);
    void applyLoopModeGUI(QJsonDocument);
private:
    void initTray();
    void loadSettings();
    void saveSettings();

    void updateMainColor(QPixmap cover);
    void setupUiElems();

    void updateData();
    void processSpotifyRequestClick();
    void spotifyRequest(QUrl url,ReqType type,responsePtr func);

    void processRequestClick();
    void request(QUrl url,ReqType type,responsePtr func);

    void initProperties();
    void initConnections();

private:
    Ui::MainWindow *ui;
    SpotifyWrapper spotify;
    QGraphicsScene *scene;
    QGraphicsPixmapItem *albumCover;
    QNetworkAccessManager mngr;
    QString prevSongName,
        prevAuthors;

    QTimer updateTimer;

    QString baseStyle="*{background:transparent;} "
                        "*[responseWindow]{background:rgba(0, 0, 0, 0.2);} ";

    QString ctrlButtonsStyle="*[ctrlButton]{background:rgba(0, 0, 0, 0.2);"
                           "border-radius:5px;}"
                           "*[playButton]{border-radius:20px;}"
                           "*[ctrlButton]:hover{background:rgba(0, 0, 0, 0.4);}"
                           "*[ctrlButton]:pressed{background:rgba(0, 0, 0, 0.6);}"
                           "*[spotifyButton]{background:transparent;}"
                           "*[spotifyButton]:pressed{background:transparent;}";
    bool is_playing=false;
    bool is_draggable=false;
    int sliderUpdateSkips=0;
    bool timeChanged=false;
    int currLoopMode=0; // 0-off,1- track,2- context
    QString LoopModes[3]{"off","track","context"};

    QPoint delta;
    QSystemTrayIcon *trayIcon;
};
#endif // MAINWINDOW_H
