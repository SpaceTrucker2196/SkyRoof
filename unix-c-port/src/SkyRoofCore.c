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

  if(callback != NULL)
    callback(logLevel, message, callbackContext);
  if(callback != NULL)
    return;

  const char* levelName = "INFO";
  switch(logLevel)
  {
    case SkyRoofLogLevelDebug:
      levelName = "DEBUG";
      break;
    case SkyRoofLogLevelInfo:
      levelName = "INFO";
      break;
    case SkyRoofLogLevelWarning:
      levelName = "WARN";
      break;
    case SkyRoofLogLevelError:
      levelName = "ERROR";
      break;
    default:
      levelName = "INFO";
      break;
  }

  fprintf(stderr, "[%s] %s\n", levelName, message);
}

const char* SkyRoofVersion(void)
{
  return "0.1.0-mvp";
}
