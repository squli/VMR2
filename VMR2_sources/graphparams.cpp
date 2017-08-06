#include "graphparams.h"

GraphParams::GraphParams(): steps_X(5), steps_Y(5), minX(0)
{

}

/*
void GraphParams::setGraphParams(const GraphData &data)
{
    if (data.curve.size() > 2)
    {     
        maxX = int(data.curve.at(data.curve.size() - 1).first) + 1;
    }
    else if (data.curve.size() == 1)
    {        
        maxX = int(data.curve.at(0).first + 0.5 * data.curve.at(0).first);
    }

}
*/
