#include "paintwidget.h"
#include <QKeyEvent>
#include <QFileDialog>
#include <QMessageBox>

void PaintWidget::setIsShiftPressed(bool value)
{
    isShiftPressed = value;
}

//-------------------------------------------SET TOOLS AND PROPERTIES---------------------------------------------

void PaintWidget::setType_fill(const TypeFill &value)
{
    type_fill = value;
    if(type_fill == TypeFill::NoFill) {
        pen.setStyle(Qt::PenStyle::SolidLine);
        brush.setStyle(Qt::BrushStyle::NoBrush);
    }
    if(type_fill == TypeFill::Full) {
        brush.setColor(fillColor);
        brush.setStyle(Qt::BrushStyle::SolidPattern);

        if(currentTool != Tool::Pen && currentTool != Tool::Brush) {
            pen.setStyle(Qt::PenStyle::NoPen);
        }
    }
    if(type_fill == TypeFill::WithBorder) {
        pen.setStyle(Qt::PenStyle::SolidLine);
        brush.setColor(fillColor);
        brush.setStyle(Qt::BrushStyle::SolidPattern);
    }
}

void PaintWidget::setLine_width(unsigned int value)
{
    lineWidth = value;
    pen.setWidth(lineWidth);
}

bool PaintWidget::isSceneEmpty()
{
    return scene.isEmpty();
}

PaintWidget::PaintWidget(QWidget *parent) : QWidget(parent)
{
}

void PaintWidget::setLineColor(const QColor &color)
{
    lineColor = color;
    pen.setColor(lineColor);
}

void PaintWidget::setFillColor(const QColor& color)
{
    fillColor = color;
    brush.setColor(fillColor);
}

void PaintWidget::setCurrentPaintTool(Tool tool)
{
    currentTool = tool;
    if(currentTool == Tool::Brush) {
        pen.setStyle(Qt::PenStyle::SolidLine);
        pen.setCapStyle(Qt::PenCapStyle::RoundCap);
        return;
    }
    if(currentTool == Tool::Pen) {
        pen.setStyle(Qt::PenStyle::SolidLine);
        pen.setCapStyle(Qt::PenCapStyle::SquareCap);
        return;
    }
    if(currentTool == Tool::Rect || currentTool == Tool::Circle || currentTool == Tool::Triangle) {
        pen.setCapStyle(Qt::PenCapStyle::SquareCap);
        if(type_fill == TypeFill::Full)
            pen.setStyle(Qt::PenStyle::NoPen);
    }
}

//-------------------------------------------PAINT EVENT----------------------------------------------------

void PaintWidget::paintEvent(QPaintEvent *event)
{
    if(event == nullptr) return;

    QPainter previewPainter(this);
    for(const auto& i : scene) {
        previewPainter.drawPicture(0, 0, i);
    }
    previewPainter.setPen(pen);
    previewPainter.setBrush(brush);

    if(isMouseButtonPressed) {
        if(currentTool == Tool::Pen) drawPen(previewPainter);
        if(currentTool == Tool::Brush) drawBrush(previewPainter);
        if(currentTool == Tool::Line) drawLine(previewPainter);
        if(currentTool == Tool::Rect) drawRect(previewPainter);
        if(currentTool == Tool::Circle) drawCircle(previewPainter);
        if(currentTool == Tool::Triangle) drawTriangle(previewPainter);
    }
    QWidget::paintEvent(event);

}

//-------------------------------------------MOUSE EVENTS------------------------------------

void PaintWidget::mousePressEvent(QMouseEvent *event)
{
    if(event == nullptr) return;
    first_point = event->pos();
    isMouseButtonPressed = true;
}

void PaintWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event == nullptr) return;
    isMouseButtonPressed = false;
    QPicture pictureToSave;
    QPainter painter(&pictureToSave);
    painter.setPen(pen);
    painter.setBrush(brush);

    if(currentTool == Tool::Line) drawLine(painter);
    if(currentTool == Tool::Rect) drawRect(painter);
    if(currentTool == Tool::Circle) drawCircle(painter);
    if(currentTool == Tool::Triangle) drawTriangle(painter);

    scene.push_back(pictureToSave);
    redoPictures.clear();
    update();
}

void PaintWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event == nullptr) return;
    second_point = event->pos();
    if((currentTool == Tool::Brush || currentTool == Tool::Pen) && isMouseButtonPressed) {
        QPicture pictureToSave;
        QPainter painter(&pictureToSave);
        painter.setPen(pen);
        if(currentTool == Tool::Pen) drawPen(painter);
        if(currentTool == Tool::Brush) drawBrush(painter);
        scene.push_back(pictureToSave);
        first_point = second_point;
    }
    update();
}

//-------------------------------------------DRAW FIGURES----------------------------------------------------------

void PaintWidget::drawLine(QPainter &painter)
{
    painter.drawLine(first_point, second_point);
}

void PaintWidget::drawRect(QPainter &painter)
{
    QRect rect(first_point, second_point);
    painter.drawRect(rect);
}

void PaintWidget::drawCircle(QPainter &painter)
{
    QRect rect(first_point, second_point);
    painter.drawEllipse(rect);
}

void PaintWidget::drawBrush(QPainter &painter)
{
    painter.drawLine(first_point, second_point);
}

void PaintWidget::drawPen(QPainter &painter)
{
    painter.drawLine(first_point, second_point);
}

void PaintWidget::drawTriangle(QPainter &painter)
{
    QPolygon poly;
    poly << first_point << second_point << QPoint(first_point.x(), second_point.y());
    painter.drawPolygon(poly, Qt::FillRule::WindingFill);
}

//-------------------------------------------NEW/OPEN/SAVE OPERATIONS-------------------------------------------

void PaintWidget::NewFile()
{
    if(scene.isEmpty()) return;
    else {
        QMessageBox warn("Warning!", "Scene is not empty", QMessageBox::Icon::Warning, QMessageBox::Button::Discard, QMessageBox::Button::Cancel, QMessageBox::Button::NoButton, this);
        int choise = warn.exec();
        if(choise == QMessageBox::Cancel) return;
    }
    scene.clear();
    update();
}

void PaintWidget::OpenFile()
{
    auto filename = QFileDialog::getOpenFileName(this);
    if(filename.isEmpty()) return;
    QPicture picture;

    if(!picture.load(filename)) {
        QMessageBox error("Something went wrong", "Very informative error", QMessageBox::Icon::Critical, QMessageBox::Button::Apply, QMessageBox::Button::Apply, QMessageBox::Button::Apply, this);
        error.exec();
        return;
    }

    scene.push_back(picture);
    update();
}

void PaintWidget::SaveFile()
{
    if(scene.isEmpty()) return;
    auto filename = QFileDialog::getSaveFileName(this);
    if(filename.isEmpty()) return;
    QPicture picture;
    QPainter painter(&picture);
    for(const auto& i : scene) {
        painter.drawPicture(0, 0, i);
    }
    painter.end();

    if(!picture.save(filename)) {
        QMessageBox error("Something went wrong", "Very informative error", QMessageBox::Icon::Critical, QMessageBox::Button::Apply, QMessageBox::Button::Apply, QMessageBox::Button::Apply, this);
        error.exec();
    }
}

void PaintWidget::Redo()
{
    if(redoPictures.isEmpty()) return;
    scene.push_back(redoPictures.pop());
    update();
}

void PaintWidget::Undo()
{
    if(scene.isEmpty()) return;
    redoPictures.push(scene.back());
    scene.pop_back();
    update();
}
