#include <game_object.h>

#include <input_manager.h>
#include <drawing_manager.h>
#include <timing.h>

PlayerObject::PlayerObject() : GameObject(PLAYER_OBJECT) {}

void PlayerObject::KeyDown(int key) {}

void PlayerObject::ProcessInput() {
	if (INPUT_MANAGER.IsKeyDown(SDLK_UP)) {
		m_movementAccum += Vector2(0, -256);
	}
	else if (INPUT_MANAGER.IsKeyDown(SDLK_DOWN)) {
		m_movementAccum += Vector2(0, 256);
	}

	if (INPUT_MANAGER.IsKeyDown(SDLK_LEFT)) {
		m_movementAccum += Vector2(-256, 0);
	}
	else if (INPUT_MANAGER.IsKeyDown(SDLK_RIGHT)) {
		m_movementAccum += Vector2(256, 0);
	}
}

void PlayerObject::Process() {
	m_location += m_movementAccum * (float)TIMER.GetFrameDuration();
	if (!m_movementAccum.IsZero()) m_animation.Update();

	GameObject::Process();
}

void PlayerObject::Render() {
	DRAWING_MANAGER.DrawImage(m_animation.GetCurrentImage(), m_location, CENTERED);
	
	GameObject::Render();
}
