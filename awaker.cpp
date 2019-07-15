#include <windows.h>
#include "main.h"

// defs that *.rc never call
#define C_WINDOW_CLASS TEXT("main_window_class")
#define C_SCWIDTH GetSystemMetrics(SM_CXSCREEN)
#define C_SCHEIGHT GetSystemMetrics(SM_CYSCREEN)

LPCTSTR C_APPNAME_STR = C_APPNAME;

void __start__() {
  // program will start from here if `gcc -nostartfiles`
  STARTUPINFO si;
  GetStartupInfo(&si);
  ExitProcess(WinMain(GetModuleHandle(NULL), 0, NULL, si.wShowWindow));
}

LRESULT CALLBACK mainWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
  switch (msg) {
  case WM_CREATE: {
    SetThreadExecutionState(ES_AWAYMODE_REQUIRED | ES_DISPLAY_REQUIRED | ES_SYSTEM_REQUIRED | ES_CONTINUOUS);
    return 0;
  }
  case WM_KEYDOWN: // same to below
  case WM_LBUTTONDOWN: {
    PostMessage(hwnd, WM_SYSCOMMAND, SC_RESTORE, 0);
    return 0;
  }
  case WM_SYSCOMMAND: {
    switch (wp & 0xFFF0) {
    case SC_MAXIMIZE: {
      SetWindowLongPtr(hwnd, GWL_STYLE, WS_VISIBLE | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_POPUP);
      while (ShowCursor(FALSE) >= 0);
      break;
    }
    case SC_RESTORE: {
      ShowWindow(hwnd, SW_RESTORE);
      SetWindowLongPtr(hwnd, GWL_STYLE, WS_VISIBLE | WS_OVERLAPPEDWINDOW);
      while (ShowCursor(TRUE) < 0);
      break;
    }
    }
    break;
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
  wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
  wc.lpszClassName = C_WINDOW_CLASS;
  wc.hIconSm = (HICON)LoadImage(hi, MAKEINTRESOURCE(C_APPICON), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
  RegisterClassEx(&wc);

  // Main Window: Create, Show
  HWND hwnd = CreateWindowEx(
    WS_EX_LEFT,
    C_WINDOW_CLASS, C_APPNAME_STR,
    (cs == SW_MAXIMIZE ? 0 : WS_VISIBLE) | WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT,
    C_SCWIDTH / 2,
    C_SCHEIGHT / 2,
    NULL, NULL, hi, NULL
  );
  // WinMain() must return 0 before msg loop
  if (hwnd == NULL) return 0;
  if (cs == SW_MAXIMIZE) {
    PostMessage(hwnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
  }

  // main
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0) > 0) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
}
