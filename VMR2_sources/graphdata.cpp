#include "graphdata.h"
#include <QDebug>
#include <cmath> //abs()

/*!
 * \brief GraphData::setFileName
 * \param fn
 */
void GraphData::setFileName(const QString & fn)
{
    source_file_name = fn;
}

/*!
 * \brief GraphData::getFileName
 * \return
 */
QString GraphData::getFileName() const
{
    return source_file_name;
}

/*!
 * \brief GraphData::setFreqFilter
 * \return
 */
void GraphData::setFreqFilter(frequency_filter_typedef newFreqF)
{
    this->freqFilter = newFreqF;
}

/*!
 * \brief GraphData::getFreqFilter
 * \return
 */
frequency_filter_typedef GraphData::getFreqFilter(void)
{
    return this->freqFilter;
}

/*!
 * \brief GraphData::getCurveHeader
 * \return
 */
std::vector<QString>* GraphData::getCurveHeader()
{
    return &curve_header;
}

/*!
 * \brief GraphData::convertHeader
 * \param [in] plot_title вектор со строками параметров кривой, будут выбраны только четыре основных.
 * \return true, если успех
 */
bool GraphData::convertHeader(std::vector<QString>&plot_title)
{
    if (!curve_header.empty())
    {
        plot_title.push_back( *(curve_header.begin() + HEADER_COMPANY_NAME_OFFSET));
        plot_title.push_back( *(curve_header.begin() + HEADER_SOFT_VERSION_OFFSET));
        plot_title.push_back( *(curve_header.begin() + HEADER_MEASURMENT_TYPE));
        plot_title.push_back( *(curve_header.begin() + HEADER_MEASURMENT_TIME_BEGIN));

        return true;
    }
    return false;
}


/*!
 * \brief GraphData::convertData Основная функция обработки массива входных данных. По полученному количеству точек формирует матрицу для отрисовки.
 *
 * \param [in] x_display_size - число точек, которые удастся отобразить с заданными в plotter параметрами, ось X
 * \param [in] y_display_size - число точек, которые удастся отобразить с заданными в plotter параметрами, ось Y
 * \param [out] result - вектор с координатами центров точек в относительных единицах
 * \param [out] param - структура для хранения параметров
 * \return true, если успех
 */
bool GraphData::convertData(const unsigned int x_display_size, const unsigned int y_display_size, std::vector<std::pair<int, int> > &result, AxisParam *param)
{
    typedef enum {
        Y_MAX = 0,
        Y_MIN = 1,
        X_MAX = 2,
    } prev_dot_type;

    //histogramms инициализация
    std::vector<std::vector<int> > histograms_array(x_display_size, std::vector<int>(y_display_size));
    for (std::vector<std::vector<int> >::iterator it = histograms_array.begin(); it != histograms_array.end(); ++it)
    {
        for(std::vector<int>::iterator it2 = it->begin(); it2 != it->end(); ++it2)
            *it2 = 0;
    }

    int converted_dot_coordinate_x = 0;
    int converted_dot_coordinate_y = 0;
    double discret_interval_x = 0.0;
    double discret_interval_y = 0.0;

    //расчет параметров графика
    AxisParam ax_params;
    ax_params.calcValues(curve.begin(), curve.end());\
    if (ax_params.valuesDone())
    {
        discret_interval_x = ax_params.get_dd_x() / (x_display_size * 1.0);
        discret_interval_y = ax_params.get_dd_y() / (y_display_size * 1.0);

        *param = ax_params;
    }
    else
        return false;

    std::vector<int>::iterator temp_it;

    //заполнение значениями массива гистограмм
    for (std::vector<std::pair<double, double> >::iterator it = curve.begin(); it != curve.end(); ++it)
    {
        converted_dot_coordinate_x = round((it->first - ax_params.get_min_x())/discret_interval_x);
        if (converted_dot_coordinate_x == static_cast<int>(x_display_size))
        {
            converted_dot_coordinate_x -= 1;
        }

        converted_dot_coordinate_y = round((it->second - ax_params.get_min_y())/discret_interval_y);
        if (converted_dot_coordinate_y == static_cast<int>(y_display_size))
        {
            converted_dot_coordinate_y -= 1;
        }

        //qDebug() << "converted_dot_coordinate_x = " << converted_dot_coordinate_x << " raw_dot_coordinate_x = " << (it->first - ax_params.get_min_x())/discret_interval_x;
        //qDebug() << "converted_dot_coordinate_y = " << converted_dot_coordinate_y << "raw_dot_coordinate_y = " << (it->second - ax_params.get_min_y())/discret_interval_y;

        temp_it = (histograms_array.begin() + converted_dot_coordinate_x)->begin();
        *(temp_it + converted_dot_coordinate_y) += 1;
    }

    //обработка гистограмм и формирование результата
    int i = 0;    
    prev_dot_type last_dot_type = Y_MAX;

    for (std::vector<std::vector<int> >::iterator it = histograms_array.begin(); it != histograms_array.end(); ++it)
    {
        std::vector<int>::iterator min_max_finder_hist = std::max_element(it->begin(), it->end());
        std::vector<int>max_indexes;

        int k = 0;
        for (std::vector<int>::iterator itl = it->begin(); itl != it->end(); ++itl)
        {
            if (*itl != 0)
            {
                max_indexes.push_back(k);
            }
            k += 1;
        }

        if (!max_indexes.empty())
        {
            if (*min_max_finder_hist != 0)
            {
                if (this->freqFilter == HIGH_FREQ)
                {
                  if (last_dot_type == Y_MAX)
                  {
                      result.push_back(std::pair<int, int>(i, *(max_indexes.end() - 1)));
                      //qDebug() << "result_pb " << *(max_indexes.end() - 1);
                      last_dot_type = Y_MIN;
                  }
                  else if (last_dot_type == Y_MIN)
                  {
                      result.push_back(std::pair<int, int>(i, *(max_indexes.begin())));
                      //qDebug() << "result_pb " << *(max_indexes.begin());
                      last_dot_type = Y_MAX;
                  }
                }
                else if (this->freqFilter == LOW_FREQ) {
                   result.push_back(std::pair<int, int>(i, *(max_indexes.begin() + max_indexes.size()/2)));
                }
                else if (this->freqFilter == AS_IS_FREQ) {
                      if (last_dot_type == Y_MAX)
                      {
                          result.push_back(std::pair<int, int>(i, *(max_indexes.end() - 1)));
                          last_dot_type = X_MAX;
                      }
                      else if (last_dot_type == X_MAX) {
                          result.push_back(std::pair<int, int>(i, *(max_indexes.begin() + max_indexes.size()/2)));
                          last_dot_type = Y_MIN;
                      }
                      else if (last_dot_type == Y_MIN)
                      {
                          result.push_back(std::pair<int, int>(i, *(max_indexes.begin())));
                          last_dot_type = Y_MAX;
                      }
                 }
            }
        }

        i += 1;
    }

    qDebug() << "convertData comlete";

    if (!result.empty())
        return true;
    else
        return false;
}

