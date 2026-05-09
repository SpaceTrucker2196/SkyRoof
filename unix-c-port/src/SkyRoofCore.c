#include "SkyRoofCore.h"

#include <stdio.h>

static SkyRoofLogCallback globalLogCallback;
static void* globalLogContext;

void SkyRoofLogSetCallback(SkyRoofLogCallback callback, void* context)
{
  globalLogCallback = callback;
  globalLogContext = context;
}

void SkyRoofLogMessage(SkyRoofLogLevel logLevel, const char* message)
{
  if(message == NULL)
    return;

  if(globalLogCallback != NULL) {
    globalLogCallback(logLevel, message, globalLogContext);
    return;
  }

  const char* levelName = "INFO";
  if(logLevel == SkyRoofLogLevelDebug)
    levelName = "DEBUG";
  else if(logLevel == SkyRoofLogLevelWarning)
    levelName = "WARN";
  else if(logLevel == SkyRoofLogLevelError)
    levelName = "ERROR";

  fprintf(stderr, "[%s] %s\n", levelName, message);
}

const char* SkyRoofVersion(void)
{
  return "0.1.0-mvp";
}
