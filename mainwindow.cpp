#include "mainwindow.h"
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent),
      m_isMousePress(false),
      m_currentCaptureState(initCapture)
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
    if (event->button() == Qt::LeftButton && m_currentCaptureState == initCapture)
    {
        m_currentCaptureState = beginCaptureImage;
        m_beginPoint = event->pos();
    }
    else if (event->button() == Qt::LeftButton && isPressPointInSelectRect(event->pos()))
    {
        m_currentCaptureState = beginMoveCaptureArea;
        setCursor(Qt::SizeAllCursor);
        m_beginMovePoint = event->pos();
    }

    return QWidget::mousePressEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (m_currentCaptureState == beginCaptureImage)
    {
        m_endPoint = event->pos();
        update();
    }
    else if (m_currentCaptureState == beginMoveCaptureArea)
    {
        m_endMovePoint = event->pos();
        update();
    }

    if (isPressPointInSelectRect(event->pos()))
    {
        setCursor(Qt::SizeAllCursor);
    }
    else if (!isPressPointInSelectRect(event->pos()) &&
             m_currentCaptureState != beginMoveCaptureArea)
    {
        setCursor(Qt::ArrowCursor);
    }

    return QWidget::mouseMoveEvent(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_currentCaptureState == beginCaptureImage)
    {
        m_currentCaptureState = finishCaptureImage;
        m_endPoint = event->pos();
        update();
    }
    else if (m_currentCaptureState == beginMoveCaptureArea)
    {
        m_currentCaptureState = finishMoveCaptureArea;
        m_endMovePoint = event->pos();
        update();
    }

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
    m_painter.begin(this);

    QColor shadowColor = QColor(0, 0, 0, 100);
    m_painter.setPen(QPen(QColor("#2CA7F8")));
    m_painter.drawPixmap(0, 0, m_loadPixmap);
    m_painter.fillRect(m_loadPixmap.rect(), shadowColor);

    switch (m_currentCaptureState)
    {
    case initCapture:
        break;
    case beginCaptureImage:
    case finishCaptureImage:
    case beginMoveCaptureArea:
    case finishMoveCaptureArea:
        m_currentSelectRect = getSelectRect();
        drawCaptureImage();
    default:
        break;
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

bool MainWindow::isPressPointInSelectRect(QPoint mousePressPoint)
{
    QRect selectRect = getRect(m_beginPoint, m_endPoint);

    if (selectRect.contains(mousePressPoint))
    {
        return true;
    }

    return false;
}

QRect MainWindow::getSelectRect()
{
    if (m_currentCaptureState == beginCaptureImage ||
        m_currentCaptureState == finishCaptureImage)
    {
        return getRect(m_beginPoint, m_endPoint);
    }
    else if (m_currentCaptureState == beginMoveCaptureArea ||
             m_currentCaptureState == finishMoveCaptureArea)
    {
        return getMoveRect();
    }

    return QRect(0, 0, 0, 0);
}

void MainWindow::drawCaptureImage()
{
    m_capturePixmap = m_loadPixmap.copy(m_currentSelectRect);
    m_painter.drawPixmap(m_currentSelectRect.topLeft(), m_capturePixmap);
    m_painter.drawRect(m_currentSelectRect);
}

QRect MainWindow::getMoveRect()
{
    QPoint movePoint = getMovePoint();
    QPoint beginPoint = m_beginPoint + movePoint;
    QPoint endPoint = m_endPoint + movePoint;

    if (m_currentCaptureState == finishMoveCaptureArea)
    {
        m_beginPoint = beginPoint;
        m_endPoint = endPoint;
        m_beginMovePoint = QPoint(0, 0);
        m_endMovePoint = QPoint(0, 0);
    }

    return getRect(beginPoint, endPoint);
}

QPoint MainWindow::getMovePoint()
{
    QPoint movePoint = m_endMovePoint - m_beginMovePoint;
    QRect currentRect = getRect(m_beginPoint, m_endPoint);

    if (currentRect.topLeft().x() + movePoint.x() < 0)
    {
        movePoint.setX(0 - currentRect.topLeft().x());
    }

    if (currentRect.topLeft().y() + movePoint.y() < 0)
    {
        movePoint.setY(0 - currentRect.topLeft().y());
    }

    if (currentRect.bottomRight().x() + movePoint.x() > m_screenwidth)
    {
        movePoint.setX(m_screenwidth - currentRect.bottomRight().x());
    }

    if (currentRect.bottomRight().y() + movePoint.y() > m_screenheight)
    {
        movePoint.setY(m_screenheight - currentRect.bottomRight().y());
    }

    return movePoint;
}
