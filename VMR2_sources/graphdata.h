#ifndef GRAPHDATA_H
#define GRAPHDATA_H

#include <vector>
#include <map>
#include <QTextStream>
#include "axisparam.h"

typedef enum {
    HEADER_COMPANY_NAME_OFFSET   = 0,
    HEADER_SOFT_VERSION_OFFSET   = 1,
    HEADER_MEASURMENT_TYPE       = 2,
    HEADER_MEASURMENT_TIME_BEGIN = 3,
} HEADER_OFFSETS;

typedef enum {
    AS_IS_FREQ = 0,
    LOW_FREQ   = 1,
    HIGH_FREQ  = 2,
} frequency_filter_typedef;

/*!
    \brief Класс реализует контейнер для считанных из файла данных, предоставляет функции для его обработки.

    Содержит все прочитанные из файла значения, пересчитывает их в необходимое количество точек.
*/
class GraphData
{    
public:
    GraphData():freqFilter(AS_IS_FREQ){}

    ///для хранения считанных данных
    std::vector<std::pair<double, double> >curve;

    ///преобразование данных
    bool convertData(const unsigned int x_display_size, const unsigned int y_display_size, std::vector<std::pair<int, int> > &result, AxisParam *param);
    ///преобразование считанных параметров в заголовок графика
    bool convertHeader(std::vector<QString>&);
    QString getFileName() const;
    void setFileName(const QString &);
    std::vector<QString>* getCurveHeader();

    void setFreqFilter(frequency_filter_typedef newFreqF);
    frequency_filter_typedef getFreqFilter();

private:    
    QString source_file_name;
    ///для хранения считанных параметров
    std::vector<QString> curve_header;
    ///для хранения типа обработки
    frequency_filter_typedef freqFilter;
};

#endif // GRAPHDATA_H
