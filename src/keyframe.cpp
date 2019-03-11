#include <keyframe.h>

Keyframe::Keyframe() {
	m_rotation = 0.0f;
	m_scale = 1.0f;
	m_position = Vector2(0, 0);
	m_timestamp = 0.0f;

	m_interpolationFunction = LINEAR;
	m_flags = 0x0;
}

Keyframe::~Keyframe() {}

void Keyframe::AddFlag(FLAG flag) {
	m_flags |= (0x1 >> flag);
}

bool Keyframe::IsFlagEnabled(FLAG flag) const {
	return (m_flags & (0x1 >> flag)) > 0;
}

float Keyframe::FilterWeight(float weight) const {
	if (m_interpolationFunction == LINEAR) {
		return weight;
	}
	else if (m_interpolationFunction == COSINE) {
		constexpr float PI = 3.14159f;
		return powf(1.0f - ((::cosf(weight * PI) + 1.0f) / 2.0f), 4.0f);
	}
	else {
		// Default to linear
		return weight;
	}
}
