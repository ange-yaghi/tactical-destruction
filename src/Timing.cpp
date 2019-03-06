#include <timing.h>

#include <windows.h>
#include <mmsystem.h>

static bool qpcFlag;
static double qpcFrequency;

unsigned systemTime() {
    if(qpcFlag) {
        static LONGLONG qpcMillisPerTick;
        QueryPerformanceCounter((LARGE_INTEGER*)&qpcMillisPerTick);
        return (unsigned)(qpcMillisPerTick * qpcFrequency);
    }
	else {
		return unsigned(timeGetTime());
	}
}

unsigned Timer::GetTime() const {
    return systemTime();
}

inline unsigned __int64 SystemClock() {
    return __rdtsc();
}

unsigned long Timer::GetClock() const {
    return (unsigned long) SystemClock();
}

void Timer::Update() {
    if (!m_isPaused) m_frameNumber++;

    unsigned thisTime = GetTime();
    m_lastFrameDuration = thisTime - m_lastFrameTimestamp;
    m_lastFrameTimestamp = thisTime;

    unsigned long thisClock = GetClock();
    m_lastFrameClockTicks = thisClock - m_lastFrameClockstamp;
    m_lastFrameClockstamp = thisClock;

    if (m_frameNumber > 1) {
        if (m_averageFrameDuration <= 0) {
            m_averageFrameDuration = (double)m_lastFrameDuration;
        }
        else {
            m_averageFrameDuration *= 0.99;
            m_averageFrameDuration += 0.01 * (double)m_lastFrameDuration;

            m_fps = (float)(1000.0 / m_averageFrameDuration);
        }
    }
}

void Timer::Initialize() {
    LONGLONG time;

    qpcFlag = (QueryPerformanceFrequency((LARGE_INTEGER*)&time) > 0);
    if (qpcFlag) qpcFrequency = 1000.0 / time;

    m_frameNumber = 0;

    m_lastFrameTimestamp = GetTime();
    m_lastFrameDuration = 0;

    m_lastFrameClockstamp = GetClock();
    m_lastFrameClockTicks = 0;

    m_isPaused = false;

    m_averageFrameDuration = 0;
    m_fps = 0;
}

bool Timer::IsAtNextFrame(double FPS) const {
	double time = (GetTime() - m_lastFrameTimestamp) / 1000.0;

	if (time >= (1 / FPS)) return true;
	else return false;
}

double Timer::GetFrameDuration() const {
	return m_lastFrameDuration / 1000.0f;
}

Timer TIMER;
