#pragma once
using namespace std;
class Textures {
public:
    SDL_Texture* grassdirt;
    SDL_Texture* uglyWorm;
    SDL_Texture* link;
    SDL_Texture* backgroundT;
    SDL_Texture* blueThing;
    SDL_Texture* movingPlatform;
    SDL_Texture* flag;
    SDL_Texture* luckyBox;
    vector<SDL_Texture*> platforms;
    SDL_Window* window;
    SDL_Renderer* renderer;

    // Static method for texture loading (called only once per game session)
    static SDL_Texture* LoadTexture(SDL_Renderer* render, const string& name) {
        SDL_Texture* texture = IMG_LoadTexture(render, name.c_str());
        SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
        if (!texture) {
            cout << "Failed to load " << name << " SDL_image Error: " << SDL_GetError() << endl;
            return nullptr;
        }
        return texture;
    }

    // Method to destroy textures in a vector
    static void DestroyTextures(vector<SDL_Texture*>& textures) {
        for (SDL_Texture* tex : textures) {
            SDL_DestroyTexture(tex);
        }
        textures.clear();
    }
    // Constructor: Load all textures once
    Textures() {
        window = SDL_CreateWindow("MyGame", 512, 480, 0);
        if (!window) {
            cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        }
        renderer = SDL_CreateRenderer(window, NULL);
        if (!renderer) {
            cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
        }
        link=LoadTexture(renderer, "images\\Link.png");
        uglyWorm = LoadTexture(renderer, "images\\uglyWorm.png");
        movingPlatform = LoadTexture(renderer, "images\\movingPlatform.png");
        blueThing = LoadTexture(renderer, "images\\bluething.png");
        flag = LoadTexture(renderer, "images\\flag.png");
        luckyBox = LoadTexture(renderer, "images\\luckybox.png");
    }
    ~Textures() {
        DestroyTextures(platforms);
        SDL_DestroyTexture(uglyWorm);
        SDL_DestroyTexture(backgroundT);
    }
    void setFullScreen(bool &isFullScreen, int cl) {
        if (isFullScreen) {
            SDL_RenderClear(renderer);
            SDL_SetWindowFullscreen(window, false);  // Switch to windowed mode
            SDL_SetRenderScale(renderer, 1.0f, 1.0f);
            isFullScreen = false;
        }
        else {
            SDL_RenderClear(renderer);
            SDL_SetWindowFullscreen(window, true);  // Switch to full-screen mode
            if (cl <= 1) {
                SDL_SetRenderScale(renderer, 3.0f, 3.0f);
            }
            else {
                SDL_SetRenderScale(renderer, 5.0f, 5.0f);
            }
            isFullScreen = true;
        }
        SDL_Delay(100);
    }
    void renderText(float cameraX) {
        static TTF_Font* font = TTF_OpenFont("dejavu-sans\\DejaVuSans.ttf", 24);
        if (!font) {
            cout << "Failed to load font! TTF_Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
        }
        char num[3];
        int l = 1;
        SDL_FRect rec{ -cameraX,0,20,40 };
        for (int i = 0; i < 200; i++) {
            if (0 <= rec.x && rec.x <= 32 * 32) {
                if (i < 10) {
                    num[0] = '0' + i; // '0' to '9'
                }
                else if (i < 100) {
                    l = 2;
                    rec.w = 10;
                    num[0] = '0' + (i / 10); // Tens place
                    num[1] = '0' + (i % 10); // Ones place
                }
                else {
                    l = 3;
                    num[0] = '1'; // Hundreds place
                    num[1] = '0' + ((i / 10) % 10); // Tens place 
                    num[2] = '0' + (i % 10); // Ones place
                }
                SDL_Surface* surfaceText = TTF_RenderText_Solid(font, num, l, { 255,255,255 });
                SDL_Texture* textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
                SDL_DestroySurface(surfaceText);
                SDL_RenderTexture(renderer, textureText, NULL, &rec);
                SDL_DestroyTexture(textureText);
            }
            rec.x += 32;
        }
    }
};
/*
vector<SDL_Texture*> rightT;
vector<SDL_Texture*> leftT;
vector<SDL_Texture*> jumpRT;
vector<SDL_Texture*> jumpLT;*/
/*
rightT = {
    LoadTexture(renderer, "images\\SR.png"),   // Idle right
    LoadTexture(renderer, "images\\R1.png"),  // Running right 1
    LoadTexture(renderer, "images\\R2.png"),  // Running right 2
    LoadTexture(renderer, "images\\R3.png"),  // Running right 3
    LoadTexture(renderer, "images\\R4.png")   // Running right 4
};
leftT = {
    LoadTexture(renderer, "images\\SL.png"),   // Idle left
    LoadTexture(renderer, "images\\L1.png"),  // Running left 1
    LoadTexture(renderer, "images\\L2.png"),  // Running left 2
    LoadTexture(renderer, "images\\L3.png"),  // Running left 3
    LoadTexture(renderer, "images\\L4.png")   // Running left 4
};
jumpRT = {
    LoadTexture(renderer, "images\\JR1.png"),  // Jump right 1
    LoadTexture(renderer, "images\\JR2.png")   // Jump right 2
};
jumpLT = {
    LoadTexture(renderer, "images\\JL1.png"),  // Jump left 1
    LoadTexture(renderer, "images\\JL2.png")   // Jump left 2
};*/
/*
DestroyTextures(rightT);
DestroyTextures(leftT);
DestroyTextures(jumpRT);
DestroyTextures(jumpLT);*/