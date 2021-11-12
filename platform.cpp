#include "platform.h"

Platform::Platform() {
    picture = nullptr;
    x = 0;
    y = 0;
    height = 0;
    width = 0;
}

Platform::Platform(HINSTANCE hInst, int x, int y) {
    picture = HBITMAP(LoadImageW(hInst, L"../images/platform.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
    this->x = x;
    this->y = y;
    height = 0;
    width = 0;
}

void Platform::draw(HDC hdc) {
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
