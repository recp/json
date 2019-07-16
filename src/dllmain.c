/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#if defined(_WIN32) || defined(WIN32)

/* Exclude rarely-used stuff from Windows headers */
#  define WIN32_LEAN_AND_MEAN 
#  include <SDKDDKVer.h>

/* Windows Header Files: */
#  include <windows.h>

#endif

BOOL APIENTRY
DllMain(HMODULE hModule,
        DWORD   ul_reason_for_call,
        LPVOID  lpReserved)
{
  switch (ul_reason_for_call)
  {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
      break;
  }
  return TRUE;
}
