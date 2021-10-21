#include "GameTime.h"
#include "Base/Utils/Logger.h"


namespace S2DE
{
	GameTime::GameTime() :
		m_time(std::chrono::high_resolution_clock::now()),
		m_time_begin(std::chrono::high_resolution_clock::now()),
		m_fps(0),
		m_frame_count(0),
		m_deltaTime(0.0f),
		m_timer(0.0f)
	{

	}

	GameTime::~GameTime()
	{

	}

	void GameTime::Tick()
	{
		m_frame_count++;

		m_time = std::chrono::high_resolution_clock::now();
		m_deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(m_time - m_time_lastUpdate).count() / 1000000.0f;



		m_timer_duration = m_time - m_time_begin;
		std::chrono::duration_cast<std::chrono::microseconds>(m_timer_duration);
		m_timer += m_timer_duration.count() / 10;

		if (m_time - m_time_begin >= std::chrono::seconds{ 1 })
		{
			m_fps = m_frame_count;
			m_time_begin = m_time;
			m_frame_count = 0;
		}

		m_time_lastUpdate = m_time;
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