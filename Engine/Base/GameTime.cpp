#include "GameTime.h"
#include "Base/Utils/Logger.h"


namespace S2DE::Core
{
	GameTime::GameTime() :
		m_tickEnd(std::chrono::high_resolution_clock::now()),
		m_fps(0),
		m_frameCount(0),
		m_deltaTime(0.0f),
		m_timer(0.0f)
	{

	}

	GameTime::~GameTime()
	{

	}

	void GameTime::Tick()
	{
		m_frameCount++;
		m_then = m_start;
		m_start = std::chrono::high_resolution_clock::now();
		m_deltaTime = std::chrono::duration_cast<us>(m_start - m_then).count() / 1000000.0f;

		m_timerDuration = m_start - m_tickEnd;
		m_timerDuration = std::chrono::duration_cast<us>(m_timerDuration);

		m_timer += m_timerDuration.count() / 10;
		if (m_start - m_tickEnd >= se{ 1 })
		{
			m_fps = m_frameCount;
			m_tickEnd = m_start;
			m_frameCount = 0;
		}

	}

	float GameTime::GetTime() const
	{
		return m_timer;
	}

	float GameTime::GetDeltaTime() const
	{ 
		return m_deltaTime;
	}

	std::int32_t GameTime::GetFPS() const
	{ 
		return m_fps;
	}
}