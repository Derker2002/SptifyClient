#ifndef CUSTOMSLIDER_H
#define CUSTOMSLIDER_H

#include <QWidget>
#include <QSlider>
#include <QPixmap>
#include <QImage>
class CustomSlider : public QSlider
{
  Q_OBJECT
  public:
  explicit CustomSlider(QWidget *parent = nullptr);

    bool isBusy(){return m_busy;};
    void setUseWheel(bool use){disableWheel=!use;}
  public slots:
    void updateSliderColors(QColor newColor);
  signals:
    void previewValue(int value);
      void newValue(int value);
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
    bool m_busy=false;
    bool disableWheel=false;
  protected:
      void mousePressEvent(QMouseEvent *event);
      void mouseReleaseEvent(QMouseEvent *event);
      void wheelEvent(QWheelEvent *event);
};

#endif // CUSTOMSLIDER_H
