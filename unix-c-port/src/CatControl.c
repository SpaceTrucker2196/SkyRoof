#include "SkyRoofCore.h"

#include <stdio.h>
#include <string.h>

static SkyRoofStatus ParseFrequency(const char* value, unsigned long long* frequencyHz)
{
  if(value == NULL || frequencyHz == NULL)
    return SkyRoofStatusInvalidArgument;

  unsigned long long parsed = 0;
  if(sscanf(value, "%llu", &parsed) != 1)
    return SkyRoofStatusInvalidArgument;

  if(parsed == 0)
    return SkyRoofStatusInvalidArgument;

  *frequencyHz = parsed;
  return SkyRoofStatusOk;
}

SkyRoofStatus SkyRoofCatParseCommand(const char* command, SkyRoofCatState* catState)
{
  if(command == NULL || catState == NULL)
    return SkyRoofStatusInvalidArgument;

  char opcode[4] = {0};
  char value[32] = {0};
  if(sscanf(command, "%3s %31s", opcode, value) < 1)
    return SkyRoofStatusInvalidArgument;

  if(strcmp(opcode, "F") == 0)
  {
    SkyRoofStatus status = ParseFrequency(value, &catState->rxFrequencyHz);
    if(status != SkyRoofStatusOk)
      return status;

    catState->txFrequencyHz = catState->rxFrequencyHz;
    return SkyRoofStatusOk;
  }

  if(strcmp(opcode, "f") == 0)
    return catState->rxFrequencyHz == 0 ? SkyRoofStatusUnderflow : SkyRoofStatusOk;

  return SkyRoofStatusInvalidArgument;
}

SkyRoofStatus SkyRoofCatFormatFrequency(const SkyRoofCatState* catState, char* output, size_t outputLength)
{
  if(catState == NULL || output == NULL || outputLength < 4)
    return SkyRoofStatusInvalidArgument;

  if(catState->rxFrequencyHz == 0)
    return SkyRoofStatusUnderflow;

  int written = snprintf(output, outputLength, "%llu", catState->rxFrequencyHz);
  if(written < 0 || (size_t)written >= outputLength)
    return SkyRoofStatusOverflow;

  return SkyRoofStatusOk;
}
