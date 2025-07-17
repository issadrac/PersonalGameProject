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
	const float TOTAL_BACKGROUND = 7160;
	const float UPPER_CAMERA_LIMIT = TOTAL_BACKGROUND - CAMERA_WIDTH - TOTAL_CHAR_WIDTH;
	vector<int> runAnimationX = { 0,32,0,64 };
	float gravity;
	float jumpPower;
	float velocityY;
	bool noClick;
	float count;
	bool easyJump;
	bool bigLink;
	int immunity;
	float i;
	SDL_FRect charSrcRect;
	SDL_FRect charDestRect;
	vector<SDL_FRect> charLethalRect;
	SDL_FRect camera;

	MainCharacter(Textures* textures) {
		charSrcRect = { 0,0,TOTAL_CHAR_WIDTH,TOTAL_CHAR_HEIGHT };
		charDestRect = { 32 * 1,charSrcRect.h, TOTAL_CHAR_WIDTH,TOTAL_CHAR_HEIGHT };
		charLethalRect = { { 32 * 1 + 13, BOTTOM_CHAR_HEIGHT,BOTTOM_CHAR_WIDTH,BOTTOM_CHAR_HEIGHT}, {32 * 1 + 14, TOTAL_CHAR_HEIGHT, 15,TOTAL_CHAR_HEIGHT - BOTTOM_CHAR_HEIGHT} };
		camera = { 0,0,CAMERA_WIDTH,CAMERA_HEIGHT_TALL };
		right = true;
		noClick = true;
		easyJump = true;
		bigLink = false;
		count = 0;
		gravity = .1f;
		jumpPower = -4.0f;
		velocityY = 0.0f;
		i = 0;
	}
	void moveRight(vector<SDL_FRect> platforms, Textures* textures, bool end = false) {
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
		Collision(platforms, camera.x, charLethalRect.at(0).x + camera.x, charLethalRect.at(0).w, charDestRect.y, charDestRect.h, right);
		sideCollision(platforms, camera.x, charLethalRect.at(0).x + camera.x, charLethalRect.at(0).w, charDestRect.y, charDestRect.h, right);
		if (noSideCollision && charDestRect.x >= CHAR_UPPER_LIMIT)
			camera.x = (camera.x >= UPPER_CAMERA_LIMIT) ? UPPER_CAMERA_LIMIT : camera.x + MOVEMENT_SPEED;
		if (noSideCollision && (charDestRect.x <= CHAR_UPPER_LIMIT - MOVEMENT_SPEED || (camera.x >= UPPER_CAMERA_LIMIT)) || end) {
			charDestRect.x = (charDestRect.x == CAMERA_WIDTH - TOTAL_CHAR_WIDTH) ? CAMERA_WIDTH - TOTAL_CHAR_WIDTH : charDestRect.x + MOVEMENT_SPEED;
		}
		if (onGround.first || end) {
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
	void moveLeft(vector<SDL_FRect> platforms, Textures* textures) {
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
		Collision(platforms, camera.x, charLethalRect.at(0).x + camera.x, charLethalRect.at(0).w, charDestRect.y, charDestRect.h, right);
		sideCollision(platforms, camera.x, charLethalRect.at(0).x + camera.x, charLethalRect.at(0).w, charDestRect.y, charDestRect.h, right);
		if (noSideCollision && charDestRect.x <= CHAR_UPPER_LIMIT) {
			camera.x = (camera.x <= MOVEMENT_SPEED) ? 0 : camera.x - MOVEMENT_SPEED;
		}
		if (noSideCollision && (camera.x <= MOVEMENT_SPEED || camera.x >= UPPER_CAMERA_LIMIT)) {
			charDestRect.x = (charDestRect.x <= 0) ? 0 : charDestRect.x - MOVEMENT_SPEED;	
		}
		if (onGround.first) {
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
	void bounceAfterHostileHit(const bool* currentKeyStates, Audio* audio) {
		velocityY = -2.5f;
		if (currentKeyStates[SDL_SCANCODE_UP] || currentKeyStates[SDL_SCANCODE_SPACE]) {
			velocityY = -4.0f;
			audio->playSomething("strongpop");
		}
		else {
			audio->playSomething("weakpop");
		}
		onGround.first = false;
		if (!onGround.first) {
			if (currentKeyStates[SDL_SCANCODE_UP] || currentKeyStates[SDL_SCANCODE_SPACE]) {
				fall(true);
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
	void jump(vector<SDL_FRect> platforms, Textures* textures, const bool* currentKeyStates) {
		if ((currentKeyStates[SDL_SCANCODE_UP] || currentKeyStates[SDL_SCANCODE_SPACE]) && onGround.first) {  // Check if on the ground and jump key is pressed
			velocityY = jumpPower;  // Apply initial jump velocity
			onGround.first = false;
		}
		if (!onGround.first) {
			if (currentKeyStates[SDL_SCANCODE_UP] || currentKeyStates[SDL_SCANCODE_SPACE])
				fall(true);
			else
				fall();
			if (!right)
				charSrcRect.y = 32;
		}
		else {
			velocityY = 0;
		}
		for (int i = 0; i < platforms.size(); i++) {
			bumpedHead(platforms.at(i));
		}
	}
	bool bumpedHead(SDL_FRect platform) {
		SDL_FRect actualRect = { charLethalRect.at(1).x + camera.x, charDestRect.y, charLethalRect.at(1).w, charDestRect.h };
		SDL_FRect GetDownRect;
		if ((platform.y + platform.h <= charDestRect.y || platform.y + platform.h >= charDestRect.y) && platform.y + platform.h <= previousY && SDL_GetRectIntersectionFloat(&actualRect, &platform, &GetDownRect)) {
			charDestRect.y += GetDownRect.h / 3;
			if (velocityY < 0)
				velocityY = -velocityY;
			return true;
		}
		return false;
	}
	void fall(bool holdingUp = false) {
		// Apply gravity to vertical velocity (affecting the character's vertical position)
		if (velocityY >= 12)
			velocityY = 12;
		velocityY += gravity;  // Gravity pulls the character down
		if (holdingUp) {
			velocityY -= .05;
		}
		charDestRect.y += velocityY;
		charSrcRect.x = LAST_RUN_X + TOTAL_CHAR_WIDTH;
		charSrcRect.y = (right) ? 0 : 32;

	}
	bool onTopOfPlatform(SDL_FRect platform) {
		return charDestRect.y + charDestRect.h <= platform.y + 1 && charDestRect.y + charDestRect.h >= platform.y - 1 && charDestRect.x + charDestRect.w + camera.x >= platform.x && charDestRect.x + camera.x <= platform.x + platform.w;
	}
	void charMovement(vector<SDL_FRect> platforms, Textures* textures, const bool* currentKeyStates) {
		noClick = true;
		previousY = charDestRect.y;
		jump(platforms, textures, currentKeyStates);
		if (currentKeyStates[SDL_SCANCODE_LEFT]) {
			moveLeft(platforms, textures);
			noClick = false;
		}
		if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
			moveRight(platforms, textures);
			noClick = false;
		}
		if (currentKeyStates[SDL_SCANCODE_LEFT] && currentKeyStates[SDL_SCANCODE_RIGHT]) {
			charSrcRect.x = 0;
			charSrcRect.y = 0;
			if (!right)
				charSrcRect.y = 32;
			noClick = true;
		}
		if (noClick && onGround.first) {
			charSrcRect.x = 0;
			charSrcRect.y = 0;
			if (!right)
				charSrcRect.y = 32;
		}
		Collision(platforms, camera.x, charLethalRect.at(0).x + camera.x, charLethalRect.at(0).w, charDestRect.y, charDestRect.h, right);
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
	void reset(map<int,SDL_FRect,comparator> platforms) {
		camera = { 0,0,CAMERA_WIDTH, CAMERA_HEIGHT_TALL };
		right = true;
		noClick = true;
		bigLink = false;
		count = 0;
		i = 0;
		charSrcRect = { 0,0,TOTAL_CHAR_WIDTH,TOTAL_CHAR_HEIGHT };
		charDestRect = { 32 * 1,(*platforms.begin()).second.y - charSrcRect.h, TOTAL_CHAR_WIDTH,TOTAL_CHAR_HEIGHT};
		charLethalRect = { { 32 * 1 + 10, (*platforms.begin()).second.y - BOTTOM_CHAR_HEIGHT,BOTTOM_CHAR_WIDTH,BOTTOM_CHAR_HEIGHT}, {32 * 1 + 12,(*platforms.begin()).second.y - TOTAL_CHAR_HEIGHT, 15,TOTAL_CHAR_HEIGHT - BOTTOM_CHAR_HEIGHT}};
	}
};