#ifndef LOADFILEDIALOG_H
#define LOADFILEDIALOG_H

#include <QDialog>
#include "fileloader.h"

namespace Ui {
class LoadFileDialog;
}

/*!
    \brief Класс отображения прогрессбара и ошибок при чтении данных из файла.
*/
class LoadFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoadFileDialog(QWidget *parent, FileLoader *fl);
    ~LoadFileDialog();

private slots:
    void on_buttonBox_rejected();

public slots:
    void format_error(int error_code);
    void setPBValue(unsigned char new_val_percent);

private:
    Ui::LoadFileDialog *ui;
    FileLoader *file_loader;
};

#endif // LOADFILEDIALOG_H
