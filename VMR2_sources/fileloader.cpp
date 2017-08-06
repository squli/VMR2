#include "fileloader.h"
#include <QDebug>
#include <QRegExpValidator>
#include <QThread>

FileLoader::FileLoader(QObject*, QFile &f, GraphData *d): in_file(f), data(d)
{
    f_info = QFileInfo(f);

    ///очистка старых данных
    d->curve.erase(d->curve.begin(), d->curve.end());
    d->getCurveHeader()->erase(d->getCurveHeader()->begin(), d->getCurveHeader()->end());

    estimated_string_count = f_info.size() / estimated_string_size_bytes;

    thread = new QThread();
    connect( thread, SIGNAL(started()), this, SLOT(doWork()) );
    connect( this, SIGNAL(workFinished()), thread, SLOT(quit()) );
    connect( thread, SIGNAL(finished()), this, SLOT(deleteLater()) );
    connect( thread, SIGNAL(finished()), thread, SLOT(deleteLater()) );
    thread->start();
}

FileLoader::~FileLoader()
{
    thread->quit();
}

/*!
 * \brief FileLoader::beginWorking запуск потока чтения файла на исполнение
 */
void FileLoader::beginWorking()
{
    this->moveToThread(thread);
}

/*!
 * \brief FileLoader::parseHeader чтение из принятого файла заголовка и проверка его корректности
 * \param [in] f ссылка на файл с данными
 * \return
 */
bool FileLoader::parseHeader(QFile &f)
{
    QString line;
    QStringList pv;
    std::vector<QString>input_lines;

    for (size_t i = 0; i < count_of_string_in_header; ++i)
    {
        line = f.readLine();
        if ( (!f.atEnd()) && (line[0] == feature_of_string_in_header) )
        {
            line.remove(0, 1);
            input_lines.push_back(line);
            qDebug() << line;
        }
        else
        {
            qDebug() << "Format error - count_of_string_in_header ";
            emit formatError(HEADER_FORMAT_ERROR);
            return false;
        }
    }

    if (input_lines.size() != count_of_string_in_header)
    {
        qDebug() << "if (in_l.size() != count_of_string_in_header) in_l.size() = " << input_lines.size();
        return false;
    }

    ///первая строка
    pv = input_lines.begin()->split(",");
    if (!pv.empty() && pv.size() > 1)
    {
        data->getCurveHeader()->push_back(pv[0]);
        data->getCurveHeader()->push_back(pv[1]);
    }
    else
    {
        qDebug() << "if (!pv.empty() && pv.size() > 2)" << pv.size();
        return false;
    }

    ///остальные
    for (size_t i = 1; i < count_of_string_in_header; ++i)
    {
        data->getCurveHeader()->push_back(input_lines.at(i));
    }

    return true;
}

/*!
 * \brief FileLoader::parseData чтение из принятого файла строк данных и проверка корректности
 * \param [in] f ссылка на файл с данными
 * \return
 */
bool FileLoader::parseData(QFile &f)
{
    QString line;
    QStringList pv;
    double d1 = 0;
    double d2 = 0;
    bool ok1 = false;
    bool ok2 = false;
    unsigned char current_percent = 0;
    size_t data_line_count = 0;

    do
    {
        line = f.readLine();
        if (line.size() > 0)
        {
            //qDebug() << "line:" << line;

            pv = line.split(" ");
            //qDebug() << "pv:" << pv;
            bool res = true;

            if (!pv.empty() && pv.size() == 2)
            {
                d1 = pv[0].toDouble(&ok1);
                d2 = pv[1].toDouble(&ok2);

                if (ok1 && ok2)
                {
                    data->curve.push_back(std::pair<double, double>(d1, d2));
                    data_line_count += 1;

                    if ((estimated_string_count / 100) > 0)
                    {
                        if ( data_line_count / (estimated_string_count/100) > current_percent)
                        {
                            current_percent += 1;
                            emit workProgress(current_percent);
                        }
                    }

                    if (data_line_count > maximum_data_string_count)
                    {
                        emit formatError(MAXIMUM_STRINGS_FORMAT_ERROR);
                    }
                }
                else
                    res = false;
            }
            else if (line == "\r\n")
            {

            }
            else
                res = false;

            if (!res)
            {
                emit formatError(data_line_count);
                return false;
            }
        }
    } while (!in_file.atEnd());

    if (data->curve.empty())
    {
        qDebug() << "Format error - file not contains data";
        emit formatError(NO_DATA_INFILE_ERROR);
        return false;
    }
    else
        return true;
}

void FileLoader::doWork()
{
    //qDebug() << "File size = " << this->f_info.size();
    //qDebug() << "Estimated string count = " << estimated_string_count ;

    ///проверка заголовка
    if (!parseHeader(in_file))
    {
        emit formatError(HEADER_FORMAT_ERROR);
        return;
    }

    if (parseData(in_file))
    {
        qDebug() << "Read from file - OK " << data->curve.size();
        data->setFileName(f_info.fileName().replace('.', ""));
        emit workFinished();
    }
    else
    {
        qDebug() << "Something wrong";
        return;
    }
}
