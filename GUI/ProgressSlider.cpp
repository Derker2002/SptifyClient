#include "ProgressSlider.h"
#include <QMouseEvent>
ProgressSlider::ProgressSlider(QWidget *parent)
    : QSlider{parent}
{
  connect(this,&ProgressSlider::valueChanged,this,&ProgressSlider::previewTime);

  setColors();

}

void ProgressSlider::updateSliderColors(QPixmap cover)
{
  int r=0,g=0,b=0,count=0;
  QImage img=cover.toImage();
  QColor col;
  count=img.width()*img.height();
  for(int i=0;i<img.width();i++)
    for(int j=0;j<img.height();j++)
    {col=img.pixelColor(i,j);
      r+=col.red();
      g+=col.green();
      b+=col.blue();
    }
  leftLine=QColor(r/count,g/count,b/count);
  rightLine=leftLine;
  rightLine.setAlphaF(0.35);
  handleHover=leftLine;
  handleBase=leftLine;
  setColors();
}
// QTC_TEMP
void ProgressSlider::setColors()
{
  QString str=sliderStyle
                    .arg(handleBase.name(QColor::HexArgb))
                    .arg(handleHover.name(QColor::HexArgb))
                    .arg(rightLine.name(QColor::HexArgb))
                    .arg(leftLine.name(QColor::HexArgb));
  setStyleSheet(str);
}

void ProgressSlider::mousePressEvent(QMouseEvent *event)
{
  if(event->button()==Qt::LeftButton)
  {m_busy=true;}
  QSlider::mousePressEvent(event);
}

void ProgressSlider::mouseReleaseEvent(QMouseEvent *event)
{
  if(event->button()==Qt::LeftButton)
  {m_busy=false;emit newTime(value());}
  QSlider::mouseReleaseEvent(event);
}
