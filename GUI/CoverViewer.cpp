#include "CoverViewer.h"
#include <QMouseEvent>
CoverViewer::CoverViewer(QWidget *parent)
    : QGraphicsView{parent}
{}

void CoverViewer::mousePressEvent(QMouseEvent *event)
{
  if(event->button()==Qt::LeftButton)
    emit clicked(mapToGlobal(event->pos()));
  QGraphicsView::mousePressEvent(event);
}
void CoverViewer::mouseReleaseEvent(QMouseEvent *event)
{
  emit released();
  QGraphicsView::mouseReleaseEvent(event);
}
void CoverViewer::mouseMoveEvent(QMouseEvent *event)
{
  emit dragged(mapToGlobal(event->pos()));
  QGraphicsView::mouseMoveEvent(event);
}

void CoverViewer::wheelEvent(QWheelEvent *event)
{
  emit wheelScroll(event->angleDelta().y());
  QGraphicsView::wheelEvent(event);
}
