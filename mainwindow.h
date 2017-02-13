#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPainter>

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

private:
    int m_screenwidth;
    int m_screenheight;

    bool m_isMousePress;
    QPoint m_beginPoint;
    QPoint m_endPoint;

    QPixmap m_loadPixmap;
    QPixmap m_capturePixmap;


};

#endif // MAINWINDOW_H
