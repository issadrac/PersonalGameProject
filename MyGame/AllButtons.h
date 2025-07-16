#pragma once
class AllButtons {
public:
	//Level Buttons
	Button* level1 = new Button(10, 10, 50, 50, 0, 0, 255, 255);
	Button* level2 = new Button(80, 10, 50, 50, 0, 0, 255, 255);
	//Exit Button
	Button* exit = new Button(0, 32 * 8, 32, 32, 255, 0, 0, 255);
	//Debugging Buttons
	Button* flipFps = new Button(0, 32 * 2, 32, 32, 255, 0, 0, 255);
	Button* renderHitBoxes = new Button(32 * 1, 32 * 2, 32, 32, 0, 255, 0, 255);
	AllButtons() {

	}
	~AllButtons() {
		delete level1;
		delete level2;
		delete flipFps;
		delete renderHitBoxes;
	}
	void mainMenuButtons(SDL_Renderer* r) {
		level1->renderButton(r);
		level2->renderButton(r);
		exit->renderButton(r);
	}
	void debugButtons(SDL_Renderer* r) {
		flipFps->renderButton(r);
		renderHitBoxes->renderButton(r);
	}
};
