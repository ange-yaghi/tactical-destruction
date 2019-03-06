#include <input_manager.h>

#include <world.h>

InputManager::InputManager() {
	m_receiverRegistry.SetSortMethod(&GameObjectSortCompare);
	m_receiverRegistry.m_dynamicallyAllocated = false;
}

void InputManager::Register(GameObject *object) {
	m_receiverRegistry.AddObject(object);
}

void InputManager::Unregister(GameObject *object) {
	m_receiverRegistry.DeleteObject(object, false);
}

void InputManager::ProcessEvent(SDL_Event event) {
	switch (event.type) {
	case SDL_KEYDOWN:
		for (int i=0; i < m_receiverRegistry.m_nObjects; i++) {
			m_receiverRegistry.m_array[i]->KeyDown(event.key.keysym.sym);
		}
		break;

	case SDL_KEYUP:
		for (int i=0; i < m_receiverRegistry.m_nObjects; i++) {
			m_receiverRegistry.m_array[i]->KeyUp(event.key.keysym.sym);
		}
		break;

	case SDL_MOUSEBUTTONDOWN:
		{
			Vector2 position;
			int x, y;
			int state = SDL_GetMouseState(&x, &y);
			int button = SDL_BUTTON(SDL_BUTTON_RIGHT);

			if (state == SDL_BUTTON(SDL_BUTTON_LEFT)) {
				position.x = x; position.y = y;

				for (int i=0; i < m_receiverRegistry.m_nObjects; i++) {
					if (m_receiverRegistry.m_array[i]->OnMouseClick(position)) {
						break;
					}
				}
			}
			else if (state == SDL_BUTTON(SDL_BUTTON_RIGHT)) {
				position.x = x; position.y = y;

				for (int i=0; i < m_receiverRegistry.m_nObjects; i++) {
					if (m_receiverRegistry.m_array[i]->OnRightClick(position)) {
						break;
					}
				}
			}
		}
		break;
	};
}

void InputManager::PollKeyboard() {
	int keyCount;
	const Uint8 *tempKeys = SDL_GetKeyboardState(&keyCount);

	for(int i=0; i < keyCount; i++) {
		m_keys.insert_or_assign(i, tempKeys[i]);
	}
}

void InputManager::Reset() {
	m_keys.clear();
}

bool InputManager::IsKeyDown(int key) const {
	//if (m_keys.
	return m_keys.at(key) == 1;
}

InputManager INPUT_MANAGER;
