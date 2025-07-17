#pragma once
class Button {
public:
    SDL_FRect buttonRect;
    bool personal;
    std::vector<int> color;
    Button(float x, float y, float w, float h, int r, int g, int b, int t) {
        buttonRect = { x,y,w,h };
        color = { r, g, b, t };
        personal = false;
    }
    void renderButton(SDL_Renderer* r) {
        SDL_SetRenderDrawColor(r, color[0], color[1], color[2], color[3]);
        SDL_RenderFillRect(r, &buttonRect);
    }
    bool clicked(float scale) {
        float x, y;
        SDL_GetMouseState(&x, &y);
        if (x >= buttonRect.x * scale && x <= (buttonRect.w + buttonRect.x) * scale && y >= buttonRect.y * scale && y <= (buttonRect.h + buttonRect.y) * scale) {
            return true;
        }
        return false;
    }
};
