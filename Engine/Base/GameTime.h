#pragma once
#include "Base/Main/Common.h"

#include <chrono>
#include <time.h>
#include <sys/timeb.h>

namespace S2DE::Core
{
	class S2DE_API GameTime
	{
	public:
		GameTime();
		~GameTime();

		//Update game timer
		void				Tick();

		inline float		GetTime()		const;
		inline float		GetDeltaTime()  const;
		inline std::int32_t	GetFPS()		const;

	private:
		float m_timer;
		std::chrono::duration<float> m_timer_duration;
		std::int32_t	m_fps;
		std::int32_t m_frame_count;
		float m_deltaTime;
		std::chrono::steady_clock::time_point m_time_lastUpdate;
		std::chrono::steady_clock::time_point m_time;
		std::chrono::steady_clock::time_point m_time_begin;
	};
}