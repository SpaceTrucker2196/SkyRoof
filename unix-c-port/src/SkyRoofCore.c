#include "SkyRoofCore.h"

#include <stdio.h>
#include <stdatomic.h>

static SkyRoofLogCallback globalLogCallback;
static void* globalLogContext;
static atomic_flag logLock = ATOMIC_FLAG_INIT;

static void LockLogState(void)
{
  while(atomic_flag_test_and_set(&logLock))
    ;
}

static void UnlockLogState(void)
{
  atomic_flag_clear(&logLock);
}

void SkyRoofLogSetCallback(SkyRoofLogCallback callback, void* context)
{
  LockLogState();
  globalLogCallback = callback;
  globalLogContext = context;
  UnlockLogState();
}

void SkyRoofLogMessage(SkyRoofLogLevel logLevel, const char* message)
{
  if(message == NULL)
    return;

  LockLogState();
  SkyRoofLogCallback callback = globalLogCallback;
  void* callbackContext = globalLogContext;
  UnlockLogState();

  if(callback != NULL) {
    callback(logLevel, message, callbackContext);
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
