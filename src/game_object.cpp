#include <game_object.h>

#include <timing.h>

GameObject::GameObject() : m_type(NULL_TYPE), m_systemID(-1) {
	m_location = Vector2();
	m_velocity = Vector2();
	m_acceleration = Vector2();
}

GameObject::GameObject(OBJECT_TYPE type) : m_type(type), m_systemID(-1) {
	m_location = Vector2();
	m_velocity = Vector2();
	m_acceleration = Vector2();
}

void GameObject::Process() {
	
}

void GameObject::Integrate() {
	m_velocity += m_acceleration * (float)TIMER.GetFrameDuration();
	m_location += m_velocity * (float)TIMER.GetFrameDuration();
}

bool GameObject::IsSettled() const {
	return m_acceleration.IsZero() && m_velocity.IsZero();
}

void GameObject::ProcessAnimations() {
	int nAnimations = (int)m_animationControllers.size();

	for (int i = 0; i < nAnimations; i++) {
		if (!m_animationControllers[i]->IsStarted()) {
			m_animationControllers[i]->OnStart();
		}
	}

	for (int i = 0; i < nAnimations; i++) {
		m_animationControllers[i]->Process();
	}
}

void GameObject::CheckAnimations() {
	int nAnimations = (int)m_animationControllers.size();

	for (int i = 0; i < nAnimations; i++) {
		if (m_animationControllers[i]->IsDone()) {
			RemoveAnimationController(i);

			// Update the index to not break the for loop
			nAnimations--;
			i--;
		}
	}
}
