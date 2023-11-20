#pragma once
#include "Z_widget.h"
#include <string>

struct Z_button : Z_widget {

    Z_button(Z_widget* parent, int rel_x, int rel_y, int w, int h);

    std::string name;
    void draw() override;
    const char* toStr() override;

    void responseToHitEvent() override;
    Uint64 gotHitTime = 0;
    bool gotHit = false;
};
