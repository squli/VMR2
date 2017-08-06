#ifndef GRAPHPLOTTER_H
#define GRAPHPLOTTER_H

#include <QWidget>
#include "graphdata.h"
#include <map>
#include <QPixmap>
#include "axisparam.h"

/*!
    \brief Класс отображения графика на экране.

    Получает обработанные данные из контейнера, параметры для построения
    надписей на осях и заголовка графика, затем рисует график на экране.
*/
class GraphPlotter : public QWidget
{
    Q_OBJECT
public:
    explicit GraphPlotter(QWidget *parent = 0);

    void refreshPixmap();
    void clearData();
    void savePNG(const QString&);
    void updateSizes();

    ///получить число точек в преобразованном графике
    unsigned int getXsize();
    unsigned int getYsize();

    /// параметры отображения
    AxisParam params;
    /// массив с точками для отображения с учетом преобразований
    std::vector<std::pair <int, int> > points_to_plot;
    /// заголовок графика
    std::vector<QString> plot_title;

protected:
    void paintEvent(QPaintEvent*);

private:
    /// отступы от края виджета до поля с координатной сеткой
    static const unsigned int margin_px_X = 25;
    static const unsigned int margin_px_Y = 50;

    /// тоже, только со стороны подписей к осям графика
    static const unsigned int margin_legend_px_X = 50;
    static const unsigned int margin_legend_px_Y = 30;

    ///размер области графика в пикселях
    unsigned int X_size;
    unsigned int Y_size;

    ///размер области под одну точку в пикселах
    unsigned int dot_X_size;
    unsigned int dot_Y_size;

    ///число вспомогательных линий на графике
    unsigned int steps_X;
    unsigned int steps_Y;

    void plotCurves(QPainter *painter);
    void plotGrids(QPainter *painter);
    void plotLegend(QPainter *painter);
    void plotTitle(QPainter *painter);

    /// сцена на которой выполняются построения
    QPixmap pixmap;

signals:

public slots:

};

#endif // GRAPHPLOTTER_H
