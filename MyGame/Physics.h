#pragma once
class Creature {
public:
	bool onGround;
	bool noSideCollision = true;
	float previousY;
	bool right = true;

	void Collision(vector<SDL_FRect> platforms, float xPos, float wide, float &yPos, float height) {
		for (SDL_FRect& platform : platforms) {
			if (yPos + height >= platform.y && previousY < platform.y && xPos + wide > platform.x && xPos < platform.w + platform.x) { // Detects if a creature is on top of a platform
				yPos = platform.y - height;
				onGround = true; 
				return;
			}
		}
		onGround = false;
	}
	void outputRect(SDL_FRect rec, float scale = 1) {
		cout << endl << rec.x * scale << endl << rec.y * scale << endl << rec.w * scale << endl << rec.h * scale << endl << endl;
	}
	bool splat(vector<SDL_FRect> platforms, int i) { // Unused function for stopping veloictyY if a wall was hit while moving (doesn't apply to connected platforms)
		if (platforms.size() >= 1 && i > 0) {
			if (platforms.at(i - 1).x + platforms.at(i - 1).w == platforms.at(i).x) {
				return false;
			}
			else
				return true;
		}
		else {
			return false;
		}
	}
	bool sideCollision(vector<SDL_FRect> platforms, float xPos, float wide, float yPos, float height, bool right) { // Checks if a movement will cause an intersection with a platform
		SDL_FRect checkRect;
		for (int i = 0; i < platforms.size(); i++) {
			if (!right) {
				checkRect = { xPos-1.5f, yPos,wide ,height };
				if (SDL_HasRectIntersectionFloat(&checkRect, &platforms.at(i)) && (yPos + height != platforms.at(i).y) && (platforms.at(i).y + platforms.at(i).h != yPos)) {
					noSideCollision = false;
					return splat(platforms, i);
				}
			}
			else {
				checkRect = { xPos+1.5f, yPos, wide ,height };
				if (SDL_HasRectIntersectionFloat(&checkRect, &platforms.at(i)) && (yPos + height != platforms.at(i).y) && (platforms.at(i).y + platforms.at(i).h != yPos)) {
					if (yPos + height != platforms.at(i).y && (platforms.at(i).y + platforms.at(i).h != yPos)) {
						noSideCollision = false;
						return splat(platforms, i);
					}
				}
			}
		}
		noSideCollision = true;
		return true;
	}
};