#ifndef KEYFRAME_ANIMATION_H
#define KEYFRAME_ANIMATION_H

#include <animation_controller.h>
#include <keyframe.h>

#include <vector>

class KeyframeAnimation : public AnimationController {
public:
	KeyframeAnimation();
	~KeyframeAnimation();

	void FindKeyframePair(float position, Keyframe::FLAG flag, Keyframe **a, Keyframe **b);
	Keyframe *GetLastKeyframe(Keyframe::FLAG flag) const;

	virtual bool IsDone() const;
	virtual void OnStart();
	virtual void OnFinish();
	virtual void Process();

	Keyframe *AddKeyframe(float position);

	static float CalculateW(float position, const Keyframe *a, const Keyframe *b);

protected:
	float m_currentPosition;
	float m_end;

	std::vector<Keyframe *> m_keyframes;
};

#endif /* KEYFRAME_ANIMATION_H */
