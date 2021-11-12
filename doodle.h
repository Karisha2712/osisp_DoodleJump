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
    int jumpForce;
public:
    Doodle();

    explicit Doodle(HINSTANCE hInst);

    void draw(HDC hdc);

    void update();

    void increaseX(HWND hwnd);

    void decreaseX(HWND hwnd);

    void jump();
};


#endif //DOODLE_DOODLE_H
