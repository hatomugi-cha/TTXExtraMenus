#include "teraterm.h"
#include "tttypes.h"
#include "ttplugin.h"
#include "tt_res.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "inifile_com_local.h"

#define ORDER 5800
#define ID_MENU_CONNECT 55200

#define INISECTION "ExtraMenus"
#define MENU_INI_SIZE 200
#define DEFAULT_MENU_INI "Serial:50350, Connect:55200, Clear:50260"

static HANDLE hInst; /* Instance handle of TTX*.DLL */

typedef struct {
	PTTSet ts;
	PComVar cv;
	PReadIniFile origReadIniFile;
	char menu_ini[MENU_INI_SIZE];
	BOOL enabled;
} TInstVar;

static TInstVar *pvar;
static TInstVar InstVar;

static void PASCAL TTXInit(PTTSet ts, PComVar cv)
{
	pvar->ts = ts;
	pvar->cv = cv;
	pvar->origReadIniFile = NULL;
	pvar->menu_ini[0] = 0;
	pvar->enabled = TRUE;
}

static void PASCAL TTXModifyMenu(HMENU menu)
{
	if (pvar->enabled == FALSE)
		return;

	char menu_str[MENU_INI_SIZE+1];
	strncpy_s(menu_str, sizeof(menu_str), pvar->menu_ini, _TRUNCATE);

	char *p = menu_str;
	char *end = menu_str + strlen(menu_str);
	char *q;
	char *menu_cap;
	UINT menu_id;

	while (p < end) {
		// Search Menu Caption
		for (; p < end && isspace((unsigned char)*p); p++);
		if (p >= end)
			break;

		q = strchr(p, ':');
		if (q == NULL)
			break;
		menu_cap = p;
		*q = 0;

		// Skip ':'
		p = q + 1;

		// Search Menu ID
		q = strchr(p, ',');
		if (q == NULL) {
			q = end;
		}
		*q = 0;

		char *id_end;
		menu_id = (UINT)strtoul(p, &id_end, 10);
		while (isspace((unsigned char)*id_end))
			id_end++;
		if (id_end == p || *id_end != '\0' || menu_id == 0 || menu_id > 0xFFFF) {
			p = q + 1;
			continue;
		}

		AppendMenu(menu, MF_ENABLED, menu_id, menu_cap);
		p = q + 1;
	}
}

static void OnConnect(HWND hWin)
{
	// This function performs pseudo disconnect with an actual Com device connected.
	// Instead of using CheckComPort(), search "[disconnected]" in the window title
	// to determine the current state.

	char title[100];
	int ret = GetWindowText(hWin, title, 100);
	if (ret == 0)
		return;

	if (strstr(title, "[disconnected]") == 0) {
		SendMessage(hWin, WM_COMMAND, MAKELONG(ID_FILE_DISCONNECT, 0), 0);
	}
	else{
		BOOL orgWaitCom = pvar->ts->WaitCom;
		pvar->ts->WaitCom = TRUE;
		SendMessage(hWin, WM_USER_COMMSTART, 0, 0);
		pvar->ts->WaitCom = orgWaitCom;
	}
}

static int PASCAL TTXProcessCommand(HWND hWin, WORD cmd)
{
	if (cmd == ID_MENU_CONNECT) {
		OnConnect(hWin);
		return 1;
	}
	return 0;
}

static void PASCAL TTXReadIniFile(const wchar_t *fn, PTTSet ts)
{
	(pvar->origReadIniFile)(fn, ts);

	GetPrivateProfileStringAFileW(INISECTION, "Enabled", "True", pvar->menu_ini,
								  sizeof(pvar->menu_ini), fn);
	if (_stricmp(pvar->menu_ini, "True") == 0) {
		pvar->enabled = TRUE;
		DWORD len = GetPrivateProfileStringAFileW(INISECTION, "Menu", "", pvar->menu_ini,
									 sizeof(pvar->menu_ini), fn);
		if (len == 0) {
			strncpy_s(pvar->menu_ini, sizeof(pvar->menu_ini), DEFAULT_MENU_INI, _TRUNCATE);
			WritePrivateProfileStringAFileW(INISECTION, "Enabled", "True", fn);
			WritePrivateProfileStringAFileW(INISECTION, "Menu", pvar->menu_ini, fn);
		}
	}
	else {
		pvar->enabled = FALSE;
		pvar->menu_ini[0] = 0;
	}
}

static void PASCAL TTXGetSetupHooks(TTXSetupHooks *hooks) {
	pvar->origReadIniFile = *hooks->ReadIniFile;
	*hooks->ReadIniFile = TTXReadIniFile;
}

static TTXExports Exports = {
	sizeof(TTXExports),
	ORDER,

	TTXInit,
	NULL, //TTXGetUIHooks,
	TTXGetSetupHooks,
	NULL, // TTXOpenTCP,
	NULL, // TTXCloseTCP,
	NULL, // TTXSetWinSize,
	TTXModifyMenu,
	NULL, // TTXModifyPopupMenu,
	TTXProcessCommand,
	NULL, // TTXEnd
};

BOOL __declspec(dllexport) PASCAL TTXBind(WORD Version, TTXExports *exports)
{
	int size = sizeof(Exports) - sizeof(exports->size);

	if (size > exports->size) {
		size = exports->size;
	}
	memcpy((char *)exports + sizeof(exports->size),
		   (char *)&Exports + sizeof(exports->size),
		   size);
	return TRUE;
}

BOOL WINAPI DllMain(HANDLE hInstance,
					ULONG ul_reason_for_call,
					LPVOID lpReserved)
{
	switch( ul_reason_for_call ) {
    case DLL_THREAD_ATTACH:
		/* do thread initialization */
		break;
    case DLL_THREAD_DETACH:
		/* do thread cleanup */
		break;
    case DLL_PROCESS_ATTACH:
		/* do process initialization */
		hInst = hInstance;
		pvar = &InstVar;
		break;
    case DLL_PROCESS_DETACH:
		/* do process cleanup */
		break;
	}
	return TRUE;
}
