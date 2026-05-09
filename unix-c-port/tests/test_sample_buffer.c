#include "SkyRoofCore.h"

#include <stdio.h>

int main(void)
{
  SkyRoofSampleBuffer sampleBuffer;
  if(SkyRoofSampleBufferInit(&sampleBuffer, 4) != SkyRoofStatusOk)
    return 1;

  float input[] = {1.0f, 2.0f, 3.0f};
  if(SkyRoofSampleBufferWrite(&sampleBuffer, input, 3) != SkyRoofStatusOk)
    return 1;

  if(SkyRoofSampleBufferSize(&sampleBuffer) != 3)
    return 1;

  float output[] = {0, 0};
  if(SkyRoofSampleBufferRead(&sampleBuffer, output, 2) != SkyRoofStatusOk)
    return 1;

  if(output[0] != 1.0f || output[1] != 2.0f)
    return 1;

  if(SkyRoofSampleBufferSize(&sampleBuffer) != 1)
    return 1;

  SkyRoofSampleBufferFree(&sampleBuffer);
  return 0;
}
