/*
 * Copyright (C) 2021- TeraTerm Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>

#include "inifile_com_local.h"

static wchar_t *AcpToWideDup(const char *src) {
  if (src == NULL) {
    return NULL;
  }

  int lenW = MultiByteToWideChar(CP_ACP, 0, src, -1, NULL, 0);
  if (lenW <= 0) {
    return NULL;
  }

  wchar_t *dst = (wchar_t *)malloc((size_t)lenW * sizeof(wchar_t));
  if (dst == NULL) {
    return NULL;
  }

  if (MultiByteToWideChar(CP_ACP, 0, src, -1, dst, lenW) == 0) {
    free(dst);
    return NULL;
  }

  return dst;
}

DWORD GetPrivateProfileStringAFileW(const char *appA, const char *keyA,
                                    const char *defA, char *strA, DWORD size,
                                    const wchar_t *filenameW) {
  wchar_t *appW = AcpToWideDup(appA);
  wchar_t *keyW = AcpToWideDup(keyA);
  wchar_t *defW = AcpToWideDup(defA);
  DWORD lenA = 0;

  if ((appW == NULL && appA != NULL) || (keyW == NULL && keyA != NULL) ||
      (defW == NULL && defA != NULL)) {
    goto cleanup;
  }

  wchar_t *bufW = (wchar_t *)malloc((size_t)size * sizeof(wchar_t));
  if (bufW == NULL) {
    goto cleanup;
  }

  DWORD lenW = GetPrivateProfileStringW(appW, keyW, defW, bufW, size, filenameW);
  if (size > 0) {
    int written =
        WideCharToMultiByte(CP_ACP, 0, bufW, -1, strA, (int)size, NULL, NULL);
    if (written > 0) {
      lenA = (DWORD)(written - 1);
    } else {
      strA[0] = '\0';
      lenA = 0;
    }
  }

  if (lenW == 0 && size > 0) {
    strA[0] = '\0';
  }

  free(bufW);

cleanup:
  free(appW);
  free(keyW);
  free(defW);
  return lenA;
}

BOOL WritePrivateProfileStringAFileW(const char *appA, const char *keyA,
                                     const char *strA,
                                     const wchar_t *filenameW) {
  wchar_t *appW = AcpToWideDup(appA);
  wchar_t *keyW = AcpToWideDup(keyA);
  wchar_t *strW = AcpToWideDup(strA);
  BOOL result = FALSE;

  if ((appW == NULL && appA != NULL) || (keyW == NULL && keyA != NULL) ||
      (strW == NULL && strA != NULL)) {
    goto cleanup;
  }

  result = WritePrivateProfileStringW(appW, keyW, strW, filenameW);

cleanup:
  free(appW);
  free(keyW);
  free(strW);
  return result;
}
