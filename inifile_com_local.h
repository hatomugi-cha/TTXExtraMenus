#pragma once

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

DWORD GetPrivateProfileStringAFileW(const char *appA, const char *keyA,
                                    const char *defA, char *strA, DWORD size,
                                    const wchar_t *filenameW);
BOOL WritePrivateProfileStringAFileW(const char *appA, const char *keyA,
                                     const char *strA,
                                     const wchar_t *filenameW);

#ifdef __cplusplus
}
#endif
