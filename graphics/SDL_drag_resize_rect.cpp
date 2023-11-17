#include "SDL_window.h"
#include <fmt/core.h>

struct demo_key_ev : SDL_window {

    SDL_Rect myRect;
    SDL_Rect innerRect;

    SDL_Point dragOffSet;

    std::pair<int, int> ogSize;
    SDL_Point resizeInitPoint;

    bool isDragging = false;
    bool isResizing = false;

    demo_key_ev(int width, int height)
        : SDL_window(width, height) {

        };
    ~demo_key_ev() = default;

    bool onBorderQ(SDL_Point p, SDL_Rect& outter, SDL_Rect& inner)
    {
        return SDL_PointInRect(&p, &outter) && !SDL_PointInRect(&p, &inner);
    }

    bool additionalInit() override
    {
        setMainRect({ 0, 0, 100, 100 });
        return true;
    };

    void setMainRect(SDL_Rect rect)
    {
        int border_width = 10;

        myRect = rect;
        innerRect = { rect.x + border_width, rect.y + border_width, rect.w - 2 * border_width, rect.h - 2 * border_width };
    }

    void draw() override
    {
        cairo_set_source_rgba(cr, 1, 1, 1, 1.0);
        cairo_rectangle(cr, 0, 0, width, height);
        cairo_fill(cr);

        cairo_set_source_rgba(cr, 0, 0, 1, 1.0);
        cairo_rectangle(cr, myRect.x, myRect.y, myRect.w, myRect.h);
        cairo_fill(cr);

        cairo_set_source_rgba(cr, 0, 1, 0, 1.0);
        cairo_rectangle(cr, innerRect.x, innerRect.y, innerRect.w, innerRect.h);
        cairo_fill(cr);

        cairo_set_source_rgb(cr, 0, 0, 0);
    };

    void updateStates() override {};

    void onEvent(SDL_Event& ev) override
    {
        if (ev.type == SDL_KEYDOWN) {
            switch (ev.key.keysym.scancode) {
            case SDL_SCANCODE_W:
                myRect.y -= 10;
                break;
            case SDL_SCANCODE_S:
                myRect.y += 10;
                break;
            case SDL_SCANCODE_A:
                myRect.x -= 10;
                break;
            case SDL_SCANCODE_D:
                myRect.x += 10;
                break;

            default:
                break;
            }

            requestRedraw();

        } else if (ev.type == SDL_MOUSEBUTTONUP) {
            isDragging = false;
            isResizing = false;
            requestRedraw();

        } else if (ev.type == SDL_MOUSEBUTTONDOWN) {
            SDL_MouseMotionEvent e = ev.motion;
            auto p = SDL_Point(e.x, e.y);
            if (SDL_PointInRect(&p, &innerRect)) {
                dragOffSet = SDL_Point(e.x - myRect.x, e.y - myRect.y);
                isDragging = true;
            }
            if (onBorderQ(p, myRect, innerRect)) {
                isResizing = true;
                resizeInitPoint = p;
                ogSize = { myRect.w, myRect.h };
            }
            requestRedraw();

        } else if (ev.type == SDL_MOUSEMOTION) {
            SDL_MouseMotionEvent e = ev.motion;
            auto p = SDL_Point(e.x, e.y);

            if (isDragging) {
                setMainRect({ p.x - dragOffSet.x, p.y - dragOffSet.y, myRect.w, myRect.h });
            } else if (isResizing) {
                int newW = abs(ogSize.first + (p.x - resizeInitPoint.x));
                int newH = abs(ogSize.second + (p.y - resizeInitPoint.y));
                setMainRect({ myRect.x, myRect.y, newW, newH });
            }
            requestRedraw();
        }
    };
};

int main()
{
    demo_key_ev app { 800, 600 };
    return app.run();
}
