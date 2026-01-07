#pragma once
using namespace std;
struct comparator {
    bool operator()(const int& a, const int& b) const {
        return a < b;
    }
};
class GamePlatforms {
public:
    SDL_FRect goalHitBox;
    SDL_FRect flag;
    float speedOfPlatform = .5;
    float blockSize = 32.0;
    vector<pair<bool, int>> originalPosition;
    vector<pair<string, SDL_FRect>> specialPlatforms;
    SDL_FRect moving = { 0,0,96,16 };
    map<int, SDL_FRect, comparator> regularPlatforms;
    void loadPlatformsFromFile(nlohmann::json j) {
        const auto& reg = j["Platforms"]["Regular"];
        for (const auto& p : reg) {
            pair<int, SDL_FRect> platform;
            platform.first = p.at("id");
            platform.second.x = float(p["rect"].at("x")) * blockSize;
            platform.second.y = float(p["rect"].at("y")) * blockSize;
            platform.second.w = float(p["rect"].at("w")) * blockSize;
            platform.second.h = float(p["rect"].at("h")) * blockSize;
            regularPlatforms.insert(platform);
        }
        const auto& special = j["Platforms"]["Special"];
        for (const auto& s : special) {
            pair<string, SDL_FRect> sPlatform;
            if (s.contains("type")) {
                sPlatform.first = s.at("type");
                sPlatform.second.x = float(s["rect"].at("x")) * blockSize;
                sPlatform.second.y = float(s["rect"].at("y")) * blockSize;
                sPlatform.second.w = float(s["rect"].at("w")) * blockSize;
                sPlatform.second.h = float(s["rect"].at("h")) * blockSize;
                specialPlatforms.push_back(sPlatform);
                if (sPlatform.first.compare("Vertical") == 0) {
                    originalPosition.push_back({ true, sPlatform.second.y });
                }
                else if (sPlatform.first.compare("Horizontal") == 0) {
                    originalPosition.push_back({ true, sPlatform.second.x });
                }
                else if (sPlatform.first.compare("SizeUpgrade") == 0) {
                    originalPosition.push_back({ true, 0 });
                }
            }
            else {
                cout << "No Specal Platforms found";
                break;
            }
        }
        const auto& ending = j["Ending"];
        if (ending.contains("goalHitBox")) {
            goalHitBox.x = float(ending["goalHitBox"].at("x")) * blockSize;
            goalHitBox.y = float(ending["goalHitBox"].at("y")) * blockSize;
            goalHitBox.w = float(ending["goalHitBox"].at("w")) * blockSize;
            goalHitBox.h = float(ending["goalHitBox"].at("h")) * blockSize;
        }
        if (ending.contains("flag")) {
            flag.x = float(ending["flag"].at("x")) * blockSize;
            flag.y = float(ending["flag"].at("y")) * blockSize;
            flag.w = float(ending["flag"].at("w")) * blockSize;
            flag.h = float(ending["flag"].at("h")) * blockSize;
        } 
    }
    float updateSpecial(pair<string, SDL_FRect>& specialPlatform, int i) { // The integer determines what kind of special platform it is 
        float moveCharacter = 0; // Moves the character with the platform
        if (specialPlatform.first.compare("Vertical") == 0) {
            if (originalPosition.at(i).first) {
                specialPlatform.second.y += speedOfPlatform;
                moveCharacter = speedOfPlatform;
            }
            else {
                specialPlatform.second.y -= speedOfPlatform;
                moveCharacter = -speedOfPlatform;
            }
            if (originalPosition.at(i).second == specialPlatform.second.y || originalPosition.at(i).second + 32 * 5 == specialPlatform.second.y) {
                originalPosition.at(i).first = !originalPosition.at(i).first;
            }
        }
        else if (specialPlatform.first.compare("Horizontal") == 0) {
            if (originalPosition.at(i).first) {
                specialPlatform.second.x += speedOfPlatform;
                moveCharacter = speedOfPlatform;
            }
            else {
                specialPlatform.second.x -= speedOfPlatform;
                moveCharacter = -speedOfPlatform;
            }
            if (originalPosition.at(i).second == specialPlatform.second.x || originalPosition.at(i).second + 32 * 5 == specialPlatform.second.x) {
                originalPosition.at(i).first = !originalPosition.at(i).first;
            }
        }
        return moveCharacter;
    }
    /*
    void easyAdd(float x, float y, float w, float h) { // A function that easily inserts a new platform with the real world coordinates multiplied by 32 for the tiles
        platforms.insert({ id,{x * 32,y * 32,w * 32,h * 32} });
        id++;
    }
    void easyAdd(float x, float y, float w, float h, string how) { // 0 is up and down, 1 is left and right, 2 is size upgrade
        specialPlatforms.push_back({ how, {x * 32,y * 32,w * 32,h * 32} });
        if (how.compare("Vertical") == 0) {
            originalPosition.push_back({ true, y * 32 });
        }
        else if (how.compare("Horizontal") == 0) {
            originalPosition.push_back({ true, x * 32 });
        }
        else if (how.compare("SizeUpgrade") == 0) {
            originalPosition.push_back({ true, 0 });
        }

    }*/
    void renderPlatforms(SDL_Renderer* renderer, float cameraX) {
        char rgb = 'r';
        for (const auto& platform : regularPlatforms) { // Cycles through R G B for easy 1 2 3 with platforms
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
    /*
    void addPlatform(float x, float y, float w, float h) { // In case automatically multiplying by 32 isn't ideal
        platforms.insert({ id,{x, y, w, h} });
        id++;
    }*/
    void deletePlatforms() {
        regularPlatforms.clear();
        specialPlatforms.clear();
        originalPosition.clear();
        goalHitBox = { 0,0,0,0 };
        flag = { 0,0,0,0 };
    }
};
