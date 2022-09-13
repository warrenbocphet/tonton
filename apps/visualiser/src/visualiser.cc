#include "visualiser.h"

Visualiser::Visualiser() : axis_(matplot::gca())
{
    axis_->x_axis().limits({-5000, 5000});
    axis_->y_axis().limits({-5000, 5000});
};

void Visualiser::draw(
    const std::vector<double>& x, const std::vector<double>& y
)
{
    matplot::scatter(x, y, 1);
}