#include <vector>
#include <matplot/matplot.h>
class Visualiser
{
  public:
    Visualiser();

    void draw(const std::vector<double>& x, const std::vector<double>& y);
  
  private:
    matplot::axes_handle axis_;
};