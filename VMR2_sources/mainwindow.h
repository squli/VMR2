#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "graphplotter.h"

namespace Ui {
class MainWindow;
}

/*!
    \brief Главное окно приложения.
*/
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *e);

private slots:
    void on_actionOpen_triggered();
    void on_actionQuit_triggered();
    void on_actionSave_as_png_triggered();

    void on_actionLow_frequency_triggered();

    void on_actionHigh_frequency_triggered();

    void on_actionAs_is_triggered();

    void on_actionSpectrum_triggered();

    void on_actionAbout_triggered();

signals:
    void exit();

private:
    Ui::MainWindow *ui;
    GraphData *data_from_file;
    GraphPlotter *plotter;
    QString currentFileName;
    QString applicationName;

    bool plot_drawed_flag;
    void drawPlot();
};

#endif // MAINWINDOW_H
