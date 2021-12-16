#ifndef UNICODE
#define UNICODE
#endif

#define INTERVAL 40
#define IDT_TIMER1 1

#include <windows.h>
#include <winuser.h>
#include <windowsx.h>
#include <chrono>
#include <random>
#include "doodle.h"
#include "vector"
#include "wingdi.h"
#include "Intsafe.h"

using namespace std;

const int START_X = 480, START_Y = 20, HEIGHT = 800, WIDTH = 600;
Doodle doodle;
HINSTANCE hInst;
bool isLeftDown = false, isRightDown = false;
vector<Platform> platforms;
int platformsNum = 6;
std::random_device dev;
std::mt19937 rng(dev());
uniform_int_distribution<> dist(1, 480);
int gap;
int score = 0;
bool isGameStarted = false;
RECT windowRect;
int mouse_x, mouse_y;
int maxScore = 0;
HBITMAP gameOverBmp, playButton;
wchar_t text[20];
wchar_t text2[20];
wchar_t szCounterFileName[] = L"score.txt";

void WriteMaxScore();

void drawScore(HDC hdc);

void drawEndScore(HDC hdc);

void makePlatforms(HWND hwnd);

void generateNewPlatforms();

void drawPlatforms(HDC hdc);

bool checkIfGameOver();

DWORD ReadCounter();

void showBmp(HDC hdc, HBITMAP bmp, int x, int y);

LRESULT CALLBACK WndProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR
                   lpCmdLine,
                   int nCmdShow) {
    hInst = hInstance;
    WNDCLASSEX wcex{};
    wchar_t CLASS_NAME[] = L"Window";
    wchar_t TITLE[] = L"Doodle Jump";

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
            DWORD mScore = ReadCounter();
            DWordToInt(mScore, &maxScore);
            doodle = Doodle(hInst);
            SetTimer(hwnd, IDT_TIMER1, INTERVAL, ((TIMERPROC) nullptr));
            GetClientRect(hwnd, &windowRect);
            gap = windowRect.bottom / platformsNum;
            gameOverBmp = HBITMAP(
                    LoadImageW(hInst, L"../images/game-over-red.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
            playButton = HBITMAP(
                    LoadImageW(hInst, L"../images/play-btn.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));

        }
            break;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            BeginPaint(hwnd, &ps);
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);

            HDC hBufferDC = CreateCompatibleDC(ps.hdc);
            HBITMAP hBufferBitmap = CreateCompatibleBitmap(ps.hdc, clientRect.right, clientRect.bottom);
            HGDIOBJ hOldBufferBitmap = SelectObject(hBufferDC, hBufferBitmap);

            FillRect(hBufferDC, &clientRect, (HBRUSH) (COLOR_WINDOW + 1));


            doodle.draw(hBufferDC);
            if (!isGameStarted) {
                mouse_x = (windowRect.right / 2) - 100;
                mouse_y = doodle.getY() + 180;
                showBmp(hBufferDC, playButton, mouse_x, mouse_y);
            } else {
                if (!checkIfGameOver()) {
                    generateNewPlatforms();
                    drawPlatforms(hBufferDC);
                    drawScore(hBufferDC);
                } else {
                    isGameStarted = false;
                    int x = (windowRect.right / 2) - 170;
                    int y = doodle.getY() - 120;
                    showBmp(hBufferDC, gameOverBmp, x, y);
                    showBmp(hBufferDC, playButton, mouse_x, mouse_y);
                    drawEndScore(hBufferDC);
                }
            }

            BitBlt(ps.hdc, 0, 0, clientRect.right, clientRect.bottom, hBufferDC, 0, 0, SRCCOPY);

            SelectObject(hBufferDC, hOldBufferBitmap);
            DeleteObject(hBufferDC);
            DeleteObject(hBufferBitmap);


            EndPaint(hwnd, &ps);
        }
            break;
        case WM_LBUTTONDOWN: {
            if (!isGameStarted) {
                if (wParam == MK_LBUTTON) {
                    int x = GET_X_LPARAM(lParam);
                    int y = GET_Y_LPARAM(lParam);
                    if ((x >= mouse_x) && (x <= mouse_x + 198) && (y >= mouse_y) && (y <= mouse_y + 76)) {
                        isGameStarted = true;
                        score = 0;
                        makePlatforms(hwnd);
                        InvalidateRect(hwnd, nullptr, true);
                    }
                }
            }
        }
            break;
        case WM_MOUSEACTIVATE: {
            if (!isGameStarted) {
                if (wParam == MK_LBUTTON) {
                    int x = GET_X_LPARAM(lParam);
                    int y = GET_Y_LPARAM(lParam);
                    if ((x >= mouse_x) && (x <= mouse_x + 198) && (y >= mouse_y) && (y <= mouse_y + 76)) {
                        SetCursor(LoadCursorA(nullptr, reinterpret_cast<LPCSTR>(IDC_HAND)));
                    }
                }
            }
        }
            break;
        case WM_ERASEBKGND: {
            return (LRESULT) 1;
        }
        case WM_KEYDOWN: {
            if (isGameStarted) {
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

        }
            break;
        case WM_KEYUP: {
            if (isGameStarted) {
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
        }
            break;
        case WM_TIMER: {
            if (isGameStarted) {
                int h = doodle.update(platforms);
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
        }
            break;
        case WM_DESTROY: {
            WriteMaxScore();
            KillTimer(hwnd, IDT_TIMER1);
            PostQuitMessage(0);
        }
            break;
        default:
            return DefWindowProc(hwnd, Msg, wParam, lParam);
    }
}

void makePlatforms(HWND hwnd) {
    platformsNum = 6;
    platforms.clear();
    for (int i = 0; i < platformsNum; i++) {
        int x = dist(rng);
        int y = windowRect.bottom - gap * (i + 1) + 100;
        platforms.emplace_back(hInst, x, y);
    }
}

void generateNewPlatforms() {
    if ((doodle.getY() > platforms.back().getY() + gap) && (platformsNum < 240)) {
        int x = dist(rng);
        int y = platforms.back().getY() - gap;
        platforms.emplace_back(hInst, x, y);
        platformsNum++;
    }
}

void drawPlatforms(HDC hdc) {
    int i = 0;
    auto iterator = platforms.cbegin();
    for (auto &platform: platforms) {
        if (platform.getY() >= windowRect.bottom + gap * 2) {
            platforms.erase(iterator + i);
            score++;
        } else {
            platform.draw(hdc);
        }
        i++;
    }
}

void drawScore(HDC hdc) {
    swprintf_s(text, L"%d", score * 13);
    RECT cell;
    cell.left = 10;
    cell.right = 40;
    cell.top = 10;
    cell.bottom = 40;
    DrawText(hdc, text, -1, &cell, DT_CENTER);
}

void drawEndScore(HDC hdc) {
    if (maxScore < score) {
        maxScore = score;
    }
    swprintf_s(text, L"%d", score * 13);
    RECT cell;
    cell.left = windowRect.right / 2 + 10;
    cell.right = windowRect.right / 2 + 40;
    cell.top = doodle.getY() + 100;
    cell.bottom = doodle.getY() + 200;
    DrawText(hdc, text, -1, &cell, DT_CENTER);

    swprintf_s(text2, L"%d", maxScore * 13);
    cell.left = windowRect.right / 2 + 10;
    cell.right = windowRect.right / 2 + 40;
    cell.top = doodle.getY() + 120;
    cell.bottom = doodle.getY() + 140;
    DrawText(hdc, text2, -1, &cell, DT_CENTER);

    wchar_t TEXT[] = L"Your score: ";
    cell.left = windowRect.right / 2 - 70;
    cell.right = windowRect.right / 2 + 10;
    cell.top = doodle.getY() + 100;
    cell.bottom = doodle.getY() + 200;
    DrawText(hdc, TEXT, -1, &cell, DT_CENTER);

    wchar_t TEXT2[] = L"Max score: ";
    cell.left = windowRect.right / 2 - 70;
    cell.right = windowRect.right / 2 + 10;
    cell.top = doodle.getY() + 120;
    cell.bottom = doodle.getY() + 140;
    DrawText(hdc, TEXT2, -1, &cell, DT_CENTER);
}

bool checkIfGameOver() {
    for (auto &platform: platforms) {
        if (platform.getY() > doodle.getY()) {
            return false;
        }
    }
    return true;
}

void showBmp(HDC hdc, HBITMAP bmp, int x, int y) {
    HDC hdcMem;
    BITMAP bitmap;
    HGDIOBJ oldBitmap;
    hdcMem = CreateCompatibleDC(hdc);
    oldBitmap = SelectObject(hdcMem, bmp);
    GetObject(bmp, sizeof(BITMAP), &bitmap);
    StretchBlt(hdc, x, y, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, bitmap.bmWidth,
               bitmap.bmHeight, SRCCOPY);

    SelectObject(hdcMem, oldBitmap);
    DeleteDC(hdcMem);
}

void WriteMaxScore() {
    DWORD dwCounter;
    IntToDWord(maxScore, &dwCounter);
    DWORD dwTemp;
    HANDLE hFile = CreateFile(szCounterFileName, GENERIC_WRITE, 0, nullptr,
                              CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (INVALID_HANDLE_VALUE == hFile) {
        return;
    }
    WriteFile(hFile, &dwCounter, sizeof(dwCounter), &dwTemp, nullptr);
    CloseHandle(hFile);
}

DWORD ReadCounter() {
    DWORD dwCounter, dwTemp;
    HANDLE hFile = CreateFile(szCounterFileName, GENERIC_READ, 0, nullptr,
                              OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (INVALID_HANDLE_VALUE == hFile) {
        return 1;
    }
    ReadFile(hFile, &dwCounter, sizeof(dwCounter), &dwTemp, nullptr);
    if (sizeof(dwCounter) != dwTemp) {
        CloseHandle(hFile);
        return 1;
    }
    CloseHandle(hFile);
    return dwCounter;
}