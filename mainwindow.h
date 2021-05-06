#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QActionGroup *toolbarActionsGroup;
    QActionGroup *fillbarActionsGroup;
    QLabel *labelPenWidth;
    QSlider *sliderPenWidth;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int getCurrentPaintTool();

private slots:
    void on_actionSelect_Color_triggered();

    void on_actionFill_Color_triggered();

    void on_actionSwitch_to_Rect_triggered();

    void on_actionsetGradient_triggered();

    void on_actionSwitch_to_Line_triggered();

    void on_actionSwitch_to_Brush_triggered();

    void on_actionSwitch_to_Circle_triggered();

    void on_actionSwitch_to_Triangle_triggered();

    void on_actionSelect_fill_with_border_triggered();

    void on_actionSwitch_to_Pen_triggered();

    void on_actionSelect_no_fill_triggered();

    void on_actionSelect_full_fill_triggered();

    void changeLineWidth();

    void on_actionExit_triggered();

private:
    Ui::MainWindow *ui;

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
