#include "doodle.h"

Doodle::Doodle() {
    x = 0;
    y = 0;
    picture = nullptr;
    width = 0;
    height = 0;
    velocity = 0;
    gravity = 0;
}

Doodle::Doodle(HINSTANCE hInst) {
    picture = HBITMAP(LoadImageW(hInst, L"../images/doodler-left.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
    x = 450;
    y = 650;
    width = 60;
    height = 60;
    velocity = 0;
    gravity = 10;
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

void Doodle::update() {
    velocity += gravity;
    y -= velocity;
}

