#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPainter>

enum CaptureState
{
    initCapture,
    beginCaptureImage,
    finishCaptureImage,
    beginMoveCaptureArea,
    finishMoveCaptureArea,
};
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void loadBackgroundPixmap();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);

    QRect getRect(const QPoint &beginPoint, const QPoint &endPoint);
    bool isPressPointInSelectRect(QPoint mousePressPoint);

    QRect getSelectRect();
    void drawCaptureImage();

    QRect getMoveRect();
    QPoint getMovePoint();


private:
    QPainter m_painter;

    int m_screenwidth;
    int m_screenheight;

    bool m_isMousePress;

    QPoint m_beginPoint;
    QPoint m_endPoint;
    QPoint m_beginMovePoint;
    QPoint m_endMovePoint;

    QPixmap m_loadPixmap;
    QPixmap m_capturePixmap;

    CaptureState m_currentCaptureState;
    QRect m_currentSelectRect;
};

#endif // MAINWINDOW_H
