#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "GameObject.h"
#include "Allocators.h"

#include <SDL.h>
#include <map>

class InputManager {
public:

	InputManager();
	~InputManager() {}

	void ProcessEvent(SDL_Event evnt);
	void Register(GameObject *object);
	void Unregister(GameObject *object);

	void PollKeyboard();
	void Reset();
	bool IsKeyDown(int key);

	bool IsMousePressed() {
		Uint8 state = SDL_GetMouseState(0, 0);
		return SDL_BUTTON(state) == SDL_BUTTON_LEFT;
	}

	Vector2 GetMousePos() {
		Vector2 result;
		int x, y;
		SDL_GetMouseState(&x, &y);

		result.x = x;
		result.y = y;

		return result;
	}

protected:
	//Uint8 m_keys[SDLK_LAST];
	std::map<int, Uint8> m_keys;

	Container<GameObject, 1024> m_receiverRegistry;
};

extern InputManager INPUT_MANAGER;

#endif
