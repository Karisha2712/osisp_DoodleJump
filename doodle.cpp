#include "doodle.h"

Doodle::Doodle() {
    x = 0;
    y = 0;
    picture = nullptr;
    width = 0;
    height = 0;
    velocity = 0;
    gravity = 0;
    jumpForce = 0;
}

Doodle::Doodle(HINSTANCE hInst) {
    picture = HBITMAP(LoadImageW(hInst, L"../images/doodler-left.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
    x = 200;
    y = 300;
    width = 60;
    height = 60;
    velocity = 0;
    gravity = 1;
    jumpForce = 20;
}

void Doodle::draw(HDC hdc) {
    HDC hdcMem;
    BITMAP bitmap;
    HGDIOBJ oldBitmap;
    hdcMem = CreateCompatibleDC(hdc);
    oldBitmap = SelectObject(hdcMem, picture);
    GetObject(picture, sizeof(BITMAP), &bitmap);
    StretchBlt(hdc, x, y, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, bitmap.bmWidth,
               bitmap.bmHeight, SRCCOPY);

    SelectObject(hdcMem, oldBitmap);
    DeleteDC(hdcMem);
    width = bitmap.bmWidth;
    height = bitmap.bmHeight;
}

int Doodle::update(const vector<Platform> &platforms) {
    velocity += gravity;
    if (velocity <= -20) {
        velocity = -20;
    }
    for (auto platform: platforms) {
        if ((y + height >= platform.getY()) && (y + height <= (platform.getY() + platform.getHeight()))) {
            int minX = platform.getX() - width / 2;
            int maxX = platform.getX() + platform.getWidth() - width / 2;
            if (x >= minX && x <= maxX) {
                jump();
            }
        }
    }
    return velocity;
}

void Doodle::jump() {
    velocity -= jumpForce;
}

void Doodle::increaseX(HWND hwnd) {
    x += 15;
    RECT windowRect;
    GetClientRect(hwnd, &windowRect);
    if (x >= windowRect.right) {
        x = windowRect.left - width;
    }
}

void Doodle::decreaseX(HWND hwnd) {
    x -= 15;
    RECT windowRect;
    GetClientRect(hwnd, &windowRect);
    if (x <= windowRect.left - width) {
        x = windowRect.right;
    }
}

int Doodle::getY() {
    return y;
}



