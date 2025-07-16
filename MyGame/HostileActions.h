
#include "Hostiles.h"
#include "Audio.h"
struct Hostiles {
    Hostile hostile;
    Hostiles* next;
    Hostiles(const Hostile& h) : hostile(h), next(nullptr) {}
};
class HostileActions {
public:
    Hostiles* headHostile;
    Hostiles* tailHostile;
    HostileActions(Textures* tex) {
        /*
      //  level1.push_back(Hostile({ 0, 0, 26, 16 }, { 32 * 6 + 10, 96, 26, 16 }, { {32 * 6 + 10, 104, 8, 8} , {32 * 6 + 18, 96, 18, 16} }, 4, 2, tex->uglyWorm));
        level1.push_back(Hostile({ 0, 0, 19, 22 }, { 32 * 15, 96, 19, 22 }, { { 32 * 15, 96, 19, 22 } }, 6, 1, tex->blueThing, false));
        //level1.push_back(Hostile({ 0, 0, 32, 32 }, { 416, 96, 32, 32 }, { { 416, 96, 32, 32 } }, 3, 3, tex->link));
        hostiles.push_back(level1);*/
    }
    pair<bool, bool> checkForHit(SDL_FRect mc1, SDL_FRect mc2, SDL_FRect &red, int previousY, int vY, float cameraX) {
        mc1.x += cameraX;
        mc2.x += cameraX;
        if (headHostile != nullptr) {
            Hostiles* current = headHostile;
            Hostiles* behind = nullptr;
            while (current != nullptr) {
                for (int i = 0; i < current->hostile.lethalArea.size(); i++) {
                    if (SDL_GetRectIntersectionFloat(&mc1, &current->hostile.lethalArea.at(i), &red) || SDL_GetRectIntersectionFloat(&mc2, &current->hostile.lethalArea.at(i), &red)) {
                        if (previousY + mc1.h < red.y && vY > 0) {
                            Hostiles* toDelete = current;
                            if (behind != nullptr) {
                                behind->next = current->next;
                            }
                            else {
                                headHostile = current->next;
                            }
                            current = current->next;
                            delete toDelete;
                           
                            return { false, true };
                        }
                        return { true, false };
                    }
                }
                behind = current;
                current = current->next;
            }
        }
        return { false, false };

    }
    void addHostiles(Textures* tex, int cl) {
        if (cl == 0) {
            appendHostile(tex, Hostile({ 0, 0, 19, 22 }, { 32 * 28, 96, 19, 22 }, { { 32 * 28, 96, 19, 22 } }, 3, 1, tex->blueThing));
            appendHostile(tex, Hostile({ 0, 0, 26, 16 }, { 32 * 46, 32 * 3, 26, 16 }, { {32 * 46, 8, 8, 8} , {32 * 46 + 8, 0, 18, 16} }, 2, 2, tex->uglyWorm, false));
            appendHostile(tex, Hostile({ 0, 0, 26, 16 }, { 32 * 47, 32 * 3, 26, 16 }, { {32 * 47, 8, 8, 8} , {32 * 47 + 8, 0, 18, 16} }, 2, 2, tex->uglyWorm, false));
            appendHostile(tex, Hostile({ 0, 0, 26, 16 }, { 32 * 82, 32 * 5, 26, 16 }, { {32 * 80, 8, 8, 8} , {32 * 80 + 8, 0, 18, 16} }, 1, 2, tex->uglyWorm, false));
            appendHostile(tex, Hostile({ 0, 0, 19, 22 }, { 32 * 133, 32 * 10, 19, 22 }, { { 32 * 133, 32 * 10, 19, 22 } }, 3, 1, tex->blueThing));
        }
        else if (cl == 1) {
            appendHostile(tex, Hostile({ 0, 0, 26, 16 }, { 32 * 6 + 10, 96, 26, 16 }, { {32 * 6 + 10, 104, 8, 8} , {32 * 6 + 18, 96, 18, 16} }, 4, 2, tex->uglyWorm));
            appendHostile(tex, Hostile({ 0, 0, 19, 22 }, { 32 * 15, 96, 19, 22 }, { { 32 * 15, 96, 19, 22 } }, 6, 1, tex->blueThing, false));
        }
        else if (cl == 2) {

        }
    }
    void appendHostile(Textures* tex, Hostile h) {
        Hostiles* newHostile = new Hostiles(h);
        if (tailHostile != nullptr) {
            tailHostile->next = newHostile;  // Link the new hostile to the current tail
        }
        if (headHostile == nullptr) {
            headHostile = newHostile;  // If the list was empty, make the new hostile the head
        }
        tailHostile = newHostile;  // Update the tail pointer to the new hostile
    }

    void updateHostiles(vector<SDL_FRect> platforms, float cameraX, bool click, bool sc, bool right, int cl, float camHeight, float totBackground, Textures* tex, float prevCameraX) {
        if (headHostile != nullptr) {
            Hostiles* current = headHostile;
            Hostiles* behind = nullptr;
            while (current != nullptr) {
                if ((current->hostile.backAndForth && current->hostile.targetX - cameraX > -100 && current->hostile.targetX - cameraX < 32 * 28) 
                    || (!current->hostile.backAndForth && current->hostile.destination.x > -100 && current->hostile.destination.x - cameraX < 32 * 28)) {
                    current->hostile.xMovement(platforms, cameraX, prevCameraX);
                    current->hostile.yMovement(platforms, cameraX);
                    if (current->hostile.destination.y > camHeight) {
                        Hostiles* toDelete = current; 
                        if (behind != nullptr) {
                            behind->next = current->next;
                        }
                        else {
                            headHostile = current->next;
                        }
                        current = current->next;
                        delete toDelete; 
                        continue;
                    }
                    if (click && sc && cameraX != 0 && cameraX != totBackground) {
                        if (right) {
                            current->hostile.destination.x -= 1;
                        }
                        else {
                            current->hostile.destination.x += 1;
                        }
                    }
                    if (current->hostile.count == 20) {
                        current->hostile.updateTexture();
                        current->hostile.count = 0;
                    }
                    current->hostile.count++;
                }
                if (current->hostile.texture == tex->uglyWorm) {
                    if (!current->hostile.right) {
                        current->hostile.lethalArea.at(0).x = current->hostile.destination.x + 18;
                        current->hostile.lethalArea.at(0).y = current->hostile.destination.y + 8;
                        current->hostile.lethalArea.at(1).x = current->hostile.destination.x;
                        current->hostile.lethalArea.at(1).y = current->hostile.destination.y;
                    }
                    else {
                        current->hostile.lethalArea.at(0).x = current->hostile.destination.x;
                        current->hostile.lethalArea.at(0).y = current->hostile.destination.y + 8;
                        current->hostile.lethalArea.at(1).x = current->hostile.destination.x + 8;
                        current->hostile.lethalArea.at(1).y = current->hostile.destination.y;
                    }
                }
                if (current->hostile.texture == tex->blueThing) {
                    current->hostile.lethalArea.at(0) = current->hostile.destination;
                }
                behind = current;
                current = current->next;
            }  
        }
    }
    void renderHostiles(SDL_Renderer* renderer, int cl, float cameraX) {
        if (headHostile != nullptr) {
            Hostiles* current = headHostile;
            while (current != nullptr) {
                current->hostile.render(renderer, cameraX);
                current = current->next;
            }
        }
    }
    void renderRects(SDL_Renderer* renderer, int cl, float cameraX) {
        if (headHostile != nullptr) {
            Hostiles* current = headHostile;
            while (current != nullptr) {
                current->hostile.renderLethal(renderer, cameraX);
                current = current->next;
            }
        }
    }
    void reset(Textures* tex, int cl) {
        Hostiles* current = headHostile;
        while (current != nullptr) {
            Hostiles* next = current->next;
            delete current;
            current = next;
        }
        headHostile = nullptr;
        addHostiles(tex, cl);
    }
    
    void deleteHostiles() {
        Hostiles* current = headHostile;
        while (current != nullptr) {
            Hostiles* next = current->next;
            delete current;
            current = next;
        }
        headHostile = nullptr;
    }
};