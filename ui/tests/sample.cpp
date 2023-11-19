#include "Z.h"

int main()
{
    auto* a = new Z_app(800, 600);
    auto* w = a->getMainWidget();
    auto* b = new Z_button(w,100,100,100,100);

    a->run();
}