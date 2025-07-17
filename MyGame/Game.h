#pragma once
class Game {
public:
    int numKeys;
    const bool* currentKeyStates = SDL_GetKeyboardState(&numKeys);
    Textures* textures;
    AllButtons* buttons;
    GamePlatforms* gamePlatforms;
    MainCharacter* mainCharacter;
    HostileActions* hostileActions;
    vector<SDL_FRect> actualPlatforms;
    Audio* audio;
    bool isFullScreen;
    int fps = 170;
    bool flipFps = false;
    bool tall = true;
    int pastLevel = 0;
    pair<bool, bool> renderHit {false,false};
    pair<bool, bool> hit;
    bool newLevel = true;
    int currentLevel = 0;
    SDL_FRect red = { 0,0,0,0 };
    SDL_Event e;
    Uint32 frameStart;
    bool quit = false;
    Game() {
        textures = new Textures();
        gamePlatforms = new GamePlatforms();
        mainCharacter = new MainCharacter(textures);
        hostileActions = new HostileActions(textures);
        buttons = new AllButtons();
        audio = new Audio();
        isFullScreen = false;
        frameStart = SDL_GetTicks();
    }
    void FrameCap() {
        if (flipFps) {
            fps = (fps == 170) ? 5 : 170;
            flipFps = false;
        }
        const int TARGET_FPS = fps;
        const int FRAME_DELAY = 1000 / TARGET_FPS;
        SDL_SetRenderDrawColor(textures->renderer, 0, 0, 0, 0);
        SDL_RenderPresent(textures->renderer);
        SDL_RenderClear(textures->renderer);
        int frameDelay = SDL_GetTicks() - frameStart;
        if (frameDelay < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameDelay);
        }
    }
    void gameLoop() {
        while (!quit) {
            // Start frame time tracking
            frameStart = SDL_GetTicks();
            // Poll events
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_EVENT_QUIT) {
                    quit = true;
                }
                if (SDL_EVENT_MOUSE_BUTTON_DOWN == e.type) {
                    if (SDL_BUTTON_LEFT == e.button.button) {
                        if (currentLevel == 0) {
                            if (buttons->level1->clicked(textures->scale)) {
                                currentLevel = 1;
                                newLevelInitialize();
                                if (isFullScreen) {
                                    textures->setFullScreen(isFullScreen, currentLevel, true);
                                    SDL_SetWindowSize(textures->window, mainCharacter->CAMERA_WIDTH, mainCharacter->CAMERA_HEIGHT_TALL);
                                    textures->setFullScreen(isFullScreen, currentLevel, true);
                                }
                                else
                                    SDL_SetWindowSize(textures->window, mainCharacter->CAMERA_WIDTH, mainCharacter->CAMERA_HEIGHT_TALL);
                                tall = true;
                                audio->playLevelWav(currentLevel);
                            }
                            if (buttons->level2->clicked(textures->scale)) {
                                currentLevel = 2;
                                newLevelInitialize();
                                if (isFullScreen) {
                                    textures->setFullScreen(isFullScreen, currentLevel, true);
                                    SDL_SetWindowSize(textures->window, mainCharacter->CAMERA_WIDTH, mainCharacter->CAMERA_HEIGHT_SHORT);
                                    textures->setFullScreen(isFullScreen, currentLevel, true);
                                }
                                else
                                    SDL_SetWindowSize(textures->window, mainCharacter->CAMERA_WIDTH, mainCharacter->CAMERA_HEIGHT_SHORT);
                                
                                tall = false;
                                audio->playLevelWav(currentLevel);
                            }
                            if (buttons->exit->clicked(textures->scale)) {
                                quit = true;
                            }
                        }
                        if (currentLevel > 0) {
                            if (buttons->flipFps->clicked(textures->scale)) {
                                flipFps = true;
                            }
                            if (buttons->renderHitBoxes->clicked(textures->scale)) {
                                renderHit.first = !renderHit.first;
                                if (renderHit.first && currentKeyStates[SDL_SCANCODE_LCTRL]) {
                                    renderHit.second = !renderHit.second;
                                }
                            }
                        }
                    }
                }
            }
            if (currentLevel == 0) {
                SDL_SetRenderDrawColor(textures->renderer, 0, 0, 0, 0);
                SDL_RenderClear(textures->renderer);
                buttons->mainMenuButtons(textures->renderer);
            }
            if (currentKeyStates[SDL_SCANCODE_ESCAPE]) {
                audio->stopWav();
                tall = true;
                SDL_SetRenderDrawColor(textures->renderer, 0, 0, 0, 0);
                currentLevel = 0;
                if (isFullScreen) {
                    textures->setFullScreen(isFullScreen, currentLevel, true);
                    SDL_SetWindowSize(textures->window, mainCharacter->CAMERA_WIDTH, mainCharacter->CAMERA_HEIGHT_SHORT);
                    textures->setFullScreen(isFullScreen, currentLevel, true);
                }
                else
                    SDL_SetWindowSize(textures->window, mainCharacter->CAMERA_WIDTH, mainCharacter->CAMERA_HEIGHT_SHORT);
                renderHit.first = false;
                newLevel = true;
            }
            if (currentKeyStates[SDL_SCANCODE_F]) {
                textures->setFullScreen(isFullScreen, currentLevel);
            }
            if (currentLevel > 0) {
                if (newLevel) {
                    gamePlatforms->levelPlatforms(currentLevel);
                    hostileActions->addHostiles(textures, currentLevel);
                    string backgroundLevel = "images\\background" + to_string(currentLevel) + ".png";
                    textures->backgroundT = textures->LoadTexture(textures->renderer, backgroundLevel);
                    newLevel = false;
                }
                level();
            }
            if (renderHit.first) {
                renderHitBoxes();
            }
            FrameCap();
        }
        delete textures;
        delete buttons;
        delete gamePlatforms;
        delete mainCharacter;
        delete hostileActions;
        delete audio;
    }
    void newLevelInitialize() {
        SDL_ClearAudioStream(audio->levelStream);
        SDL_ResumeAudioStreamDevice(audio->levelStream);
        if (pastLevel != currentLevel) {
            pastLevel = currentLevel;
            SDL_FlushAudioStream(audio->levelStream);
            gamePlatforms->deletePlatforms();
            hostileActions->deleteHostiles();
            mainCharacter->camera.x = 0;
            mainCharacter->charDestRect.y = 0;
            mainCharacter->charDestRect.x = 32 * 3;
            newLevel = true;
            if(isFullScreen)
                textures->setScale(currentLevel);
        }
        else {
            newLevel = false;
        }
    }
    void level() {
        if (SDL_GetAudioStreamQueued(audio->levelStream) == 0) {
            audio->playLevelWav(currentLevel);
        }
        actualPlatforms.clear();
        SDL_FRect bgRect;
        SDL_FRect addition;
        if (tall) {
            bgRect = { mainCharacter->camera.x, 0, mainCharacter->CAMERA_WIDTH * 2, mainCharacter->CAMERA_HEIGHT_TALL };
            addition = { 0, 0, mainCharacter->CAMERA_WIDTH * 2, mainCharacter->CAMERA_HEIGHT_TALL };
        }
        else {
            bgRect = { mainCharacter->camera.x, 0, mainCharacter->CAMERA_WIDTH * 2, mainCharacter->CAMERA_HEIGHT_SHORT };
            addition = { 0, 0, mainCharacter->CAMERA_WIDTH * 2, mainCharacter->CAMERA_HEIGHT_SHORT };
        }
        int saveI = 0;
        for (int i = 0; i < gamePlatforms->platforms.size(); i++) {
            if (SDL_HasRectIntersectionFloat(&(gamePlatforms->platforms.at(i)), &bgRect)) {
                actualPlatforms.push_back(gamePlatforms->platforms.at(i));
                saveI = i;
            }
            if (gamePlatforms->platforms.at(i).x > mainCharacter->UPPER_CAMERA_LIMIT + 100) {
               break;
            }
        }
        for (int i = 1; i + saveI < gamePlatforms->platforms.size() && i < 4; i++) {
            actualPlatforms.push_back(gamePlatforms->platforms.at(saveI + i));
        }
        for (int i = 0; i < gamePlatforms->specialPlatforms.size(); i++) {
            bool onTop = mainCharacter->onTopOfPlatform(gamePlatforms->specialPlatforms.at(i).second);
            actualPlatforms.push_back(gamePlatforms->specialPlatforms.at(i).second);
            float move = gamePlatforms->updateSpecial(gamePlatforms->specialPlatforms.at(i), i);
            if (gamePlatforms->specialPlatforms.at(i).first == 0 && onTop) {
                mainCharacter->charDestRect.y += move;
                mainCharacter->charLethalRect.at(0).y += move;
                mainCharacter->charLethalRect.at(1).y += move;
            }
            else if (onTop){
                mainCharacter->camera.x += move;
            }
            if (gamePlatforms->specialPlatforms.at(i).first == 2) {
                if (mainCharacter->bigLink == false && mainCharacter->bumpedHead(gamePlatforms->specialPlatforms.at(i).second)) {
                    audio->playSomething("happy");
                    mainCharacter->bigLink = true;
                    mainCharacter->charDestRect.w *= 1.25;
                    mainCharacter->charDestRect.h *= 1.25;
                    mainCharacter->charLethalRect.at(0).w *= 1.25;
                    mainCharacter->charLethalRect.at(0).h *= 1.25;
                    mainCharacter->charLethalRect.at(1).w *= 1.25;
                    mainCharacter->charLethalRect.at(1).h *= 1.25;
                }

            }
        }
        if (renderHit.second) {
            actualPlatforms.push_back({ 1*32,11*32,300*32,1*32 });
        }
        mainCharacter->immunity++;
        float prevCameraX = mainCharacter->camera.x;
        mainCharacter->charMovement(actualPlatforms, textures, currentKeyStates);
        hostileActions->updateHostiles(actualPlatforms, mainCharacter->camera.x, !mainCharacter->noClick, mainCharacter->noSideCollision, mainCharacter->right, currentLevel - 1, mainCharacter->CAMERA_HEIGHT_TALL, mainCharacter->TOTAL_BACKGROUND, textures, prevCameraX);
        renderNecessary(bgRect, addition);
        buttons->debugButtons(textures->renderer);
        hit = hostileActions->checkForHit(mainCharacter->charLethalRect.at(0), mainCharacter->charLethalRect.at(1), red, mainCharacter->previousY, mainCharacter->velocityY, mainCharacter->camera.x);
        if (hit.first && !hit.second && mainCharacter->immunity > 170 * 3) { // 3 seconds immunity
            if (mainCharacter->bigLink == false) {
                audio->playSomething("hurt");
                reset();   
            }
            else {
                mainCharacter->bigLink = false;
                mainCharacter->charDestRect.y += 10;
                mainCharacter->charDestRect.w = mainCharacter->TOTAL_CHAR_WIDTH;
                mainCharacter->charDestRect.h = mainCharacter->TOTAL_CHAR_HEIGHT;
                mainCharacter->charLethalRect.at(0).w = mainCharacter->BOTTOM_CHAR_WIDTH;
                mainCharacter->charLethalRect.at(0).h = mainCharacter->BOTTOM_CHAR_HEIGHT;
                mainCharacter->charLethalRect.at(1).w = 15;
                mainCharacter->charLethalRect.at(1).h = mainCharacter->TOTAL_CHAR_HEIGHT - mainCharacter->BOTTOM_CHAR_HEIGHT;
                mainCharacter->immunity = 0;
                audio->playSomething("surprised");
            }

        }
        else if (hit.second) {
            mainCharacter->bounceAfterHostileHit(currentKeyStates, audio);
        }
        if (mainCharacter->charDestRect.y > mainCharacter->CAMERA_HEIGHT_TALL + 32) {
            audio->playSomething("fall");
            SDL_Delay(500);
            reset(false);
        }
        if (win()) {
            audio->stopWav();
            audio->playWinningWav("winning");
            SDL_FRect flagSrc = { 0, 0, 32, 32 };
            SDL_FRect flagDest = gamePlatforms->flag;
            flagDest.x -= mainCharacter->camera.x;
            fps = 130;
            while (mainCharacter->charDestRect.x <= gamePlatforms->goalPlatform.x + gamePlatforms->goalPlatform.w - mainCharacter->camera.x - 5) {
                frameStart = SDL_GetTicks();
                mainCharacter->moveRight(actualPlatforms, textures, true);
                renderWinningFrame(bgRect, flagSrc, flagDest, addition);
            }
            fps = 170;
            while (mainCharacter->charDestRect.y < gamePlatforms->goalPlatform.y + gamePlatforms->goalPlatform.h) {
                frameStart = SDL_GetTicks();
                mainCharacter->fall();
                if (mainCharacter->charDestRect.y > 32 * 4 + 10) {
                    flagDest.y = mainCharacter->charDestRect.y - 40;
                }
                if (mainCharacter->charDestRect.y >= gamePlatforms->goalPlatform.y + gamePlatforms->goalPlatform.h) {
                    flagDest.y = 352;
                }
                renderWinningFrame(bgRect, flagSrc, flagDest, addition);
            }
            for (int i = 0; i < 32 * 3; i++) {
                frameStart = SDL_GetTicks();
                mainCharacter->moveRight(actualPlatforms, textures, true);
                renderWinningFrame(bgRect, flagSrc, flagDest, addition);
            }
            renderWinningFrame(bgRect, flagSrc, flagDest, addition, true);
            SDL_RenderPresent(textures->renderer);
            SDL_Delay(3000);
            mainCharacter->reset(gamePlatforms->platforms);
            hostileActions->deleteHostiles();
            hostileActions->addHostiles(textures, currentLevel);
            renderHit.first = false;
            renderHit.second = false;
            SDL_RenderClear(textures->renderer);
            SDL_ResumeAudioStreamDevice(audio->levelStream);
            //fps = 170;
        }
    }
    bool win() {
        SDL_FRect goal = gamePlatforms->goalPlatform;
        goal.x = goal.x - mainCharacter->camera.x;
        return SDL_HasRectIntersectionFloat(&mainCharacter->charDestRect, &goal);
    }
    void renderWinningFrame(SDL_FRect bgRect, SDL_FRect flagSrc, SDL_FRect flagDest, SDL_FRect addition, bool disappear = false) {
        bgRect.x = mainCharacter->camera.x;
        flagDest.x = gamePlatforms->flag.x - mainCharacter->camera.x;
        SDL_RenderTexture(textures->renderer, textures->backgroundT, &bgRect, &addition);
        SDL_RenderTexture(textures->renderer, textures->flag, &flagSrc, &flagDest);
        if (!disappear) {
            SDL_RenderTexture(textures->renderer, textures->link, &mainCharacter->charSrcRect, &mainCharacter->charDestRect);
        }
        FrameCap();
    }
    void renderNecessary(SDL_FRect bgRect, SDL_FRect addition) {
        SDL_RenderTexture(textures->renderer, textures->backgroundT, &bgRect, &addition);
        for (int i = 0; i < gamePlatforms->specialPlatforms.size(); i++) {
            SDL_FRect special = gamePlatforms->specialPlatforms.at(i).second;
            special.x = special.x - mainCharacter->camera.x;
            if (gamePlatforms->specialPlatforms.at(i).first != 2) {
                SDL_RenderTexture(textures->renderer, textures->movingPlatform, &(gamePlatforms->moving), &special);
            }
            else {
                SDL_FRect box = { 0,0,32,32 };
                SDL_RenderTexture(textures->renderer, textures->luckyBox, &box, &special);
            }
        }
        SDL_RenderTexture(textures->renderer, textures->link, &mainCharacter->charSrcRect, &mainCharacter->charDestRect);
        SDL_FRect flagSrc = { 0, 0, 32, 32 };
        SDL_FRect flagDest = gamePlatforms->flag;
        flagDest.x -= mainCharacter->camera.x;
        SDL_RenderTexture(textures->renderer, textures->flag, &flagSrc, &flagDest);
        hostileActions->renderHostiles(textures->renderer, currentLevel-1, mainCharacter->camera.x);
    }
    void renderHitBoxes() {
        SDL_SetRenderDrawColor(textures->renderer, 0, 255, 0, 255);
        hostileActions->renderRects(textures->renderer, currentLevel-1, mainCharacter->camera.x);
        SDL_RenderRect(textures->renderer, &mainCharacter->charLethalRect.at(0));
        SDL_RenderRect(textures->renderer, &mainCharacter->charLethalRect.at(1));
        renderPlatforms();
    }
    void renderPlatforms() {
        if (renderHit.second) {
            gamePlatforms->renderPlatforms(textures->renderer, mainCharacter->camera.x, currentLevel-1);
            textures->renderText(mainCharacter->camera.x);
            SDL_RenderRect(textures->renderer, &actualPlatforms.at(actualPlatforms.size() - 1));
        }
        else {
            for (SDL_FRect& platform : actualPlatforms) {
                SDL_FRect adjustedPlatform = { platform.x - mainCharacter->camera.x, platform.y, platform.w, platform.h };
                SDL_RenderFillRect(textures->renderer, &adjustedPlatform);
            }
        }
    }
    void reset(bool fill = true) {
        SDL_SetRenderDrawColor(textures->renderer, 255, 0, 0, 255);
        if(fill)
            SDL_RenderFillRect(textures->renderer, &red);
        if (renderHit.first)
            renderHitBoxes();
        SDL_RenderPresent(textures->renderer);
        SDL_Delay(500);
        mainCharacter->reset(gamePlatforms->platforms);
        hostileActions->reset(textures, currentLevel);
    }
};