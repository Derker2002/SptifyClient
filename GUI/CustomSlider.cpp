#include "CustomSlider.h"
#include <QMouseEvent>
CustomSlider::CustomSlider(QWidget *parent)
    : QSlider{parent}
{
  connect(this,&CustomSlider::valueChanged,this,&CustomSlider::previewValue);

  setColors();

}

void CustomSlider::updateSliderColors(QColor newColor)
{

  leftLine=newColor;
  rightLine=leftLine;
  rightLine.setAlphaF(0.35);
  handleHover=leftLine;
  handleBase=leftLine;
  setColors();
}
// QTC_TEMP
void CustomSlider::setColors()
{
  QString str=sliderStyle
                    .arg(handleBase.name(QColor::HexArgb))
                    .arg(handleHover.name(QColor::HexArgb))
                    .arg(rightLine.name(QColor::HexArgb))
                    .arg(leftLine.name(QColor::HexArgb));
  setStyleSheet(str);
}

void CustomSlider::mousePressEvent(QMouseEvent *event)
{
  if(event->button()==Qt::LeftButton)
  {m_busy=true;}
  QSlider::mousePressEvent(event);
}

void CustomSlider::mouseReleaseEvent(QMouseEvent *event)
{
  if(event->button()==Qt::LeftButton)
  {m_busy=false;emit newValue(value());}
  QSlider::mouseReleaseEvent(event);
}

void CustomSlider::wheelEvent(QWheelEvent *event)
{
  if(disableWheel)
    return;
  emit newValue(value());
  QSlider::wheelEvent(event);
}
