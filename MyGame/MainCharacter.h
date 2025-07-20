#pragma once
class MainCharacter : public Creature {
public:
	const float MOVEMENT_SPEED = 1;
	const float TOTAL_CHAR_WIDTH = 32;
	const float TOTAL_CHAR_HEIGHT = 32;
	const float BOTTOM_CHAR_WIDTH = 11;
	const float BOTTOM_CHAR_HEIGHT = 18;
	const float LAST_RUN_X = 32 * 2;
	const float STARTING_POSITION = 32 * 1;
	const float CHAR_UPPER_LIMIT = 32*8;
	const float CAMERA_WIDTH = 512;
	const float CAMERA_HEIGHT_TALL = 480;
	const float CAMERA_HEIGHT_SHORT = 288;
	float TOTAL_BACKGROUND = 7160;
	float UPPER_CAMERA_LIMIT = TOTAL_BACKGROUND - CAMERA_WIDTH - TOTAL_CHAR_WIDTH;
	vector<int> runAnimationX = { 0,32,0,64 };
	float gravity;
	float jumpPower;
	float velocityY;
	bool noClick;
	int count;
	bool bigLink;
	int immunity;
	float i;
	SDL_FRect charSrcRect;
	SDL_FRect charDestRect;
	vector<SDL_FRect> charLethalRect;
	SDL_FRect camera;

	MainCharacter(Textures* textures) { // Initialize mainCharacter with all defaults
		charSrcRect = { 0,0,TOTAL_CHAR_WIDTH,TOTAL_CHAR_HEIGHT };
		charDestRect = { 32 * 1,charSrcRect.h, TOTAL_CHAR_WIDTH,TOTAL_CHAR_HEIGHT };
		charLethalRect = { { 32 * 1 + 13, BOTTOM_CHAR_HEIGHT,BOTTOM_CHAR_WIDTH,BOTTOM_CHAR_HEIGHT}, {32 * 1 + 14, TOTAL_CHAR_HEIGHT, 15,TOTAL_CHAR_HEIGHT - BOTTOM_CHAR_HEIGHT} };
		camera = { 0,0,CAMERA_WIDTH,CAMERA_HEIGHT_TALL };
		right = true;
		noClick = true;
		bigLink = false;
		count = 0;
		gravity = .1f;
		jumpPower = -4.0f;
		velocityY = 0.0f;
		immunity = 0;
		i = 0;
	}
	void moveRight(vector<SDL_FRect> platforms, Textures* textures, bool end = false) { // End meaning the goal platform has been reached; level has been beaten
		bool add = false;
		if (right) {
			charSrcRect.y = 0;
			add = true;
		}
		else {
			count = 0;
			i = 0;
		}
		right = true;
		Collision(platforms, charLethalRect.at(0).x + camera.x, charLethalRect.at(0).w, charDestRect.y, charDestRect.h);
		sideCollision(platforms, charLethalRect.at(0).x + camera.x, charLethalRect.at(0).w, charDestRect.y, charDestRect.h, right); // Only move if there is no Side Collision
		if (noSideCollision && charDestRect.x >= CHAR_UPPER_LIMIT)
			camera.x = (camera.x >= UPPER_CAMERA_LIMIT) ? UPPER_CAMERA_LIMIT : camera.x + MOVEMENT_SPEED;
		if (noSideCollision && (charDestRect.x <= CHAR_UPPER_LIMIT - MOVEMENT_SPEED || (camera.x >= UPPER_CAMERA_LIMIT)) || end) {
			charDestRect.x = (charDestRect.x == CAMERA_WIDTH - TOTAL_CHAR_WIDTH) ? CAMERA_WIDTH - TOTAL_CHAR_WIDTH : charDestRect.x + MOVEMENT_SPEED;
		}
		if (onGround || end) { // Only use running animations on the ground OR if level has been beat
			if (count == 10) {
				if (add)
					i++;
				if (i == 4)
					i = 0;
				charSrcRect.x = runAnimationX.at(i);
				count = 0;
			}
			count++;
		}
	}
	void moveLeft(vector<SDL_FRect> platforms, Textures* textures) { // Same thing as moveRight but just going left
		bool add = false;
		if (!right) {
			charSrcRect.y = 32;
			add = true;
		}
		else {
			count = 0;
			i = 0;
		}
		charSrcRect.x = (right) ? 0 : charSrcRect.x;
		right = false;
		Collision(platforms, charLethalRect.at(0).x + camera.x, charLethalRect.at(0).w, charDestRect.y, charDestRect.h);
		sideCollision(platforms, charLethalRect.at(0).x + camera.x, charLethalRect.at(0).w, charDestRect.y, charDestRect.h, right);
		if (noSideCollision && charDestRect.x <= CHAR_UPPER_LIMIT) {
			camera.x = (camera.x <= MOVEMENT_SPEED) ? 0 : camera.x - MOVEMENT_SPEED;
		}
		if (noSideCollision && (camera.x <= MOVEMENT_SPEED || camera.x >= UPPER_CAMERA_LIMIT)) {
			charDestRect.x = (charDestRect.x <= 0) ? 0 : charDestRect.x - MOVEMENT_SPEED;	
		}
		if (onGround) {
			if (count == 10) {
				if (add)
					i++;
				if (i == 4)
					i = 0;
				charSrcRect.x = runAnimationX.at(i);
				count = 0;
			}
			count++;
		}
	}
	void bounceAfterHostileHit(const bool* currentKeyStates, Audio* audio) { // Different audio plays if the user is holding a different button if the hostile was hit
		velocityY = -2.5f;
		if (currentKeyStates[SDL_SCANCODE_UP] || currentKeyStates[SDL_SCANCODE_SPACE]) { // If holding up then launch player higher
			velocityY = -4.0f;
			audio->playSomething("strongpop");
		}
		else {
			audio->playSomething("weakpop");
		}
		onGround = false;
		potentialFall(currentKeyStates);
	}
	void jump(vector<SDL_FRect> platforms, Textures* textures, const bool* currentKeyStates) {
		if ((currentKeyStates[SDL_SCANCODE_UP] || currentKeyStates[SDL_SCANCODE_SPACE]) && onGround) {  // Check if on the ground and jump key is pressed
			velocityY = jumpPower;  // Apply initial jump velocity
			onGround = false;
			noClick = false;
		}
		potentialFall(currentKeyStates);
		for (int i = 0; i < platforms.size(); i++) {
			bumpedHead(platforms.at(i)); // If bumped head go back down at 1/3 of how much the character was in
		}
	}
	bool bumpedHead(SDL_FRect platform) {
		SDL_FRect actualRect = { charLethalRect.at(1).x + camera.x, charDestRect.y, charLethalRect.at(1).w, charDestRect.h };
		SDL_FRect GetDownRect;
		if ((platform.y + platform.h <= charDestRect.y || platform.y + platform.h >= charDestRect.y) && platform.y + platform.h <= previousY && SDL_GetRectIntersectionFloat(&actualRect, &platform, &GetDownRect)) {
			charDestRect.y += GetDownRect.h / 3;
			if (velocityY < 0)
				velocityY = -velocityY; // If going up when bumped head, go back down at the same rate
			return true;
		}
		return false;
	}
	void fall(bool holdingUp = false) { // Makes the character fall
		if (velocityY >= 12) // Max velocity is 12
			velocityY = 12; 
		velocityY += gravity;  // Gravity pulls the character down
		if (holdingUp) {
			velocityY -= .05;
		}
		charDestRect.y += velocityY;
		charSrcRect.x = LAST_RUN_X + TOTAL_CHAR_WIDTH;
		charSrcRect.y = (right) ? 0 : 32;

	}
	void potentialFall(const bool* currentKeyStates) {
		if (!onGround) {
			if (currentKeyStates[SDL_SCANCODE_UP] || currentKeyStates[SDL_SCANCODE_SPACE]) {
				fall(true); // True meaning fall more slowly (results in higher jump if up or space is held down)
			}
			else {
				fall();
			}
			if (!right)
				charSrcRect.y = 32;
		}
		else {
			velocityY = 0;
		}
	}
	bool onTopOfPlatform(SDL_FRect platform) {
		return charDestRect.y + charDestRect.h <= platform.y + 1 && charDestRect.y + charDestRect.h >= platform.y - 1 && charDestRect.x + charDestRect.w + camera.x >= platform.x && charDestRect.x + camera.x <= platform.x + platform.w;
	}
	void charMovement(vector<SDL_FRect> platforms, Textures* textures, const bool* currentKeyStates) {
		noClick = true; // Assume there was no click
		previousY = charDestRect.y; // Get y before any changes
		jump(platforms, textures, currentKeyStates); // Only jumps if up was clicked and is on ground
		if (currentKeyStates[SDL_SCANCODE_LEFT]) {
			moveLeft(platforms, textures); // Move left if clicked left
			noClick = false; 
		}
		if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
			moveRight(platforms, textures); // Move right if clicked right
			noClick = false;
		}
		if (currentKeyStates[SDL_SCANCODE_LEFT] && currentKeyStates[SDL_SCANCODE_RIGHT]) { // If both left and right are clicked do not move char left or right
			charSrcRect.x = 0;
			charSrcRect.y = 0;
			if (!right)
				charSrcRect.y = 32;
			noClick = true; // Should act the same as no click
		}
		if (noClick && onGround) { 
			charSrcRect.x = 0;
			charSrcRect.y = 0;
			if (!right)
				charSrcRect.y = 32;
		}
		Collision(platforms, charLethalRect.at(0).x + camera.x, charLethalRect.at(0).w, charDestRect.y, charDestRect.h); // Check for collision once more
		charLethalRect.at(0).x = charDestRect.x + 10;
		charLethalRect.at(1).x = charDestRect.x + 8;
		if (right) {
			charLethalRect.at(0).x++;
			charLethalRect.at(1).x++;
		}
		charLethalRect.at(0).y = charDestRect.y + (TOTAL_CHAR_HEIGHT - BOTTOM_CHAR_HEIGHT);
		charLethalRect.at(1).y = charLethalRect.at(0).y + (BOTTOM_CHAR_HEIGHT - TOTAL_CHAR_HEIGHT);
		if (bigLink) {
			charLethalRect.at(0).x += 3;
			charLethalRect.at(0).y += 4;
			charLethalRect.at(1).x += 3;
			charLethalRect.at(1).y += 2;

		}
	}
	void setToBigLink() { // BigLink can survive one hit and is 25% larger than normal link
		bigLink = true;
		charDestRect.w *= 1.25;
		charDestRect.h *= 1.25;
		charLethalRect.at(0).w *= 1.25;
		charLethalRect.at(0).h *= 1.25;
		charLethalRect.at(1).w *= 1.25;
		charLethalRect.at(1).h *= 1.25;
	}
	void resetToNormalLink() { // NormalLink is just default link
		bigLink = false;
		charDestRect.y += 10;
		charDestRect.w = TOTAL_CHAR_WIDTH;
		charDestRect.h = TOTAL_CHAR_HEIGHT;
		charLethalRect.at(0).w = BOTTOM_CHAR_WIDTH;
		charLethalRect.at(0).h = BOTTOM_CHAR_HEIGHT;
		charLethalRect.at(1).w = 15;
		charLethalRect.at(1).h = TOTAL_CHAR_HEIGHT - BOTTOM_CHAR_HEIGHT;
		immunity = 0;
	}
	void reset(map<int,SDL_FRect,comparator> platforms) { // Resets the character to the start with defaults
		camera = { 0,0,CAMERA_WIDTH, CAMERA_HEIGHT_TALL };
		right = true;
		noClick = true;
		bigLink = false;
		count = 0;
		i = 0;
		charSrcRect = { 0,0,TOTAL_CHAR_WIDTH,TOTAL_CHAR_HEIGHT };
		charDestRect = { 32 * 1,(*platforms.begin()).second.y - charSrcRect.h, TOTAL_CHAR_WIDTH,TOTAL_CHAR_HEIGHT};
	}
};