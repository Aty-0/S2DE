#include "GameTime.h"

namespace S2DE::Core
{
	GameTime::GameTime() :
		m_tickEnd(std::chrono::high_resolution_clock::now()),
		m_now(std::chrono::high_resolution_clock::now()),
		m_then(std::chrono::high_resolution_clock::now()),
		m_fps(0),
		m_frameCount(0),
		m_deltaTime(0.0f),
		m_timer(0.0f),
		m_timerDuration()
	{

	}

	GameTime::~GameTime()
	{
		m_fps = 0;
		m_frameCount = 0;
		m_deltaTime = 0.0f;
		m_timer = 0.0f;
	}

	void GameTime::Begin()
	{
		m_frameCount++;
		m_now = std::chrono::high_resolution_clock::now();
	}

	void GameTime::End()
	{
		m_deltaTime = (float)std::chrono::duration_cast<std::chrono::microseconds>( m_now - m_then).count() / 1000.0f;
		m_fps = (1.0f / m_deltaTime) * 1000.0f;
		m_then = m_now;

		m_timerDuration = m_now - m_tickEnd;
		m_timerDuration = std::chrono::duration_cast<std::chrono::microseconds>(m_timerDuration);
		
		m_timer += m_timerDuration.count() / 10;
		if (m_now - m_tickEnd >= std::chrono::seconds{ 1 })
		{
			//m_fps = m_frameCount;
			m_tickEnd = m_now;
			m_frameCount = 0;
		}

	}

	inline float GameTime::GetTime() const
	{
		return m_timer;
	}

	inline float GameTime::GetDeltaTime() const
	{
		return  m_deltaTime; 
	}

	inline std::int32_t GameTime::GetFrameCount() const
	{
		return m_frameCount;
	}

	inline std::int32_t GameTime::GetFPS() const
	{ 
		return m_fps;
	}
}