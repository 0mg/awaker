#include <windows.h>
#include "main.h"

// defs that *.rc never call
#define C_WINDOW_CLASS TEXT("main_window_class")
#define C_SCWIDTH GetSystemMetrics(SM_CXSCREEN)
#define C_SCHEIGHT GetSystemMetrics(SM_CYSCREEN)
#define C_FGCOLOR 0xFF000000
#define C_BGCOLOR 0xFFFFFFFF

LPCTSTR C_APPNAME_STR = C_APPNAME;

void __start__() {
  // program will start from here if `gcc -nostartfiles`
  ExitProcess(WinMain(GetModuleHandle(NULL), 0, NULL, 0));
}

LRESULT CALLBACK mainWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
  switch (msg) {
  case WM_CREATE: {
    SetThreadExecutionState(ES_SYSTEM_REQUIRED | ES_CONTINUOUS);
    return 0;
  }
  case WM_CLOSE: {
    DestroyWindow(hwnd);
    return 0;
  }
  case WM_DESTROY: {
    PostQuitMessage(0);
    break;
  }
  }
  end:
  return DefWindowProc(hwnd, msg, wp, lp);
}

int WINAPI WinMain(HINSTANCE hi, HINSTANCE hp, LPSTR cl, int cs) {
  // DPI Scale
  SetProcessDPIAware();

  // Main Window: Settings
  WNDCLASSEX wc;
  SecureZeroMemory(&wc, sizeof(WNDCLASSEX));
  wc.cbSize = sizeof(WNDCLASSEX);
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = mainWndProc;
  wc.hInstance = hi;
  wc.hIcon = (HICON)LoadImage(hi, MAKEINTRESOURCE(C_APPICON), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
  wc.hCursor = (HCURSOR)LoadImage(NULL, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_SHARED);
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wc.lpszClassName = C_WINDOW_CLASS;
  wc.hIconSm = (HICON)LoadImage(hi, MAKEINTRESOURCE(C_APPICON), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
  RegisterClassEx(&wc);

  // Main Window: Create, Show
  HWND hwnd = CreateWindowEx(
    WS_EX_LEFT,
    C_WINDOW_CLASS, C_APPNAME_STR,
    WS_VISIBLE | WS_MINIMIZEBOX | WS_SYSMENU,
    CW_USEDEFAULT, CW_USEDEFAULT,
    C_SCWIDTH / 2,
    C_SCHEIGHT / 2,
    NULL, NULL, hi, NULL
  );
  // WinMain() must return 0 before msg loop
  if (hwnd == NULL) return 0;

  // main
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0) > 0) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
}
