#include "SkyRoofCore.h"

#include <stdio.h>
#include <threads.h>

static SkyRoofLogCallback globalLogCallback;
static void* globalLogContext;
static once_flag logMutexInitFlag = ONCE_FLAG_INIT;
static mtx_t logMutex;

static void InitializeLogMutex(void)
{
  mtx_init(&logMutex, mtx_plain);
}

static void LockLogState(void)
{
  call_once(&logMutexInitFlag, InitializeLogMutex);
  mtx_lock(&logMutex);
}

static void UnlockLogState(void)
{
  mtx_unlock(&logMutex);
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
