#pragma once
class Hostile : public Creature {
public:
    SDL_FRect source, destination;
    vector<SDL_FRect> lethalArea;
    float speedAdjust = 6;
    float speed, targetX, currentVersion, count, distance;
    int numVersions;
    SDL_Texture* texture;
    bool backAndForth;

    Hostile(SDL_FRect src, SDL_FRect dest, vector<SDL_FRect> lethal, float spd, int versions, SDL_Texture* tex, float dist, bool bAF)
        : source(src), destination(dest), lethalArea(lethal), speed(spd), numVersions(versions), texture(tex), distance(dist), backAndForth(bAF) {
        targetX = destination.x;
        if (!backAndForth) { // Get the left animation to move left
            source.y = source.h;
            right = false;
        }
        previousY = destination.y;
        currentVersion = 0; // For hostile with different animations
        count = 0; // To not change the animation every frame
    }
    void xMovement(vector<SDL_FRect> platforms, float cameraX, float prevCameraX) {
        sideCollision(platforms, destination.x, destination.w, destination.y, destination.h, right); // Check for side collision before moving
        if (noSideCollision) {
            destination.x += cameraX - prevCameraX;
            if (!backAndForth) {
                destination.x -= speed / speedAdjust; 
            }
            else {
                if (right && targetX + distance > destination.x) {
                    destination.x += speed / speedAdjust;
                }
                else if (!right && targetX - distance < destination.x) {
                    destination.x -= speed / speedAdjust;
                }
                else {
                    right = !right;
                    source.y = (source.y == source.h) ? 0 : source.h;
                }
            }
        }
    }
    void yMovement(vector<SDL_FRect> platforms) {
        previousY = destination.y;
        Collision(platforms, destination.x, destination.w, destination.y, destination.h); // Check if on Ground
        if (!onGround) {
            destination.y += 1; // If not on ground fall
        }
    }
    void updateTexture() {
        currentVersion += source.w; // The next texture is source.w away
        if (currentVersion / source.w >= numVersions) {
            currentVersion = 0; // Reset to first version
        }
        source.x = currentVersion; // Get the source to be currentVersion
    }

    void render(SDL_Renderer* renderer, float cameraX) {
        SDL_FRect tempDest = destination;
        tempDest.x -= cameraX; // Positions are real world so must subtract by cameraX
        SDL_RenderTexture(renderer, texture, &source, &tempDest);
    }
    void renderLethal(SDL_Renderer* renderer, float cameraX) {
        for (int i = 0; i < lethalArea.size(); i++) {
            SDL_FRect tempDest = lethalArea.at(i);
            tempDest.x -= cameraX; // Positions are real world so must subtract by cameraX
            SDL_RenderRect(renderer, &tempDest);
        }
    }
};