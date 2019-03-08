#include <falling_animation.h>

#include <game_object.h>

const float FallingAnimation::STANDARD_G = 500.0f;

FallingAnimation::FallingAnimation() {
	m_g = STANDARD_G;
	m_startingImpulse = Vector2();
	m_targetY = 0.0f;
	m_object = nullptr;
}

FallingAnimation::~FallingAnimation() {}

bool FallingAnimation::IsDone() const {
	return m_object->m_location.y > m_targetY;
}

void FallingAnimation::OnStart() {
	AnimationController::OnStart();

	m_object->m_velocity.y += m_startingImpulse.y;
	m_object->m_velocity.x += m_startingImpulse.x;
}

void FallingAnimation::OnFinish() {
	m_object->m_velocity.y = 0.0f;
	m_object->m_acceleration.y = 0.0f;
	m_object->m_location.y = m_targetY;
}

void FallingAnimation::Process() {
	m_object->m_acceleration.y = m_g;
}
