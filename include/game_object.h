#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector.h"
#include "Image.h"

#include <SDL.h>

// Forward declarations
class World;

class GameObject {
public:
	enum OBJECT_TYPE {
		PLAYER_OBJECT,
		GUI_OBJECT,
		NUM_OBJECTS,
		NULL_TYPE = NUM_OBJECTS,
		SEARCH_TYPE = NULL_TYPE,
	};

public:
	GameObject() { m_type = NULL_TYPE; m_systemID = -1; }
	GameObject(OBJECT_TYPE type) : m_type(type), m_systemID(-1) {}
	virtual ~GameObject() {}

	virtual void KeyDown(int key) {}
	virtual void KeyUp(int key) {}
	virtual bool OnMouseClick(Vector2 location) { return false; }
	virtual bool OnRightClick(Vector2 location) { return false; }

	virtual void Process() { m_movementAccum.Clear(); }
	virtual void ProcessInput() {}
	virtual void Render() {}
	virtual void RenderShadow() {}

	virtual void OnDestroy() {}
	virtual void Register() {}

	OBJECT_TYPE GetType() { return m_type; }

private:
	OBJECT_TYPE m_type;
	
public:
	Vector2 m_location;
	Vector2 m_movementAccum;

	int m_systemID;
	World *m_world;
};

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

#endif /* GAME_OBJECT_H */
