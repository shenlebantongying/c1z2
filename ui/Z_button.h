#pragma once
#include "Z_widget.h"

struct Z_button : Z_widget {

    Z_button(Z_widget* parent, int rel_x, int rel_y, int w, int h);

    void draw() override;
};
