#include "Z.h"

int main()
{
    auto* a = new Z_app(800, 600);
    auto* w = a->getMainWidget();

    auto* r1 = new Z_button(w, 0, 0, 0, 0);
    auto* c11 = new Z_button(r1, 0, 0, 0, 0);
    auto* c12 = new Z_button(r1, 0, 0, 0, 0);

    r1->name = "r1";
    c11->name = "c11";
    c12->name = "c12";

    r1->layout = Z_layout::V_BOX;

    auto* r2 = new Z_button(w, 0, 0, 0, 0);
    r2->name = "r2";

    w->layout = Z_layout::H_BOX;

    a->run();
}