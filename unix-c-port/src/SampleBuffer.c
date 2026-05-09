#include "SkyRoofCore.h"

#include <stdlib.h>
#include <string.h>

SkyRoofStatus SkyRoofSampleBufferInit(SkyRoofSampleBuffer* sampleBuffer, size_t capacity)
{
  if(sampleBuffer == NULL || capacity == 0)
    return SkyRoofStatusInvalidArgument;

  sampleBuffer->data = (float*)calloc(capacity, sizeof(float));
  if(sampleBuffer->data == NULL)
    return SkyRoofStatusInternalError;

  sampleBuffer->capacity = capacity;
  sampleBuffer->readIndex = 0;
  sampleBuffer->writeIndex = 0;
  sampleBuffer->size = 0;
  return SkyRoofStatusOk;
}

void SkyRoofSampleBufferFree(SkyRoofSampleBuffer* sampleBuffer)
{
  if(sampleBuffer == NULL)
    return;

  free(sampleBuffer->data);
  sampleBuffer->data = NULL;
  sampleBuffer->capacity = 0;
  sampleBuffer->readIndex = 0;
  sampleBuffer->writeIndex = 0;
  sampleBuffer->size = 0;
}

SkyRoofStatus SkyRoofSampleBufferWrite(SkyRoofSampleBuffer* sampleBuffer, const float* input, size_t count)
{
  if(sampleBuffer == NULL || input == NULL)
    return SkyRoofStatusInvalidArgument;

  if(count > sampleBuffer->capacity - sampleBuffer->size)
    return SkyRoofStatusOverflow;

  for(size_t i = 0; i < count; ++i) {
    sampleBuffer->data[sampleBuffer->writeIndex] = input[i];
    sampleBuffer->writeIndex = (sampleBuffer->writeIndex + 1) % sampleBuffer->capacity;
  }

  sampleBuffer->size += count;
  return SkyRoofStatusOk;
}

SkyRoofStatus SkyRoofSampleBufferRead(SkyRoofSampleBuffer* sampleBuffer, float* output, size_t count)
{
  if(sampleBuffer == NULL || output == NULL)
    return SkyRoofStatusInvalidArgument;

  if(count > sampleBuffer->size)
    return SkyRoofStatusUnderflow;

  for(size_t i = 0; i < count; ++i) {
    output[i] = sampleBuffer->data[sampleBuffer->readIndex];
    sampleBuffer->readIndex = (sampleBuffer->readIndex + 1) % sampleBuffer->capacity;
  }

  sampleBuffer->size -= count;
  return SkyRoofStatusOk;
}

size_t SkyRoofSampleBufferSize(const SkyRoofSampleBuffer* sampleBuffer)
{
  if(sampleBuffer == NULL)
    return 0;

  return sampleBuffer->size;
}
