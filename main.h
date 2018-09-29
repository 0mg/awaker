#include <windows.h>
// common defs (used by *.cpp and *.rc)
#define C_APPVER 0,5,0,0
#define C_APPNAME TEXT("Awaker")
// icon
#define C_APPICON 0x0001
// menu
#define C_CTXMENU 0x0001
// WM_COMMAND & strings
#define C_CMD_EXIT 0x001
#define C_STR_FILE 0x701
#define C_STR_EXIT_CONFIRM 0x702
#define C_CMD_LANG_FIRST 0xFF0
#define C_CMD_LANG_DEFAULT 0xFF0
#define C_CMD_LANG_JA 0xFF1
#define C_CMD_LANG_LAST 0xFFF

// lang mask
#define C_LANG_DEFAULT 0x0000
#define C_LANG_JA 0x1000
