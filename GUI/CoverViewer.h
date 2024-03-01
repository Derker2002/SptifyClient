#ifndef COVERVIEWER_H
#define COVERVIEWER_H

#include <QWidget>
#include <QGraphicsView>
class CoverViewer : public QGraphicsView
{
  Q_OBJECT
  public:
  explicit CoverViewer(QWidget *parent = nullptr);

  signals:
    void clicked(QPoint mousePos);
    void dragged(QPoint newMousePos);
    void released();
    void wheelScroll(int delta);
      // QWidget interface
  protected:
      void mousePressEvent(QMouseEvent *event);
      void mouseReleaseEvent(QMouseEvent *event);
      void mouseMoveEvent(QMouseEvent *event);
      void wheelEvent(QWheelEvent *event);
};

#endif // COVERVIEWER_H
