#include "SkyRoofCore.h"

static const double speedOfLightMetersPerSecond = 299792458.0;

double SkyRoofDopplerShiftHz(double baseFrequencyHz, double radialVelocityMetersPerSecond)
{
  if(baseFrequencyHz <= 0.0)
    return 0.0;

  return baseFrequencyHz * radialVelocityMetersPerSecond / speedOfLightMetersPerSecond;
}
