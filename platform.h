#ifndef DOODLE_PLATFORM_H
#define DOODLE_PLATFORM_H

#include <windows.h>
#include <winuser.h>

class Platform {
private:
    HBITMAP picture;
    int x, y;
    int height, width;
public:
    Platform();

    Platform(HINSTANCE hInst, int x, int y);

    void draw(HDC hdc);

    void update(int i);

};


#endif //DOODLE_PLATFORM_H
