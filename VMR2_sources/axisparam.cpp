#include "axisparam.h"
#include <algorithm>


static bool pairs_compare_x(const std::pair<double, double> &lhs, const std::pair<double, double> &rhs)
{
    return (lhs.first < rhs.first);
}

static bool pairs_compare_y(const std::pair<double, double> &lhs, const std::pair<double, double> &rhs)
{
    return (lhs.second < rhs.second);
}

void AxisParam::clear()
{
    max_val_x = 0;
    max_indx_x = 0;
    min_val_x = 0;
    min_indx_x = 0;
    max_val_y = 0;
    max_indx_y = 0;
    min_val_y = 0;
    min_indx_y = 0;
    dynam_dia_y = 0;
    valuesDoneFlag = false;
}

void AxisParam::calcValues(std::vector<std::pair<double, double> >::iterator begin, std::vector<std::pair<double, double> >::iterator end)
{
    std::vector<std::pair<double, double> >::iterator min_max_finder;

    min_max_finder = std::max_element(begin, end, pairs_compare_x);
    max_val_x = min_max_finder->first;
    max_indx_x = static_cast<int>(end - min_max_finder);

    min_max_finder = std::min_element(begin, end, pairs_compare_x);
    min_val_x = min_max_finder->first;
    min_indx_x = static_cast<int>(end - min_max_finder);

    dynam_dia_x = (max_val_x - min_val_x);

    min_max_finder = std::max_element(begin, end, pairs_compare_y);
    max_val_y = min_max_finder->second;
    max_indx_y = static_cast<int>(end - min_max_finder);

    min_max_finder = std::min_element(begin, end, pairs_compare_y);
    min_val_y = min_max_finder->second;
    min_indx_y = static_cast<int>(end - min_max_finder);

    dynam_dia_y = (max_val_y - min_val_y);

    valuesDoneFlag = true;
}

bool AxisParam::valuesDone(void)
{
    return valuesDoneFlag;
}

double AxisParam::get_min_x() const
{
    return min_val_x;
}

double AxisParam::get_max_x() const
{
    return max_val_x;
}

double AxisParam::get_min_y() const
{
    return min_val_y;
}

double AxisParam::get_max_y() const
{
    return max_val_y;
}

double AxisParam::get_dd_x() const
{
    return dynam_dia_x;
}

double AxisParam::get_dd_y() const
{
    return dynam_dia_y;
}

