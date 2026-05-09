#include "SkyRoofCore.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int ParseDouble(const char* input, double* value)
{
  if(input == NULL || value == NULL)
    return 0;

  char* parseEnd = NULL;
  double parsed = strtod(input, &parseEnd);
  if(parseEnd == input || (parseEnd != NULL && *parseEnd != '\0'))
    return 0;

  *value = parsed;
  return 1;
}

static int RunDoppler(int argc, char** argv)
{
  if(argc < 4) {
    fprintf(stderr, "usage: skyroof_cli doppler <base_hz> <radial_velocity_mps>\n");
    return 1;
  }

  double baseHz = 0.0;
  double radialVelocity = 0.0;
  if(!ParseDouble(argv[2], &baseHz) || !ParseDouble(argv[3], &radialVelocity)) {
    fprintf(stderr, "base_hz and radial_velocity_mps must be numeric values\n");
    return 1;
  }

  double shift = SkyRoofDopplerShiftHz(baseHz, radialVelocity);
  printf("%.6f\n", shift);
  return 0;
}

static int RunCatParse(int argc, char** argv)
{
  if(argc < 3) {
    fprintf(stderr, "usage: skyroof_cli cat-parse \"F 145950000\"\n");
    return 1;
  }

  SkyRoofCatState catState = {0};
  SkyRoofStatus status = SkyRoofCatParseCommand(argv[2], &catState);
  if(status != SkyRoofStatusOk) {
    fprintf(stderr, "failed to parse CAT command\n");
    return 1;
  }

  char output[32] = {0};
  status = SkyRoofCatFormatFrequency(&catState, output, sizeof(output));
  if(status != SkyRoofStatusOk) {
    fprintf(stderr, "frequency not available\n");
    return 1;
  }

  printf("%s\n", output);
  return 0;
}

int main(int argc, char** argv)
{
  if(argc < 2) {
    fprintf(stderr, "usage: skyroof_cli <doppler|cat-parse|version> ...\n");
    return 1;
  }

  if(strcmp(argv[1], "doppler") == 0)
    return RunDoppler(argc, argv);

  if(strcmp(argv[1], "cat-parse") == 0)
    return RunCatParse(argc, argv);

  if(strcmp(argv[1], "version") == 0) {
    printf("%s\n", SkyRoofVersion());
    return 0;
  }

  fprintf(stderr, "unknown command: %s\n", argv[1]);
  return 1;
}
