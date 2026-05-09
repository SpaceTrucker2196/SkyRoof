#include "SkyRoofCore.h"

#include <math.h>

int main(void)
{
  const double baseHz = 145950000.0;
  const double radialVelocity = 7600.0;
  const double expectedShift = 3698.992741;

  double actualShift = SkyRoofDopplerShiftHz(baseHz, radialVelocity);
  if(fabs(actualShift - expectedShift) > 0.5)
    return 1;

  if(SkyRoofDopplerShiftHz(0.0, radialVelocity) != 0.0)
    return 1;

  return 0;
}
