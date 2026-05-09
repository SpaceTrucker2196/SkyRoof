#include "SkyRoofCore.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static SkyRoofStatus ParseFrequency(const char* value, unsigned long long* frequencyHz)
{
  if(value == NULL || frequencyHz == NULL)
    return SkyRoofStatusInvalidArgument;

  char* parseEnd = NULL;
  unsigned long long parsed = strtoull(value, &parseEnd, 10);
  if(parseEnd == value || (parseEnd != NULL && *parseEnd != '\0'))
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

  while(*command == ' ' || *command == '\t')
    command++;
  if(*command == '\0')
    return SkyRoofStatusInvalidArgument;

  const char* space = command;
  while(*space != '\0' && *space != ' ' && *space != '\t')
    space++;

  size_t opcodeLength = (size_t)(space - command);
  if(opcodeLength == 0 || opcodeLength > 3)
    return SkyRoofStatusInvalidArgument;

  char opcode[4] = {0};
  memcpy(opcode, command, opcodeLength);

  while(*space == ' ' || *space == '\t')
    space++;

  if(strcmp(opcode, "F") == 0) {
    if(*space == '\0')
      return SkyRoofStatusInvalidArgument;

    if(strlen(space) > 31)
      return SkyRoofStatusInvalidArgument;

    SkyRoofStatus status = ParseFrequency(space, &catState->rxFrequencyHz);
    if(status != SkyRoofStatusOk)
      return status;

    catState->txFrequencyHz = catState->rxFrequencyHz;
    return SkyRoofStatusOk;
  }

  if(strcmp(opcode, "f") == 0) {
    if(*space != '\0')
      return SkyRoofStatusInvalidArgument;

    return catState->rxFrequencyHz == 0 ? SkyRoofStatusUnderflow : SkyRoofStatusOk;
  }

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
