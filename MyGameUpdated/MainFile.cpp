#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include "Buttons.h"
#include "AllButtons.h"
#include "GamePlatforms.h"
#include "Textures.h"
#include "Physics.h"
#include "Hostiles.h"
#include "HostileActions.h"
#include "MainCharacter.h"
#include "Audio.h"
#include "Game.h"
using namespace std;
int main() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return 1;
    }
    if (TTF_Init() == -1) {
        std::cout << "SDL_ttf could not initialize! TTF_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }
    nlohmann::json menuJson;
    std::ifstream f("Menu/menu.json");
    if (!f.is_open()) {
        cout << "Unable to open Menu/menu.json" << endl;
        return -1;
    }
    f >> menuJson;
    SDL_Window* window = SDL_CreateWindow("MyGame", menuJson["Window"]["width"], menuJson["Window"]["height"], 0);
    if (!window) {
        cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
    }
    Game game(window, renderer, menuJson);
    game.gameLoop();
    SDL_DestroyRenderer(game.textures->renderer);
    SDL_DestroyWindow(game.textures->window);
    SDL_Quit();
    return 0;
}
