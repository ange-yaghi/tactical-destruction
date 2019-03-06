#include <animation.h>

#include <drawing_manager.h>
#include <timing.h>

Animation::Animation() { 
	m_images = 0; 
	m_nFrames = 0; 
	m_frameRate = DEFAULT_FRAME_RATE; 
	m_currentLocation = 0.0; 
	m_playMode = PM_PLAY; 
}

Animation::~Animation() {}

void Animation::LoadAnimation(const char *fname, const char *ext, int start, int end) {
	char *temp = DRAWING_MANAGER.ALLOCATOR.Allocate<char>(1024);

	m_nFrames = end - start + 1;
	m_images = DRAWING_MANAGER.ALLOCATOR.Allocate<Image>(m_nFrames);

	for (int i = start; i <= end; i++) {
		sprintf_s(temp, 1024, "%s%0.4i.%s", fname, i, ext);
		m_images[i - start].LoadImage(temp);

		assert(m_images[i - start].GetSurface() != NULL);
	}

	DRAWING_MANAGER.ALLOCATOR.Deallocate(temp);
}

void Animation::SetFrameRate(double FPS) {
	m_frameRate = FPS;
}

void Animation::SetTotalLength(double length) {
	m_frameRate = m_nFrames / length;
}

void Animation::GotoLocation(double location) {
	m_currentLocation = location;
}

void Animation::Update() {
	if ((m_playMode & PM_PLAY) != 0) {
		if ((m_playMode & PM_LOOP) != 0) {
			m_currentLocation += TIMER.GetFrameDuration();
			if (m_currentLocation > GetEnd()) m_currentLocation = 0;
		}
		else if ((m_playMode & PM_PING_PONG) != 0) {
			if (!m_reverse) {
				m_currentLocation += TIMER.GetFrameDuration();
				if (m_currentLocation >= GetEnd()) m_reverse = true;
			}
			else {
				m_currentLocation -= TIMER.GetFrameDuration();
				if (m_currentLocation <= 0) m_reverse = false;
			}
		}
		else {
			m_currentLocation += TIMER.GetFrameDuration();
		}
	}
	else {
		if ((m_playMode & PM_LOOP) != 0) {
			m_currentLocation += TIMER.GetFrameDuration();
			if (m_currentLocation > GetEnd()) m_currentLocation = 0;
		}
		else if ((m_playMode & PM_PING_PONG) != 0) {
			if (m_reverse) {
				m_currentLocation += TIMER.GetFrameDuration();
				if (m_currentLocation >= GetEnd()) m_reverse = false;
			}
			else {
				m_currentLocation -= TIMER.GetFrameDuration();
				if (m_currentLocation <= 0) m_reverse = true;
			}
		}
		else {
			m_currentLocation -= TIMER.GetFrameDuration();
		}
	}

	m_currentLocation = fmin(fmax(m_currentLocation, 0.0), GetEnd());
}

void Animation::Free() {
	if (m_images > 0) {
		DRAWING_MANAGER.ALLOCATOR.Deallocate(m_images);
		m_images = 0;
	}
}
