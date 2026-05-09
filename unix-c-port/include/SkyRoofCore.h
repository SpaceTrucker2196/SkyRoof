#ifndef SKYROOF_CORE_H
#define SKYROOF_CORE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum SkyRoofStatus
{
  SkyRoofStatusOk = 0,
  SkyRoofStatusInvalidArgument = 1,
  SkyRoofStatusOverflow = 2,
  SkyRoofStatusUnderflow = 3,
  SkyRoofStatusInternalError = 4
} SkyRoofStatus;

typedef enum SkyRoofLogLevel
{
  SkyRoofLogLevelDebug = 0,
  SkyRoofLogLevelInfo = 1,
  SkyRoofLogLevelWarning = 2,
  SkyRoofLogLevelError = 3
} SkyRoofLogLevel;

typedef void (*SkyRoofLogCallback)(SkyRoofLogLevel logLevel, const char* message, void* context);

void SkyRoofLogSetCallback(SkyRoofLogCallback callback, void* context);
void SkyRoofLogMessage(SkyRoofLogLevel logLevel, const char* message);
const char* SkyRoofVersion(void);

typedef struct SkyRoofSampleBuffer
{
  float* data;
  size_t capacity;
  size_t readIndex;
  size_t writeIndex;
  size_t size;
} SkyRoofSampleBuffer;

SkyRoofStatus SkyRoofSampleBufferInit(SkyRoofSampleBuffer* sampleBuffer, size_t capacity);
void SkyRoofSampleBufferFree(SkyRoofSampleBuffer* sampleBuffer);
SkyRoofStatus SkyRoofSampleBufferWrite(SkyRoofSampleBuffer* sampleBuffer, const float* input, size_t count);
SkyRoofStatus SkyRoofSampleBufferRead(SkyRoofSampleBuffer* sampleBuffer, float* output, size_t count);
size_t SkyRoofSampleBufferSize(const SkyRoofSampleBuffer* sampleBuffer);

typedef struct SkyRoofCatState
{
  unsigned long long rxFrequencyHz;
  unsigned long long txFrequencyHz;
} SkyRoofCatState;

SkyRoofStatus SkyRoofCatParseCommand(const char* command, SkyRoofCatState* catState);
SkyRoofStatus SkyRoofCatFormatFrequency(const SkyRoofCatState* catState, char* output, size_t outputLength);

double SkyRoofDopplerShiftHz(double baseFrequencyHz, double radialVelocityMetersPerSecond);

#ifdef __cplusplus
}
#endif

#endif
