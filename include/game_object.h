#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <vector.h>
#include <image.h>
#include <animation.h>
#include <animation_controller.h>

#include <SDL.h>
#include <vector>

// Forward declarations
class World;
class AnimationController;

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
	GameObject();
	GameObject(OBJECT_TYPE type);
	virtual ~GameObject() {}

	virtual void KeyDown(int key) {}
	virtual void KeyUp(int key) {}
	virtual bool OnMouseClick(const Vector2 &location) { return false; }
	virtual bool OnRightClick(const Vector2 &location) { return false; }

	void ProcessAnimations();
	void CheckAnimations();
	virtual void Process();
	void Integrate();
	virtual void ProcessInput() {}
	virtual void Render() {}
	virtual void RenderShadow() {}

	virtual void OnDestroy() {}
	virtual void Register() {}

	bool IsSettled() const;

	OBJECT_TYPE GetType() { return m_type; }

	template<typename T> 
	T *AddAnimationController() {
		T *newController = new T;
		m_animationControllers.push_back(newController);

		// Set up new controller
		newController->SetObject(this);

		return newController;
	}

private:
	OBJECT_TYPE m_type;

	void RemoveAnimationController(int index) {
		AnimationController *controller = m_animationControllers[index];
		controller->OnFinish();
		delete controller;

		m_animationControllers.erase(m_animationControllers.begin() + index);
	}

protected:
	std::vector<AnimationController *> m_animationControllers;
	
public:
	Vector2 m_location;
	Vector2 m_velocity;
	Vector2 m_acceleration;

	int m_systemID;
	World *m_world;
};

#endif /* GAME_OBJECT_H */
