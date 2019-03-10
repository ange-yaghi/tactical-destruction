#include <keyframe_animation.h>
#include <timing.h>

#include <game_object.h>

KeyframeAnimation::KeyframeAnimation() {
	m_currentPosition = 0.0f;
	m_end = 0.0f;
}

KeyframeAnimation::~KeyframeAnimation() {
	int nKeys = m_keyframes.size();
	for (int i = 0; i < nKeys; i++) {
		delete m_keyframes[i];
	}
}

void KeyframeAnimation::FindKeyframePair(float position, Keyframe::FLAG flag, Keyframe **a, Keyframe **b) {
	int nKeyframes = m_keyframes.size();

	*a = nullptr;
	*b = nullptr;

	for (int i = 0; i < nKeyframes; i++) {
		if (m_keyframes[i]->IsFlagEnabled(flag) && m_keyframes[i]->GetTimestamp() <= position) {
			*a = m_keyframes[i];
		}

		if (m_keyframes[i]->IsFlagEnabled(flag) && m_keyframes[i]->GetTimestamp() > position) {
			if (*a == nullptr) {
				*a = m_keyframes[i];
			}
			else {
				*b = m_keyframes[i];
			}

			return;
		}
	}
}

Keyframe *KeyframeAnimation::GetLastKeyframe(Keyframe::FLAG flag) {
	int nKeyframes = m_keyframes.size();

	for (int i = nKeyframes - 1; i >= 0; i--) {
		if (m_keyframes[i]->IsFlagEnabled(flag)) {
			return m_keyframes[i];
		}
	}

	return nullptr;
}

bool KeyframeAnimation::IsDone() const {
	return m_currentPosition >= m_end;
}

void KeyframeAnimation::OnStart() {
	AnimationController::OnStart();
	m_currentPosition = 0.0f;
}

void KeyframeAnimation::OnFinish() {
	Keyframe *posKey = GetLastKeyframe(Keyframe::LOCATION_KEY);
	m_object->m_location = posKey->GetPosition();
	m_object->m_velocity = Vector2(0.0f, 0.0f);

	Keyframe *scaleKey = GetLastKeyframe(Keyframe::SCALE_KEY);
	// TODO
}

void KeyframeAnimation::Process() {
	Keyframe *key1, *key2;
	float dt = TIMER.GetFrameDuration();
	float w;

	FindKeyframePair(m_currentPosition, Keyframe::LOCATION_KEY, &key1, &key2);

	if (key1 != nullptr) {
		w = CalculateW(m_currentPosition, key1, key2);
		Vector2 newPosition = key1->GetPosition() * (1.0f - w);
		if (key2 != nullptr) newPosition += key2->GetPosition() * w;

		Vector2 delta = newPosition - m_object->m_location;
		Vector2 impliedVelocity = delta * (1.0f / dt);
		m_object->m_velocity = impliedVelocity;
	}

	m_currentPosition += dt;
}

Keyframe *KeyframeAnimation::AddKeyframe(float position) {
	if (position > m_end) {
		m_end = position;
	}

	Keyframe *newKeyframe = new Keyframe;
	newKeyframe->SetTimestamp(position);

	m_keyframes.push_back(newKeyframe);

	return newKeyframe;
}

float KeyframeAnimation::CalculateW(float position, const Keyframe *a, const Keyframe *b) {
	if (b == nullptr) return 0.0f;
	
	float distance = b->GetTimestamp() - a->GetTimestamp();
	float w = (position - a->GetTimestamp()) / distance;

	float w1 = a->FilterWeight(w);
	float w2 = b->FilterWeight(w);

	float remappedWeight = (1.0f - w) * w1 + w * w2;
	return remappedWeight;
}
