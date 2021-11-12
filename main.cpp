#ifndef UNICODE
#define UNICODE
#endif

#define INTERVAL 100
#define IDT_TIMER1 1

#include <windows.h>
#include <winuser.h>
#include <chrono>
#include <random>
#include "doodle.h"
#include "platform.h"
#include "vector"
#include "wingdi.h"

using namespace std;

const int START_X = 480, START_Y = 20, HEIGHT = 800, WIDTH = 600;
Doodle doodle;
HINSTANCE hInst;
bool isLeftDown = false, isRightDown = false;
vector<Platform> platforms;
int platformsNum = 6;

void makePlatforms(HWND hwnd);

LRESULT CALLBACK WndProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR
                   lpCmdLine,
                   int nCmdShow) {
    hInst = hInstance;
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
        case WM_CREATE: {
            doodle = Doodle(hInst);
            SetTimer(hwnd, IDT_TIMER1, INTERVAL, ((TIMERPROC) nullptr));
            makePlatforms(hwnd);
        }
            break;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW + 1));
            doodle.draw(hdc);
            RECT windowRect;
            GetClientRect(hwnd, &windowRect);
            MoveToEx(hdc, 0, windowRect.bottom / 2, nullptr);
            for (auto platform: platforms) {
                platform.draw(hdc);
            }
            EndPaint(hwnd, &ps);
        }
            break;
        case WM_KEYDOWN: {
            switch (wParam) {
                case VK_UP: {
                    doodle.jump();
                }
                    break;
                case VK_RIGHT: {
                    isRightDown = true;
                }
                    break;
                case VK_LEFT: {
                    isLeftDown = true;
                }
                    break;
                default: {
                }
            }
            InvalidateRect(hwnd, nullptr, true);
        }
            break;
        case WM_KEYUP: {
            switch (wParam) {
                case VK_RIGHT: {
                    isRightDown = false;
                }
                    break;
                case VK_LEFT: {
                    isLeftDown = false;
                }
                    break;
                default: {
                }
            }
        }
            break;
        case WM_TIMER: {
            int h = doodle.update();
            for (auto &platform: platforms) {
                platform.update(h);
            }
            if (isRightDown) {
                doodle.increaseX(hwnd);
            }
            if (isLeftDown) {
                doodle.decreaseX(hwnd);
            }
            InvalidateRect(hwnd, nullptr, true);
        }
            break;
        case WM_DESTROY: {
            PostQuitMessage(0);
        }
            break;
        default:
            return DefWindowProc(hwnd, Msg, wParam, lParam);
    }
}

void makePlatforms(HWND hwnd) {
    RECT windowRect;
    GetClientRect(hwnd, &windowRect);
    int gap = windowRect.bottom / platformsNum;
    std::random_device dev;
    std::mt19937 rng(dev());
    uniform_int_distribution<> dist(1, windowRect.right - 120);
    for (int i = 0; i < platformsNum; i++) {
        int x = dist(rng);
        int y = windowRect.bottom - gap * (i + 1);
        platforms.emplace_back(hInst, x, y);
    }
}