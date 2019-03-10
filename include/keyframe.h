#ifndef KEYFRAME_H
#define KEYFRAME_H

#include <vector.h>

class Keyframe {
public:
	enum FLAG {
		LOCATION_KEY,
		SCALE_KEY,
		ROTATION_KEY,
		NEXT_FLAG
	};

public:
	Keyframe();
	~Keyframe();

	void AddFlag(FLAG flag);
	bool IsFlagEnabled(FLAG flag) const;

	void SetRotation(float rotation) { m_rotation = rotation; }
	float GetRotation() const { return m_rotation; }

	void SetScale(float scale) { m_scale = scale; }
	float GetScale() const { return m_scale; }

	void SetPosition(Vector2 position) { m_position = position; }
	Vector2 GetPosition() const { return m_position; }

	void SetTimestamp(float timestamp) { m_timestamp = timestamp; }
	float GetTimestamp() const { return m_timestamp; }

protected:
	unsigned int m_flags;

	float m_rotation;
	float m_scale;
	Vector2 m_position;

	float m_timestamp;
};

#endif /* KEYFRAME_H */
