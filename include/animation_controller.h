#ifndef ANIMATION_CONTROLLER_H
#define ANIMATION_CONTROLLER_H

#include <vector>

// Forward declarations
class Keyframe;
class GameObject;

class AnimationController {
public:
	AnimationController();
	virtual ~AnimationController();

	virtual bool IsDone() const = 0;

	virtual void OnFinish() = 0;
	virtual void OnStart();

	virtual void Process() = 0;

	void SetObject(GameObject *object) { m_object = object; }
	GameObject *GetObject() const { return m_object; }

	bool IsStarted() const { return m_started; }

protected:
	GameObject *m_object;

	bool m_started;
};

#endif /* ANIMATION_CONTROLLER_H */
