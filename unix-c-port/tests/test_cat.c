#include "SkyRoofCore.h"

#include <string.h>

int main(void)
{
  SkyRoofCatState catState = {0};

  if(SkyRoofCatParseCommand("F 145950000", &catState) != SkyRoofStatusOk)
    return 1;

  char output[32] = {0};
  if(SkyRoofCatFormatFrequency(&catState, output, sizeof(output)) != SkyRoofStatusOk)
    return 1;

  if(strcmp(output, "145950000") != 0)
    return 1;

  if(SkyRoofCatParseCommand("Q", &catState) != SkyRoofStatusInvalidArgument)
    return 1;

  return 0;
}
