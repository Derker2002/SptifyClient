#ifndef PROGRESSSLIDER_H
#define PROGRESSSLIDER_H

#include <QWidget>
#include <QSlider>
#include <QPixmap>
#include <QImage>
class ProgressSlider : public QSlider
{
  Q_OBJECT
  public:
  explicit ProgressSlider(QWidget *parent = nullptr);

  public slots:
  void updateSliderColors(QPixmap cover);
  signals:
  private:
      void setColors();
  private:
      QString sliderStyle="*[songProgressSlider]::groove:horizontal{"
                            "background: transparent;"
                            "height:5px;"
                            "}"
                            // "*[songProgressSlider]::groove:horizontal:hover > .handle::horizontal{background:red; border-radius:5px;width: 11px; margin:-3px;}"
                            "*[songProgressSlider]::handle:horizontal{border-radius:5px;width: 11px; margin:-3px; background:%1;}"
                            "*[songProgressSlider]::handle:horizontal:hover{background:%2;}"
                            "*[songProgressSlider]::add-page:horizontal{background:%3;}"
                            "*[songProgressSlider]::sub-page:horizontal{background:%4;}";
      QColor handleBase,handleHover,leftLine,rightLine=QColor(0,0,0,60);
};

#endif // PROGRESSSLIDER_H
