#pragma once

class GameTime
{
public:
	GameTime() : m_BaseTime(0), m_CurrentTime(0), m_PrevTime(0), m_GameTimeScale(1.f),
		m_RealDeltaTime(0.f), m_GameDeltaTime(0.f), m_Stopped(false)
	{ 
		__int64 clockPerSec;
		QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&clockPerSec));
		m_SecPerClock = 1.0 / clockPerSec;
	}
	~GameTime() { }

	// Starts clock
	void Reset()
	{
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_BaseTime));
		m_CurrentTime = m_PrevTime = m_BaseTime;
		m_RealDeltaTime = m_GameDeltaTime = 0.f;
	}

	void Stop()
	{
		m_Stopped = true;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_StopTime));

		m_GameDeltaTime = 0;
	}
	void Continue()
	{
		m_Stopped = false;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_CurrentTime));
		
		m_BaseTime += m_CurrentTime - m_StopTime;
		m_CurrentTime = m_PrevTime;
	}

	void Tick()
	{
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_CurrentTime));
		m_RealDeltaTime = static_cast<float>((m_CurrentTime - m_PrevTime) * m_SecPerClock);
		m_PrevTime = m_CurrentTime;
		
		if (!m_Stopped)
		{
			m_GameDeltaTime = m_RealDeltaTime * m_GameTimeScale;
		}
	}


	float GetRealTime() const
	{
		return m_RealDeltaTime;
	}
	float GetDeltaTime() const
	{
		return m_GameDeltaTime;
	}
	float GetTimeFromStart() const
	{
		__int64 deltaTime = m_Stopped ? m_StopTime - m_BaseTime : m_CurrentTime - m_BaseTime;
		return static_cast<float>(deltaTime * m_SecPerClock) * m_GameTimeScale;
	}



private:
	// Thay are used to compute elapsed time from game start
	__int64 m_BaseTime;
	__int64 m_StopTime;

	// Computing elapsed time between two frames
	__int64 m_CurrentTime;
	__int64 m_PrevTime;

	// Used to converting from cpu cycles to seconds
	double m_SecPerClock;
	
	// Scales game time
	// 1.f -> real time
	// 2.f -> double speed
	// 0.5f -> half speed
	float m_GameTimeScale;

	float m_RealDeltaTime;
	float m_GameDeltaTime;

	bool m_Stopped;
};