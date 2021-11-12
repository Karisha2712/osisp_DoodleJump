#ifndef DOODLE_DOODLE_H
#define DOODLE_DOODLE_H

#include <windows.h>
#include <winuser.h>

class Doodle {
private:
    HBITMAP picture;
    int x, y;
    int height, width;
    int velocity{}, gravity{};
public:
    Doodle();

    explicit Doodle(HINSTANCE hInst);

    void draw(HDC hdc);

    void update();
};


#endif //DOODLE_DOODLE_H
