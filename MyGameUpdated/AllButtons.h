#pragma once
class AllButtons {
public:
	//Menu Buttons
		//Level Buttons
		Button level1;
		Button level2;
		//Settings Button
		Button settings;
		//Exit Button
		Button exit;
	//Debugging Buttons
	Button flipFps;
	Button renderHitBoxes;
	AllButtons() {}
	AllButtons(nlohmann::json buttons) {
		setButtonValues(buttons["level1"], level1);
		setButtonValues(buttons["level2"], level2);
		setButtonValues(buttons["settings"], settings);
		setButtonValues(buttons["exit"], exit);
		setButtonValues(buttons["flipFps"], flipFps);
		setButtonValues(buttons["renderHitBoxes"], renderHitBoxes);
	}
	void setButtonValues(const nlohmann::json& val, Button& btn) {
		btn.buttonRect.x = val.at("x");
		btn.buttonRect.y = val.at("y");
		btn.buttonRect.w = val.at("w");
		btn.buttonRect.h = val.at("h");
		btn.color.at(0) = val.at("r");
		btn.color.at(1) = val.at("g");
		btn.color.at(2) = val.at("b");
		btn.color.at(3) = val.at("t");
	}
	void mainMenuButtons(SDL_Renderer* r) {
		level1.renderButton(r);
		level2.renderButton(r);
		settings.renderButton(r);
		exit.renderButton(r);
	}
	void debugButtons(SDL_Renderer* r) {
		flipFps.renderButton(r);
		renderHitBoxes.renderButton(r);
	}
};
