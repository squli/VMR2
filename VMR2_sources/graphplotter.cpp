#include "graphplotter.h"
#include <QtGui>
#include <QColor>
#include <cstdlib>

GraphPlotter::GraphPlotter(QWidget *parent) : QWidget(parent), dot_X_size(1), dot_Y_size(1), steps_X(5), steps_Y(5)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    updateSizes();
}

void GraphPlotter::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, pixmap);

    updateSizes();
}

/*!
 * \brief GraphPlotter::updateSizes Обновляет размеры виджета в пикселях
 */
void GraphPlotter::updateSizes()
{
    X_size = width() - margin_legend_px_X - margin_px_X;
    Y_size = height() - margin_legend_px_Y - margin_px_Y;

    qDebug() << "X_size = " << X_size << width();
    qDebug() << "Y_size = " << Y_size << height();
}

/*!
 * \brief GraphPlotter::getXsize
 * \return count of dots to plot by x
 */
unsigned int GraphPlotter::getXsize()
{
    return round(X_size/(dot_X_size));
}

/*!
 * \brief GraphPlotter::getYsize
 * \return count of dots to plot by y
 */
unsigned int GraphPlotter::getYsize()
{
    return round(Y_size/(dot_Y_size));
}

/*!
 * \brief GraphPlotter::clearData Очистка данных от текущего графика
 */
void GraphPlotter::clearData(void)
{
    points_to_plot.erase(points_to_plot.begin(), points_to_plot.end());
    params.clear();
    plot_title.erase(plot_title.begin(), plot_title.end());
}

/*!
 * \brief GraphPlotter::refreshPixmap Обновляет изображение на pixmap
 */
void GraphPlotter::refreshPixmap(void)
{
    pixmap = QPixmap(size());
    pixmap.fill((QColor("white")));

    QPainter painter(&pixmap);
    painter.initFrom(this);

    //отрисовка координатной сетки и данных
    plotGrids(&painter);
    plotLegend(&painter);
    plotTitle(&painter);
    plotCurves(&painter);

    //обновление виджета в приложении
    update();
}

/*!
 * \brief GraphPlotter::plotCurves Строит непосредственно кривую с точками
 *        по
 * \param painter
 */
void GraphPlotter::plotCurves(QPainter *painter)
{
    QPen pen( Qt::red );
    pen.setWidth( dot_X_size );
    painter->setPen(pen);

    for(std::vector<std::pair<int, int> >::iterator it = points_to_plot.begin(); it != points_to_plot.end() - 1; ++it)
    {
        int x1 = it->first * dot_X_size + margin_legend_px_X;
        int y1 = Y_size - it->second * (dot_Y_size) + margin_px_Y;

        int x2 = (it + 1)->first * dot_X_size + margin_legend_px_X;
        int y2 = Y_size - (it + 1)->second * (dot_Y_size) + margin_px_Y;

        //qDebug() << "(x1, y1) " << x1 << " " << y1;
        //qDebug() << "(x2, y2) " << x2 << " " << y2;

        if ( abs(x2 - x1) == 1)
        {
            if (y1 >= y2)
            {
                painter->drawLine(x1, y1, x1, y1 - (abs(y1 - y2)/2) );
                painter->drawLine(x2, y1 - (abs(y1 - y2)/2), x2, y2 );
            }
            else
            {
                painter->drawLine(x1, y1, x1, y1 + (abs(y1 - y2)/2) );
                painter->drawLine(x2, y1 + (abs(y1 - y2)/2), x2, y2 );
            }
            //qDebug() << "Custom plot";
        }
        else
        {
            painter->drawLine(x1, y1, x2, y2);
            //qDebug() << "Default plot";
        }
    }
}

/*!
 * \brief GraphPlotter::plotGrids рисует сетку
 * \param painter
 */
void GraphPlotter::plotGrids(QPainter *painter)
{
    painter->setPen(QColor("gray"));

    //внешняя рамка
    painter->drawRect(margin_legend_px_X, margin_px_Y, X_size, Y_size);

    //внутренние линии
    for (unsigned int i = 1; i < steps_X; ++i)
    {
        int x = margin_legend_px_X + i * X_size/steps_X;
        painter->drawLine(x, Y_size + margin_px_Y, x, margin_px_Y);
    }
    for (unsigned int i = 1; i < steps_Y; ++i)
    {
        int y = margin_px_Y + i * Y_size/steps_Y;
        painter->drawLine(margin_legend_px_X, y, X_size + margin_legend_px_X, y);
    }
}

/*!
 * \brief GraphPlotter::plotLegend рисует легенду графика из params
 * \param painter
 */
void GraphPlotter::plotLegend(QPainter *painter)
{
     painter->setPen(QColor("gray"));
     QFontMetrics fm = painter->fontMetrics();

    ///подписи к осям
    std::vector<QString> x_legend;
    std::vector<QString> y_legend;
    unsigned int count_chrs_in_dgt_x = 1;
    unsigned int count_chrs_in_dgt_y = 2;

    //подписи по оси X
    x_legend.push_back(QString("%1").arg(this->params.get_min_x(), 0, 'E', count_chrs_in_dgt_x));
    for (size_t i = 1; i < steps_X; ++i)
    {
        x_legend.push_back(QString("%1").arg(i * this->params.get_dd_x()/steps_X, 0, 'E', count_chrs_in_dgt_x));
    }
    x_legend.push_back(QString("%1").arg(this->params.get_max_x(), 0, 'E', count_chrs_in_dgt_x));

    //подписи по оси Y
    y_legend.push_back(QString("%1").arg(params.get_min_y(), 0, 'E', count_chrs_in_dgt_y));
    for (size_t i = 1; i < steps_Y; ++i)
    {
        y_legend.push_back(QString("%1").arg(i * params.get_dd_y()/steps_Y + params.get_min_y(), 0, 'E', count_chrs_in_dgt_y));
    }
    y_legend.push_back(QString("%1").arg(params.get_max_y(), 0, 'E', count_chrs_in_dgt_y));

    //отображение на оси X
    for (unsigned int i = 0; i < steps_X + 1; ++i)
    {
        int x = margin_legend_px_X + i * X_size/steps_X - fm.width(x_legend.at(i)) / 2;
        painter->drawText(x, margin_px_Y + Y_size + fm.height(), x_legend.at(i));
    }

    //отображение на оси Y
    for (unsigned int i = 0; i < steps_Y + 1; ++i)
    {
        int y = margin_px_Y + i * Y_size/steps_Y + fm.height()/2;
        if (i == steps_Y)
            y = margin_px_Y + i * Y_size/steps_Y;

        painter->drawText(margin_legend_px_X - fm.width(y_legend.at(steps_Y - i)) - 1, y, y_legend.at(steps_Y - i));
    }
}

/*!
 * \brief GraphPlotter::plotTitle Отображает загловок графика как все содержимое plot_title
 * \param painter
 */
void GraphPlotter::plotTitle(QPainter *painter)
{
    painter->setPen(QColor("black"));
    QFontMetrics fm = painter->fontMetrics();

    int offset = 0;
    int space_btwn_sntns_px = 10;
    for (std::vector<QString>::iterator it = plot_title.begin(); it != plot_title.end(); ++it)
    {
        painter->drawText(2 * margin_legend_px_X + offset, margin_px_Y/2, *it);
        offset += (fm.width(*it) + space_btwn_sntns_px);
    }
}

/*!
 * \brief GraphPlotter::savePNG сохранение текущей картинки в png файл
 * \param filename имя файла
 */
void GraphPlotter::savePNG(const QString &filename)
{
    QFile file(filename + ".png");
    file.open(QIODevice::WriteOnly);
    pixmap.save(&file, "PNG");
}
