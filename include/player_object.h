#ifndef PLAYER_OBJECT_H
#define PLAYER_OBJECT_H

#include <game_object.h>

class PlayerObject : public GameObject {
public:
	PlayerObject();
	~PlayerObject() {}

	SDL_Surface *m_surface;

	void Process();
	void Render();

	void KeyDown(int key);
	void ProcessInput();

	Animation m_animation;

	bool m_moving;
};

#endif /* PLAYER_OBJECT_H */
