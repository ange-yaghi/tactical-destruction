#include "animation_controller.h"

AnimationController::AnimationController() { 
	m_started = false; 
	m_object = nullptr; 
}

AnimationController::~AnimationController() {

}

void AnimationController::OnStart() {
	m_started = true;
}
