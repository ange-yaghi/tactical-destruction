#ifndef TIMING_H
#define TIMING_H

#include <intrin.h>

class Timer
{

public:

    unsigned m_frameNumber;

    unsigned m_lastFrameTimestamp;
    unsigned m_lastFrameDuration;

    unsigned long m_lastFrameClockstamp;
    unsigned long m_lastFrameClockTicks;

    bool m_isPaused;

    double m_averageFrameDuration;
    float m_fps;

    void Update();
    void Initialize();

	double GetFrameDuration();

	bool IsAtNextFrame(double FPS);

    unsigned GetTime();
    unsigned long GetClock();

};

extern Timer TIMER;

#endif