#include "mainwindow.h"
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent),
      m_isMousePress(false)
{
    this->setMouseTracking(true);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setWindowState(Qt::WindowActive | Qt::WindowFullScreen);
    this->setMouseTracking(true);

    loadBackgroundPixmap();
}

MainWindow::~MainWindow()
{

}

void MainWindow::loadBackgroundPixmap()
{
    m_loadPixmap = QPixmap::grabWindow(QApplication::desktop()->winId());
    m_screenwidth = m_loadPixmap.width();
    m_screenheight = m_loadPixmap.height();
}
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_isMousePress = true;
        m_beginPoint = event->pos();
    }

    return QWidget::mousePressEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isMousePress)
    {
        m_endPoint = event->pos();
        update();
    }

    return QWidget::mouseMoveEvent(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    m_endPoint = event->pos();
    m_isMousePress = false;

    return QWidget::mouseReleaseEvent(event);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        close();
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter m_painter;
    m_painter.begin(this);

    QColor shadowColor = QColor(0, 0, 0, 100);
    m_painter.setPen(QPen(QColor("#2CA7F8")));
    m_painter.drawPixmap(0, 0, m_loadPixmap);
    m_painter.fillRect(m_loadPixmap.rect(), shadowColor);

    if (m_isMousePress)
    {
        QRect selectedRect = getRect(m_beginPoint, m_endPoint);
        m_capturePixmap = m_loadPixmap.copy(selectedRect);
        m_painter.drawPixmap(selectedRect.topLeft(), m_capturePixmap);
        m_painter.drawRect(selectedRect);
    }

    m_painter.end();
}

QRect MainWindow::getRect(const QPoint &beginPoint, const QPoint &endPoint)
{
    int x, y;
    int width, height;

    width = qAbs(beginPoint.x() - endPoint.x());
    height = qAbs(beginPoint.y() - endPoint.y());
    x = beginPoint.x() < endPoint.x() ? beginPoint.x() : endPoint.x();
    y = beginPoint.y() < endPoint.y() ? beginPoint.y() : endPoint.y();

    QRect selectedRect = QRect(x, y, width, height);

    if (selectedRect.width() == 0)
    {
        selectedRect.setWidth(1);
    }

    if (selectedRect.height() == 0)
    {
        selectedRect.setHeight(1);
    }

    return selectedRect;
}
