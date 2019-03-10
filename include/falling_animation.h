#ifndef FALLING_ANIMATION_H
#define FALLING_ANIMATION_H

#include <animation_controller.h>

#include <vector.h>

class FallingAnimation : public AnimationController {
public:
	static const float STANDARD_G;

public:
	FallingAnimation();
	~FallingAnimation();

	void SetTargetY(float targetY) { m_targetY = targetY; }
	float GetTargetY() const { return m_targetY; }

	void SetStartingImpulse(const Vector2 &impulse) { m_startingImpulse = impulse; }
	Vector2 GetStartingImpulse() const { return m_startingImpulse; }

	void SetG(float g) { m_g = g; }
	float GetG() const { return m_g; }

	virtual bool IsDone() const;
	virtual void OnStart();
	virtual void OnFinish();
	virtual void Process();

protected:
	float m_targetY;
	float m_g;
	Vector2 m_startingImpulse;
};

#endif /* FALLING_ANIMATION_H */
