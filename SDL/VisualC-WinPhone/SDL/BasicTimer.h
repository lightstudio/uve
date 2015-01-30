#pragma once

#include <wrl.h>

// 用于基本计时的帮助器类。
ref class BasicTimer sealed
{
public:
	// 初始化内部计时器值。
	BasicTimer()
	{
		if (!QueryPerformanceFrequency(&m_frequency))
		{
			throw ref new Platform::FailureException();
		}
		Reset();
	}
	
	// 将计时器重置为初始值。
	void Reset()
	{
		Update();
		m_startTime = m_currentTime;
		m_total = 0.0f;
		m_delta = 1.0f / 60.0f;
	}
	
	// 更新计时器的内部值。
	void Update()
	{
		if (!QueryPerformanceCounter(&m_currentTime))
		{
			throw ref new Platform::FailureException();
		}
		
		m_total = static_cast<float>(
			static_cast<double>(m_currentTime.QuadPart - m_startTime.QuadPart) /
			static_cast<double>(m_frequency.QuadPart)
			);
		
		if (m_lastTime.QuadPart == m_startTime.QuadPart)
		{
			// 如果仅重置了计时器，则将报告等效于 60Hz 帧时间的时间增量。
			m_delta = 1.0f / 60.0f;
		}
		else
		{
			m_delta = static_cast<float>(
				static_cast<double>(m_currentTime.QuadPart - m_lastTime.QuadPart) /
				static_cast<double>(m_frequency.QuadPart)
				);
		}
		
		m_lastTime = m_currentTime;
	}
	
	// 在最后一次调用 Reset()与最后一次调用 Update()之间的持续时间(秒)。
	property float Total
	{
		float get() { return m_total; }
	}
	
	// 在对 Update()的前两次调用之间的持续时间(秒)。
	property float Delta
	{
		float get() { return m_delta; }
	}

private:
	LARGE_INTEGER m_frequency;
	LARGE_INTEGER m_currentTime;
	LARGE_INTEGER m_startTime;
	LARGE_INTEGER m_lastTime;
	float m_total;
	float m_delta;
};
