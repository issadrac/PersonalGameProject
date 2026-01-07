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

    pair<bool, bool> checkForHit(SDL_FRect mc1, SDL_FRect mc2, SDL_FRect& red, float previousY, float vY, float cameraX) { // If mainCharacter jumped on a hostile's head
        mc1.x += cameraX;
        mc2.x += cameraX;
        if (headHostile != nullptr) {
            Hostiles* current = headHostile;
            Hostiles* behind = nullptr;
            while (current != nullptr) {
                for (int i = 0; i < current->hostile.lethalArea.size(); i++) {
                    if (SDL_GetRectIntersectionFloat(&mc1, &current->hostile.lethalArea.at(i), &red) || SDL_GetRectIntersectionFloat(&mc2, &current->hostile.lethalArea.at(i), &red)) {
                        if (previousY + mc1.h <= red.y && vY > 0) { // Delete the hostile if mainCharacter hit from above
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
                        return { true, false }; // Reset the character
                    }
                }
                behind = current; // Keep track of what was behind for potential deletion
                current = current->next;
            }
        }
        return { false, false }; // Nothing happened

    }
    void addHostiles(Textures* tex, nlohmann::json j, float blockSize) { // Adds hostile dependent on the level
        const auto& hostilesToAdd = j["Hostiles"];
        for (const auto& hostile : hostilesToAdd) {
            string name = hostile.at("name");
            SDL_FRect src;
            SDL_FRect dest;
            auto& defaults = j["HostileDefaults"][name];
            if (defaults.at("numLethal") == 1) {
                SDL_FRect lethal;
                src.x = defaults.at("srcX") * blockSize;
                src.y = defaults.at("srcY") * blockSize;
                src.w = defaults.at("allW");
                src.h = defaults.at("allH");
                dest.x = hostile.at("destX") * blockSize;
                dest.y = hostile.at("destY") * blockSize;
                dest.w = defaults.at("allW");
                dest.h = defaults.at("allH");
                lethal.x = hostile.at("lethalX") * blockSize;
                lethal.y = hostile.at("lethalY") * blockSize;
                lethal.w = defaults.at("allW");
                lethal.h = defaults.at("allH");
                float speed = hostile.at("speed");
                float distance = hostile.at("distance") * blockSize;
                SDL_Texture* texture;
                if (name == "blueThing") {
                    texture = tex->blueThing;
                }
                else if (true) { // Hypothetically more 1 lethal Area hostiles
                    texture = tex->blueThing;
                }
                vector<SDL_FRect> lethals;
                lethals.push_back(lethal);
                appendHostile(Hostile(src, dest, lethals, speed, defaults.at("numLethal"), texture, distance, hostile.at("backAndForth")));
            }
            else {
                src.x = defaults.at("srcX") * blockSize;
                src.y = defaults.at("srcY") * blockSize;
                src.w = defaults.at("srcAndDestW");
                src.h = defaults.at("srcAndDestH");
                dest.x = hostile.at("destX") * blockSize;
                dest.y = hostile.at("destY") * blockSize;
                dest.w = defaults.at("srcAndDestW");
                dest.h = defaults.at("srcAndDestH");
                SDL_FRect lethal1;
                lethal1.x = hostile.at("lethalX1") * blockSize;
                lethal1.y = hostile.at("lethalY1") * blockSize;
                lethal1.w = defaults.at("lethalW1");
                lethal1.h = defaults.at("lethalH1");
                SDL_FRect lethal2;
                lethal2.x = hostile.at("lethalX2") * blockSize;
                lethal2.y = hostile.at("lethalY2") * blockSize;
                lethal2.w = defaults.at("lethalW2");
                lethal2.h = defaults.at("lethalH2");
                float speed = hostile.at("speed");
                float distance = hostile.at("distance") * blockSize;
                SDL_Texture* texture;
                if (name == "uglyWorm") {
                    texture = tex->uglyWorm;
                }
                else if (true) { // Hypothetically more (more than 1) lethal Area hostiles
                    texture = tex->uglyWorm;
                }
                vector<SDL_FRect> lethals;
                lethals.push_back(lethal1);
                lethals.push_back(lethal2);
                appendHostile(Hostile(src, dest, lethals, speed, defaults.at("numLethal"), texture, distance, hostile.at("backAndForth")));
            }
        }
    }
    void appendHostile(Hostile h) {
        Hostiles* newHostile = new Hostiles(h);
        if (tailHostile != nullptr) {
            tailHostile->next = newHostile;  // Link the new hostile to the current tail
        }
        if (headHostile == nullptr) {
            headHostile = newHostile;  // If the list was empty, make the new hostile the head
        }
        tailHostile = newHostile;  // Update the tail pointer to the new hostile
    }

    void updateHostiles(vector<SDL_FRect> platforms, float cameraX, bool click, bool sc, bool right, float camHeight, float totBackground, Textures* tex, float prevCameraX) {
        if (headHostile != nullptr) {
            Hostiles* current = headHostile;
            Hostiles* behind = nullptr;
            while (current != nullptr) {
                if ((current->hostile.backAndForth && current->hostile.targetX - cameraX > -100 && current->hostile.targetX - cameraX < 32 * 28)
                    || (!current->hostile.backAndForth && current->hostile.destination.x > -100 && current->hostile.destination.x - cameraX < 32 * 28)) { // Only update if the hostile is on screen
                    current->hostile.xMovement(platforms, cameraX, prevCameraX);
                    current->hostile.yMovement(platforms);
                    if (current->hostile.destination.y > camHeight) { // If the hostile is below the world, then delete
                        Hostiles* toDelete = current;
                        if (behind != nullptr) {
                            behind->next = current->next;
                            if (current->next == nullptr) {
                                tailHostile = behind;
                            }
                        }
                        else {
                            headHostile = current->next;
                            if (headHostile == nullptr) {
                                tailHostile = nullptr;
                            }
                        }
                        current = current->next;
                        delete toDelete;
                        continue;
                    }
                    if (click && sc && cameraX != 0 && cameraX != totBackground) { // Move the hostile if the character moved towards them to counteract
                        if (right) {
                            current->hostile.destination.x -= 1;
                        }
                        else {
                            current->hostile.destination.x += 1;
                        }
                    }
                    if (current->hostile.count == 20) { // Only update the texture every 20 frames
                        current->hostile.updateTexture();
                        current->hostile.count = 0;
                    }
                    current->hostile.count++;
                }
                if (current->hostile.texture == tex->uglyWorm) { // Update lethal depending on the hostile
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
                else if (current->hostile.texture == tex->blueThing) { // Update lethal depending on the hostile
                    current->hostile.lethalArea.at(0) = current->hostile.destination;
                }
                behind = current;
                current = current->next;
            }
        }
    }
    void renderHostiles(SDL_Renderer* renderer, float cameraX) {
        if (headHostile != nullptr) {
            Hostiles* current = headHostile;
            while (current != nullptr) {
                current->hostile.render(renderer, cameraX);
                current = current->next;
            }
        }
    }
    void renderRects(SDL_Renderer* renderer, float cameraX) {
        if (headHostile != nullptr) {
            Hostiles* current = headHostile;
            while (current != nullptr) {
                current->hostile.renderLethal(renderer, cameraX);
                current = current->next;
            }
        }
    }
    void reset(Textures* tex, nlohmann::json j, float blockSize) { // Delete the remaining hostiles for a reset
        deleteHostiles();
        addHostiles(tex, j, blockSize);
    }
    void deleteHostiles() { // Function to just delete all hostiles
        Hostiles* current = headHostile;
        while (current != nullptr) {
            Hostiles* next = current->next;
            delete current;
            current = next;
        }
        headHostile = nullptr;
        tailHostile = nullptr;
    }
};