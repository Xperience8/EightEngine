#pragma once

class Timer
{
public:
	Timer() : m_BaseTime(0), m_PrevTime(0), m_CurrentTime(0), m_DeltaTime(0.0)  
	{ 
		__int64 CountPerSec;
		QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&CountPerSec));
		m_SecPerCount = 1.0 / CountPerSec;
	}
	~Timer() { }

	// Reseting BaseTime to current time value -> it is used for computing whole app running time
	// Set previous time and current time with base time value
	void Reset()
	{
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_BaseTime));
		m_CurrentTime = m_PrevTime = m_BaseTime;
	}
	// Save new time and compute delta time(time of previous frame - new time)
	void Update()
	{
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_CurrentTime));
		m_DeltaTime = (m_CurrentTime - m_PrevTime) * m_SecPerCount;
		m_PrevTime = m_CurrentTime;
	}

	// Delta time is time of previous frame - time of current frame
	float GetDeltaTime() const { return static_cast<float>(m_DeltaTime); }
private:
	__int64 m_BaseTime;
	__int64 m_PrevTime;
	__int64 m_CurrentTime;
	double m_DeltaTime;
	double m_SecPerCount;
};