#ifndef FILELOADER_H
#define FILELOADER_H

#include <QObject>
#include <QTextStream>
#include "graphdata.h"
#include <QFile>
#include <QFileInfo>


/*!
  * \brief Error codes for FileLoader class
  */
typedef enum {
    HEADER_FORMAT_ERROR          = -1,
    NO_DATA_INFILE_ERROR         = -2,
    MAXIMUM_STRINGS_FORMAT_ERROR = - 3,

} FileLoaderErrorCodes;

/*!
    \brief Класс реализует загрузку данных из файла в контейнер для дальнейшей работы и хранения.

    Выполняется в отдельном потоке. Отправляет сигналы классу loadfiledialog об ошибках
    и окончании чтения.
*/
class FileLoader : public QObject
{
    Q_OBJECT

static const unsigned int count_of_string_in_header = 3;
static const unsigned int maximum_data_string_count = 5000000;
static const unsigned char feature_of_string_in_header = '#';
static const unsigned int estimated_string_size_bytes = 42;

public:
    explicit FileLoader(QObject *parent, QFile &in_file, GraphData *data);
    void beginWorking();
    ~FileLoader();

private:
    QThread *thread;
    QFile &in_file;
    GraphData *data;
    QFileInfo f_info;

    ///примерное количество строк в файле
    unsigned int estimated_string_count;

    ///преобразование считанных строк в вектор значений параметров
    bool parseHeader(QFile &f);
    bool parseData(QFile &f);

signals:
    void workOnePercentDone();
    void workFinished();
    void formatError(int error_code);
    void workProgress(unsigned char percent);

public slots:
    void doWork();
};

#endif // FILELOADER_H
