#pragma once
using namespace std;
struct comparator {
    bool operator()(const int& a, const int& b) const {
        return a < b;
    }
};
class GamePlatforms {
public:
    SDL_FRect goalPlatform;
    SDL_FRect flag;
    vector<pair<bool, int>> originalPosition;
    vector<pair<int, SDL_FRect>> specialPlatforms;
    SDL_FRect moving = { 0,0,96,16 };
    map<int, SDL_FRect, comparator> platforms;
    int id = 0;

    void levelPlatforms(int cl) {
        if (cl == 1) {
            easyAdd(0, 13, 16, 2);
            easyAdd(18, 12, 4, 1);
            easyAdd(24, 9, 8, 1);
            easyAdd(26, 5, 5, 1);
            easyAdd(32, 12, 3, 1);
            easyAdd(35, 8, 5, 1);
            easyAdd(40, 4, 7, 1);
            easyAdd(50, 13, 4, 1);
            easyAdd(59, 13, 5, 1);
            easyAdd(60, 5, 4, 1);
            easyAdd(65, 13, 5, 1);
            easyAdd(70, 9, 3, 1);
            easyAdd(76, 6, 6, 1);
            easyAdd(98, 11, 4, 1);
            easyAdd(104, 7, 8, 1);
            easyAdd(113, 13, 3, 1);
            easyAdd(116, 9, 4, 1);
            easyAdd(122, 9, 4, 1);
            easyAdd(129, 13, 30, 2);
            easyAdd(138, 9, 2, 4);
            easyAdd(140, 7, 2, 6);
            easyAdd(142, 5, 2, 8);
            easyAdd(152, 12, 1, 1);
            id = 0;
            easyAdd(55, 6, 3, .5, 0);
            easyAdd(83, 8, 3, .5, 1);
            easyAdd(90, 9, 3, .5, 1);
            easyAdd(128, 6, 3, .5, 1);
            easyAdd(60, 10, 1, 1, 2);
            goalPlatform = { 144 * 32, 2 * 32, 8 * 32, 10 * 32 };
            flag = { 152 * 32 - 16, 3 * 32, 1 * 32, 1 * 32 };
        }
        else if (cl == 2) {
            easyAdd(0, 4, 5, 1);
            easyAdd(5, 5, 4, 1);
            easyAdd(8, 5, 1, 4);
            easyAdd(13, 6, 1, 3);
            easyAdd(14, 6, 10, 1);
            easyAdd(24, 7, 1, 2);
            easyAdd(29, 7, 1, 2);
            easyAdd(30, 7, 1, 1);
            easyAdd(31, 5, 1, 2);
            easyAdd(32, 5, 9, 1);
            easyAdd(40, 5, 1, 2);
            easyAdd(41, 6, 2, 1);
            easyAdd(43, 7, 6, 1);
            easyAdd(49, 5, 1, 2);
            easyAdd(50, 3, 1, 2);
            easyAdd(51, 2, 7, 1);
            easyAdd(58, 3, 2, 1);
            easyAdd(60, 4, 5, 1);
            easyAdd(65, 5, 3, 1);
            easyAdd(68, 6, 3, 1);
            easyAdd(71, 3, 1, 3);
            easyAdd(72, 3, 3, 1);
            easyAdd(75, 4, 1, 1);
            easyAdd(76, 5, 1, 2);
            easyAdd(0, 0, 0, 0, 0);
            id = 0;
        }
        else if (cl == 3) {

        }
    }
    float updateSpecial(pair<int, SDL_FRect>& specialPlatform, int i) { // The integer determines what kind of special platform it is 
        float moveCharacter = 0; // Moves the character with the platform
        if (specialPlatform.first == 0) { // 0 is up and down
            if (originalPosition.at(i).first) { 
                specialPlatform.second.y += .5;
                moveCharacter = .5; 
            }
            else {
                specialPlatform.second.y -= .5;
                moveCharacter = -.5;
            }
            if (originalPosition.at(i).second == specialPlatform.second.y || originalPosition.at(i).second + 32 * 5 == specialPlatform.second.y) {
                originalPosition.at(i).first = !originalPosition.at(i).first;
            }
        }
        else if (specialPlatform.first == 1) { // 1 is left and right
            if (originalPosition.at(i).first) {
                specialPlatform.second.x += .5;
                moveCharacter = .5;
            }
            else {
                specialPlatform.second.x -= .5;
                moveCharacter = -.5;
            }
            if (originalPosition.at(i).second == specialPlatform.second.x || originalPosition.at(i).second + 32 * 5 == specialPlatform.second.x) {
                originalPosition.at(i).first = !originalPosition.at(i).first;
            }
        }
        return moveCharacter;
    }
    void easyAdd(float x, float y, float w, float h) { // A function that easily inserts a new platform with the real world coordinates multiplied by 32 for the tiles
        platforms.insert({ id,{x * 32,y * 32,w * 32,h * 32} });
        id++;
    }
    void easyAdd(float x, float y, float w, float h, int how) { // 0 is up and down, 1 is left and right, 2 is size upgrade
        specialPlatforms.push_back({ how, {x * 32,y * 32,w * 32,h * 32} });
        if (how == 0) {
            originalPosition.push_back({ true, y * 32});
        }
        else if (how == 1) {
            originalPosition.push_back({ true, x * 32});
        }
        else {
            originalPosition.push_back({ true, 0 });
        }

    }
    void renderPlatforms(SDL_Renderer* renderer, float cameraX) {
        char rgb = 'r';
        for (const auto& platform : platforms) { // Cycles through R G B for easy 1 2 3 with platforms
                if (rgb == 'r') {
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                    rgb = 'g';
                }
                else if (rgb == 'g') {
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                    rgb = 'b';
                }
                else if (rgb == 'b') {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                    rgb = 'r';
                }
                SDL_FRect adjustedPlatform = { platform.second.x - cameraX, platform.second.y, platform.second.w, platform.second.h };
                SDL_RenderFillRect(renderer, &adjustedPlatform);
        }
    }
    void addPlatform(float x, float y, float w, float h) { // In case automatically multiplying by 32 isn't ideal
        platforms.insert({ id,{x, y, w, h} });
        id++;
    }
    void deletePlatforms() {
        platforms.clear();
        specialPlatforms.clear();
        originalPosition.clear();
        goalPlatform = { 0,0,0,0 };
        flag = { 0,0,0,0 };
    }
};
