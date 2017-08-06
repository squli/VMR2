#include "loadfiledialog.h"
#include "ui_loadfiledialog.h"
#include <QDebug>
#include "fileloader.h"

LoadFileDialog::LoadFileDialog(QWidget *parent, FileLoader *fl) :
    QDialog(parent),
    ui(new Ui::LoadFileDialog)
{
    ui->setupUi(this);

    file_loader = fl;

    connect( file_loader, SIGNAL(workFinished()), this, SLOT(accept()));
    connect( file_loader, SIGNAL(formatError(int)), this, SLOT(format_error(int)));
    connect( file_loader, SIGNAL(workProgress(unsigned char)), this, SLOT(setPBValue(unsigned char)));

    fl->beginWorking();
}

LoadFileDialog::~LoadFileDialog()
{    
    delete ui;    
}

/*!
 * \brief LoadFileDialog::on_buttonBox_rejected
 */
void LoadFileDialog::on_buttonBox_rejected()
{
    qDebug() << "Load file: pressed cancel";
}

/*!
 * \brief LoadFileDialog::format_error
 * \param [in] error_code код ошибки из FileLoaderErrorCodes
 */
void LoadFileDialog::format_error(int error_code)
{
    ui->progressBar->setValue(100);
    ui->progressBar->setStyleSheet(ui->progressBar->property("defaultStyleSheet").toString() + " QProgressBar::chunk { background: red; }");

    QString err_d;

    if (error_code == HEADER_FORMAT_ERROR)
        err_d = "Header format error";
    else if (error_code == NO_DATA_INFILE_ERROR)
        err_d = "There are no data in file";
    else if (error_code == MAXIMUM_STRINGS_FORMAT_ERROR)
        err_d = "File size is too large";
    else if (error_code > 0)
    {
        err_d = "Format error in line " + QString::number(error_code);
    }
    else
    {
        err_d = "Unknown error";
    }

    ui->err_descr->setText(err_d);
    ui->err_descr->show();
}

/*!
 * \brief LoadFileDialog::setPBValue Обновление процентов при загрузке
 * \param new_val_percent
 */
void LoadFileDialog::setPBValue(unsigned char new_val_percent)
{
    ui->progressBar->setValue(new_val_percent);
}
