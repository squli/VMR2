#ifndef AXISPARAM_H
#define AXISPARAM_H

#include <vector>

/*!
    \brief Класс реализует работо по поиску минимумов, максимумов и т.д. для осей графика

    Рассчитывает значения и индексы минимумов, максимумов и динамического диапазона по обеим осям.
*/
class AxisParam
{
public:
    AxisParam(): valuesDoneFlag(false) {}

    void calcValues(std::vector<std::pair<double, double> >::iterator begin, std::vector<std::pair<double, double> >::iterator end);
    bool valuesDone(void);

    double get_min_x() const;
    double get_max_x() const;

    double get_min_y() const;
    double get_max_y() const;

    double get_dd_x() const;
    double get_dd_y() const;

    void clear();

private:
    bool valuesDoneFlag;

    double min_val_x;
    double max_val_x;

    double min_val_y;
    double max_val_y;

    double min_indx_x;
    double max_indx_x;

    double min_indx_y;
    double max_indx_y;

    double dynam_dia_x;
    double dynam_dia_y;
};

#endif // AXISPARAM_H
