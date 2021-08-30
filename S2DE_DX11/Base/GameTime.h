#pragma once
#include "Base/Main/Common.h"

#include <chrono>
#include <time.h>
#include <sys/timeb.h>

namespace S2DE
{
	class S2DE_DX11_API GameTime
	{
	public:
		GameTime();
		~GameTime();

		float			GetTime()		const;
		float			GetDeltaTime()  const;
		std::int32_t	GetFPS()		const;

		void	Tick();

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