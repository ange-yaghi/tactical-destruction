#include <keyframe.h>

Keyframe::Keyframe() {
	m_rotation = 0.0f;
	m_scale = 1.0f;
	m_position = Vector2(0, 0);
	m_timestamp = 0.0f;
	m_flags = 0x0;
}

Keyframe::~Keyframe() {}

void Keyframe::AddFlag(FLAG flag) {
	m_flags |= (0x1 >> flag);
}

bool Keyframe::IsFlagEnabled(FLAG flag) const {
	return (m_flags & (0x1 >> flag)) > 0;
}
