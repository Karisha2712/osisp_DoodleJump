#ifndef DOODLE_DOODLE_H
#define DOODLE_DOODLE_H

#include <windows.h>
#include <winuser.h>
#include "platform.h"
#include "vector"

using namespace std;

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

    int update(const vector<Platform>& platforms);

    void increaseX(HWND hwnd);

    void decreaseX(HWND hwnd);

    void jump();

    int getY();
};


#endif //DOODLE_DOODLE_H
