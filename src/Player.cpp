#include "GameObject.h"
#include "InputManager.h"
#include "DrawingManager.h"
#include "Timing.h"

PlayerObject::PlayerObject()
{

	m_type = PLAYER_OBJECT;
	//INPUT_MANAGER.Register(this);

}

void PlayerObject::KeyDown(int key)
{

	//if (key == SDLK_UP)
	//{

	//	m_movementAccum += Vector2(0, -100) * TIMER.GetFrameDuration();

	//}

	//else if (key == SDLK_DOWN)
	//{

	//	m_movementAccum += Vector2(0, 100) * TIMER.GetFrameDuration();

	//}

	//else if (key == SDLK_RIGHT)
	//{

	//	m_movementAccum += Vector2(100, 0) * TIMER.GetFrameDuration();

	//}

	//else if (key == SDLK_LEFT)
	//{

	//	m_movementAccum += Vector2(-100, 0) * TIMER.GetFrameDuration();

	//}

}

void PlayerObject::ProcessInput()
{

	if (INPUT_MANAGER.IsKeyDown(SDLK_UP))
	{

		m_movementAccum += Vector2(0, -256);

	}

	else if (INPUT_MANAGER.IsKeyDown(SDLK_DOWN))
	{

		m_movementAccum += Vector2(0, 256);

	}

	if (INPUT_MANAGER.IsKeyDown(SDLK_LEFT))
	{

		m_movementAccum += Vector2(-256, 0);

	}

	else if (INPUT_MANAGER.IsKeyDown(SDLK_RIGHT))
	{

		m_movementAccum += Vector2(256, 0);

	}

}

void PlayerObject::Process()
{

	//m_movementAccum = Vector2(100, 100);

	m_location += m_movementAccum * TIMER.GetFrameDuration();
	if (!m_movementAccum.IsZero()) 
		m_animation.Update();

	GameObject::Process();

}

void PlayerObject::Render()
{

	DRAWING_MANAGER.DrawImage(m_animation.GetCurrentImage(), m_location, CENTERED);

	GameObject::Render();

}