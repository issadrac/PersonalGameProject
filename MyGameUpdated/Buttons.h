#pragma once
struct Button {
    SDL_FRect buttonRect;
    std::vector<int> color = { 0,0,0,0 };
    void renderButton(SDL_Renderer* r) {
        SDL_SetRenderDrawColor(r, color[0], color[1], color[2], color[3]);
        SDL_RenderFillRect(r, &buttonRect);
    }
    bool clicked(float scale) {
        float x, y;
        SDL_GetMouseState(&x, &y);
        if (x >= buttonRect.x * scale && x <= (buttonRect.w + buttonRect.x) * scale && y >= buttonRect.y * scale && y <= (buttonRect.h + buttonRect.y) * scale) { // Button could appear larger depending on the scale
            return true;
        }
        return false;
    }
};
