#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>
#include <QPainter>
#include "graphdata.h"

#include <loadfiledialog.h>
#include "fileloader.h"
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(this, SIGNAL(exit()), qApp, SLOT(quit()));

    plot_drawed_flag = false;
    plotter = new GraphPlotter();
    data_from_file = new GraphData();

    applicationName = MainWindow::windowTitle();

    MainWindow::setCentralWidget(plotter);
}

MainWindow::~MainWindow()
{    
    delete ui;
}

/*!
 * \brief MainWindow::drawPlot расчет и отображение кривой
 */
void MainWindow::drawPlot()
{
    plotter->clearData();
    plotter->updateSizes();
    //обработать считанные данные, обновить параметры отображения и заголовок
    if (data_from_file->convertData(plotter->getXsize(), plotter->getYsize(), plotter->points_to_plot, &plotter->params) == true)
    {
        currentFileName = data_from_file->getFileName();
        data_from_file->convertHeader(plotter->plot_title);
        plotter->refreshPixmap();
    }

    plot_drawed_flag = true;
    MainWindow::setWindowTitle(applicationName + " - " + currentFileName);
}

/*!
 * \brief MainWindow::on_actionOpen_triggered Обработка нажатия на кнопку открывания нового файла.
 */
void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File with results of measurments"), "",
        tr("Data files (*.txt);; All files (*.*)"));

    if (fileName != "") {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }

        ///чтение файла:
        FileLoader *fl = new FileLoader(NULL, file, data_from_file);
        LoadFileDialog *dlg = new LoadFileDialog(NULL, fl);

        if (dlg->exec() == QDialog::Rejected)
        {
            qDebug() << "if (dialog->exec() == QDialog::reject())";
        }
        else
        {
            qDebug() << "if (dialog->exec() != QDialog::reject())";                       
            drawPlot();
        }

        file.close();
        delete dlg;
    }
}

/*!
 * \brief MainWindow::resizeEvent
 */
void MainWindow::resizeEvent(QResizeEvent*)
{
    if (plot_drawed_flag)
        drawPlot();
}

/*!
 * \brief MainWindow::on_actionQuit_triggered
 */
void MainWindow::on_actionQuit_triggered()
{
    qDebug() << "Application closed";
    emit exit();
}

/*!
 * \brief MainWindow::on_actionSave_as_png_triggered Сохраняет текущий график как картинку
 */
void MainWindow::on_actionSave_as_png_triggered()
{
    if (plot_drawed_flag)
    {
        plotter->savePNG(currentFileName);
    }
}

/*!
 * \brief MainWindow::on_actionLow_frequency_triggered
 */
void MainWindow::on_actionLow_frequency_triggered()
{
    data_from_file->setFreqFilter(LOW_FREQ);
    if (plot_drawed_flag)
        drawPlot();
}

/*!
 * \brief MainWindow::on_actionHigh_frequency_triggered
 */
void MainWindow::on_actionHigh_frequency_triggered()
{
    data_from_file->setFreqFilter(HIGH_FREQ);
    if (plot_drawed_flag)
        drawPlot();
}

/*!
 * \brief MainWindow::on_actionAs_is_triggered
 */
void MainWindow::on_actionAs_is_triggered()
{
    data_from_file->setFreqFilter(AS_IS_FREQ);
    if (plot_drawed_flag)
        drawPlot();
}

/*!
 * \brief MainWindow::on_actionSpectrum_triggered
 */
void MainWindow::on_actionSpectrum_triggered()
{

}

void MainWindow::on_actionAbout_triggered()
{

}
