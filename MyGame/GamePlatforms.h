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
        if (cl == 0) {
            easyAdd(platforms, 0, 13, 16, 2);
            easyAdd(platforms, 18, 12, 4, 1);
            easyAdd(platforms, 24, 9, 8, 1);
            easyAdd(platforms, 26, 5, 5, 1);
            easyAdd(platforms, 32, 12, 3, 1);
            easyAdd(platforms, 35, 8, 5, 1);
            easyAdd(platforms, 40, 4, 7, 1);
            easyAdd(platforms, 50, 13, 4, 1);
            easyAdd(platforms, 59, 13, 5, 1);
            easyAdd(platforms, 60, 5, 4, 1);
            easyAdd(platforms, 65, 13, 5, 1);
            easyAdd(platforms, 70, 9, 3, 1);
            easyAdd(platforms, 76, 6, 6, 1);
            easyAdd(platforms, 98, 11, 4, 1);
            easyAdd(platforms, 104, 7, 8, 1);
            easyAdd(platforms, 113, 13, 3, 1);
            easyAdd(platforms, 116, 9, 4, 1);
            easyAdd(platforms, 122, 9, 4, 1);
            easyAdd(platforms, 129, 13, 30, 2);
            easyAdd(platforms, 138, 9, 2, 4);
            easyAdd(platforms, 140, 7, 2, 6);
            easyAdd(platforms, 142, 5, 2, 8);
            easyAdd(platforms, 152, 12, 1, 1);
            id = 0;
            easyAdd(specialPlatforms, 55, 6, 3, .5, 0);
            easyAdd(specialPlatforms, 83, 8, 3, .5, 1);
            easyAdd(specialPlatforms, 90, 9, 3, .5, 1);
            easyAdd(specialPlatforms, 128, 6, 3, .5, 1);
            easyAdd(specialPlatforms, 60, 10, 1, 1, 2);
            goalPlatform = { 144 * 32, 2 * 32, 8 * 32, 10 * 32 };
            flag = { 152 * 32 - 16, 3 * 32, 1 * 32, 1 * 32 };
        }
        else if (cl == 1) {
            easyAdd(platforms, 0, 4, 5, 1);
            easyAdd(platforms, 5, 5, 4, 1);
            easyAdd(platforms, 8, 5, 1, 4);
            easyAdd(platforms, 13, 6, 1, 3);
            easyAdd(platforms, 14, 6, 10, 1);
            easyAdd(platforms, 24, 7, 1, 2);
            easyAdd(platforms, 29, 7, 1, 2);
            easyAdd(platforms, 30, 7, 1, 1);
            easyAdd(platforms, 31, 5, 1, 2);
            easyAdd(platforms, 32, 5, 9, 1);
            easyAdd(platforms, 40, 5, 1, 2);
            easyAdd(platforms, 41, 6, 2, 1);
            easyAdd(platforms, 43, 7, 6, 1);
            easyAdd(platforms, 49, 5, 1, 2);
            easyAdd(platforms, 50, 3, 1, 2);
            easyAdd(platforms, 51, 2, 7, 1);
            easyAdd(platforms, 58, 3, 2, 1);
            easyAdd(platforms, 60, 4, 5, 1);
            easyAdd(platforms, 65, 5, 3, 1);
            easyAdd(platforms, 68, 6, 3, 1);
            easyAdd(platforms, 71, 3, 1, 3);
            easyAdd(platforms, 72, 3, 3, 1);
            easyAdd(platforms, 75, 4, 1, 1);
            easyAdd(platforms, 76, 5, 1, 2);
            easyAdd(specialPlatforms, 0, 0, 0, 0, 0);
            id = 0;
        }
        else if (cl == 2) {

        }
    }
    float updateSpecial(pair<int, SDL_FRect>& specialPlatform, int i) {
        float moveCharacter = 0;
        if (specialPlatform.first == 0) {
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
        else if (specialPlatform.first == 1) {
            
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
    void easyAdd(map<int,SDL_FRect, comparator>& easyMap, float x, float y, float w, float h) {
        easyMap.insert({ id,{x * 32,y * 32,w * 32,h * 32} });
        id++;
    }
    void easyAdd(vector<pair<int, SDL_FRect>>& specialPlatform, float x, float y, float w, float h, int how) { // 0 is up and down, 1 is left and right, 2 is size upgrade
        specialPlatform.push_back({ how, {x * 32,y * 32,w * 32,h * 32} });
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
    void renderPlatforms(SDL_Renderer* renderer, float cameraX, int cl) {
        char rgb = 'r';
        for (const auto& platform : platforms) {
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
    void addPlatform(float x, float y, float w, float h, int cl) {
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
