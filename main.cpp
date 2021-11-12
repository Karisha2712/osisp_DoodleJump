#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <winuser.h>
#include <chrono>

const int START_X = 280, START_Y = 20, HEIGHT = 800, WIDTH = 1000;

LRESULT CALLBACK WndProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR
                   lpCmdLine,
                   int nCmdShow) {
    WNDCLASSEX wcex{};
    wchar_t CLASS_NAME[] = L"Window";
    wchar_t TITLE[] = L"lab1";

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = CLASS_NAME;

    RegisterClassEx(&wcex);

    HWND hWnd = CreateWindowEx(
            WS_EX_OVERLAPPEDWINDOW,
            CLASS_NAME,
            TITLE,
            WS_OVERLAPPEDWINDOW,
            START_X, START_Y,
            WIDTH, HEIGHT,
            nullptr,
            nullptr,
            hInstance,
            nullptr
    );

    if (!hWnd) {
        MessageBox(nullptr,
                   L"Call to CreateWindow failed!",
                   L"Windows Desktop Guided Tour",
                   NULL);

        return 1;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    MSG msg{};

    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WndProc(HWND
                         hwnd,
                         UINT Msg, WPARAM
                         wParam,
                         LPARAM lParam) {
    switch (Msg) {
        case WM_DESTROY: {
            PostQuitMessage(0);
        }
            break;
        default:
            return DefWindowProc(hwnd, Msg, wParam, lParam);
    }
}