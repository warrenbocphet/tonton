#include <matplot/matplot.h>

#include "visualiser.h"

Visualiser::Visualiser() {};

void Visualiser::draw(const std::vector<double>& x, const std::vector<double>& y)
{
    matplot::scatter(x, y);
}