#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include "painttools.h"
#include <QWidget>
#include <QList>
#include <QStack>
#include <QPicture>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QLine>
#include <QRect>
#include <QColor>
#include <QPoint>


class PaintWidget : public QWidget
{
    Q_OBJECT

    QList<QPicture> scene;
    QStack<QPicture> redoPictures;

    QPoint first_point;
    QPoint second_point;

    QColor lineColor;
    QColor fillColor;
    QPen pen;
    QBrush brush;

    unsigned int lineWidth = 1;

    Tool currentTool;
    TypeFill typeFill;

    bool isMouseButtonPressed = false;
    bool isFill = false;
    bool isShiftPressed = false;

    void drawPen(QPainter &painter);
    void drawBrush(QPainter &painter);
    void drawLine(QPainter& painter);
    void drawRect(QPainter &painter);
    void drawCircle(QPainter &painter);
    void drawTriangle(QPainter &painter);

public:
    explicit PaintWidget(QWidget *parent = nullptr);
    void setLineColor(const QColor& color);
    void setFillColor(const QColor& color);
    void setCurrentPaintTool(Tool tool);
    void setIsShiftPressed(bool value);
    void setTypeFill(const TypeFill &value);
    void setLine_width(unsigned int value);
    bool isSceneEmpty();

public slots:
    void Undo();
    void Redo();
    void NewFile();
    void OpenFile();
    void SaveFile();

signals:


    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;   
};

#endif // PAINTWIDGET_H
