#include "GUI/mainwindow.h"

#include <QApplication>
#include <QStyleFactory>
void initApp()
{
  qApp->setStyle(QStyleFactory::create("fusion"));
  QPalette darkPalette;

  darkPalette.setColor(QPalette::WindowText, Qt::white);
  darkPalette.setColor(QPalette::Text,Qt::white);
  darkPalette.setColor(QPalette::ButtonText,Qt::white);
  // darkPalette.setColor(QPalette::Button,QColor(38,38,38));
  // darkPalette.setColor(QPalette::Base,QColor(45,45,45));
  // darkPalette.setColor(QPalette::Window,QColor(53,53,53));
  darkPalette.setColor(QPalette::Highlight,QColor(144,202,249));
  darkPalette.setColor(QPalette::HighlightedText,Qt::black);

  qApp->setPalette(darkPalette);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    initApp();
    MainWindow w;
    w.show();
    return a.exec();
}
