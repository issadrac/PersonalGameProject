#pragma once
class Hostile : public Creature {
public:
    SDL_FRect source, destination;
    vector<SDL_FRect> lethalArea;
    float speed, numVersions, targetX, currentVersion, count;
    SDL_Texture* texture;
    bool backAndForth;
    bool found = false;

    Hostile(SDL_FRect src, SDL_FRect dest, vector<SDL_FRect> lethal, float spd, int versions, SDL_Texture* tex, bool bAF = true)
        : source(src), destination(dest), lethalArea(lethal), speed(spd), numVersions(versions), texture(tex), backAndForth(bAF) {
        targetX = destination.x;
        if (!backAndForth) {
            source.y = source.h;
            right = false;
        }
        previousY = destination.y;
        currentVersion = 0;
        count = 0;
    }
    void xMovement(vector<SDL_FRect> platforms, float cameraX, float prevCameraX) {
        sideCollision(platforms, cameraX,  destination.x, destination.w, destination.y, destination.h, right);
        if (noSideCollision) {
            destination.x += cameraX - prevCameraX;
            if (!backAndForth) {
                destination.x -= speed / 6;
            }
            else {
                if (right && targetX + speed * 15 > destination.x) {
                    destination.x += speed / 6;
                }
                else if (!right && targetX - speed * 15 < destination.x) {
                    destination.x -= speed / 6;
                }
                else {
                    right = !right;
                    source.y = (source.y == source.h) ? 0 : source.h;
                }
            }
        }
    }
    void yMovement(vector<SDL_FRect> platforms, float cameraX) {
        previousY = destination.y;
        Collision(platforms, cameraX, destination.x,  destination.w, destination.y, destination.h, right);
        if (!onGround.first) {
            destination.y += 1;
        }
    }
    void updateTexture() {
        currentVersion += source.w;
        if (currentVersion / source.w >= numVersions) {
            currentVersion = 0;
        }
        source.x = currentVersion;
    }

    void render(SDL_Renderer* renderer, float cameraX) {
            SDL_FRect tempDest = destination;
            tempDest.x -= cameraX;
            SDL_RenderTexture(renderer, texture, &source, &tempDest);
    }
    void renderLethal(SDL_Renderer* renderer, float cameraX) {
        for (int i = 0; i < lethalArea.size(); i++) {
                SDL_FRect tempDest = lethalArea.at(i);
                tempDest.x -= cameraX;
               SDL_RenderRect(renderer, &tempDest);
        }
    }
};