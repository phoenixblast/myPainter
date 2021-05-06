#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QColorDialog>
#include <QKeyEvent>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    labelPenWidth = new QLabel();
    if(labelPenWidth) {
        ui->fileToolbar->addWidget(labelPenWidth);

    }
    if(sliderPenWidth) {
        sliderPenWidth = new QSlider(this);
        ui->fileToolbar->addWidget(sliderPenWidth);
        sliderPenWidth->setOrientation(Qt::Orientation::Horizontal);
        sliderPenWidth->setFixedWidth(100);
        sliderPenWidth->setTickInterval(1);
        sliderPenWidth->setRange(1,50);
        labelPenWidth->setText("Line width: " +  QString::number(sliderPenWidth->value()));
        connect(sliderPenWidth, SIGNAL(sliderReleased()), this, SLOT(changeLineWidth()));
    }

    toolbarActionsGroup = new QActionGroup(this);
    if(toolbarActionsGroup) {
        toolbarActionsGroup->addAction(ui->actionSwitch_to_Pen);
        toolbarActionsGroup->addAction(ui->actionSwitch_to_Brush);
        toolbarActionsGroup->addAction(ui->actionSwitch_to_Line);
        toolbarActionsGroup->addAction(ui->actionSwitch_to_Rect);
        toolbarActionsGroup->addAction(ui->actionSwitch_to_Circle);
        toolbarActionsGroup->addAction(ui->actionSwitch_to_Triangle);
    }

    fillbarActionsGroup = new QActionGroup(this);
    if(fillbarActionsGroup) {
        fillbarActionsGroup->addAction(ui->actionSelect_no_fill);
        fillbarActionsGroup->addAction(ui->actionSelect_full_fill);
        fillbarActionsGroup->addAction(ui->actionSelect_fill_with_border);
    }

    connect(ui->actionUndo, SIGNAL(triggered()), ui->paintWidget, SLOT(Undo()));
    connect(ui->actionRedo, SIGNAL(triggered()), ui->paintWidget, SLOT(Redo()));
    connect(ui->actionNew, SIGNAL(triggered()), ui->paintWidget, SLOT(NewFile()));
    connect(ui->actionOpen_File, SIGNAL(triggered()), ui->paintWidget, SLOT(OpenFile()));
    connect(ui->actionSave_File, SIGNAL(triggered()), ui->paintWidget, SLOT(SaveFile()));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionSelect_Color_triggered()
{
    if(!ui->paintWidget) return;
    ui->paintWidget->setLineColor(QColorDialog::getColor());
}

void MainWindow::on_actionFill_Color_triggered()
{
    if(!ui->paintWidget) return;
    ui->paintWidget->setFillColor(QColorDialog::getColor());
}

void MainWindow::on_actionsetGradient_triggered()
{
    //TODO
}

void MainWindow::on_actionSwitch_to_Pen_triggered()
{
    if(!ui->paintWidget) return;
    ui->paintWidget->setCurrentPaintTool(Tool::Pen);
}

void MainWindow::on_actionSwitch_to_Brush_triggered()
{
    if(!ui->paintWidget) return;
    ui->paintWidget->setCurrentPaintTool(Tool::Brush);
}

void MainWindow::on_actionSwitch_to_Line_triggered()
{
    if(!ui->paintWidget) return;
    ui->paintWidget->setCurrentPaintTool(Tool::Line);
}

void MainWindow::on_actionSwitch_to_Rect_triggered()
{
    if(!ui->paintWidget) return;
    ui->paintWidget->setCurrentPaintTool(Tool::Rect);
}

void MainWindow::on_actionSwitch_to_Circle_triggered()
{
    if(!ui->paintWidget) return;
    ui->paintWidget->setCurrentPaintTool(Tool::Circle);
}

void MainWindow::on_actionSwitch_to_Triangle_triggered()
{
    if(!ui->paintWidget) return;
    ui->paintWidget->setCurrentPaintTool(Tool::Triangle);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(!event || !ui->paintWidget) return;
    if(event->key() == Qt::Key_Shift)
        ui->paintWidget->setIsShiftPressed(true);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(!event || !ui->paintWidget) return;
    ui->paintWidget->setIsShiftPressed(false);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(event && ui->paintWidget) {
        if(!ui->paintWidget->isSceneEmpty()) {
            QMessageBox warn("Warning!", "Scene is not empty", QMessageBox::Icon::Warning, QMessageBox::Button::Discard, QMessageBox::Button::Cancel, QMessageBox::Button::NoButton, this);
            int choise = warn.exec();
            if(choise == QMessageBox::Cancel) return;
        }
        QApplication::quit();
    }
    QCoreApplication::exit(-1);
}

void MainWindow::on_actionSelect_fill_with_border_triggered()
{
    if(!ui->paintWidget) return;
    ui->paintWidget->setType_fill(TypeFill::WithBorder);
}

void MainWindow::on_actionSelect_no_fill_triggered()
{
    if(!ui->paintWidget) return;
    ui->paintWidget->setType_fill(TypeFill::NoFill);
}

void MainWindow::on_actionSelect_full_fill_triggered()
{
    if(!ui->paintWidget) return;
    ui->paintWidget->setType_fill(TypeFill::Full);
}

void MainWindow::changeLineWidth()
{
    if(!ui->paintWidget || !sliderPenWidth) return;
    labelPenWidth->setText("Line width: " +  QString::number(sliderPenWidth->value()));
    ui->paintWidget->setLine_width(sliderPenWidth->value());
    update();
}

void MainWindow::on_actionExit_triggered()
{
    QCloseEvent closeApp;
    closeEvent(&closeApp);
}
