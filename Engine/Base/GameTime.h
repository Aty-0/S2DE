#pragma once
#include "Base/Main/Common.h"

#include <chrono>
#include <time.h>
#include <sys/timeb.h>

namespace S2DE::Core
{
	typedef std::chrono::seconds		se;
	typedef std::chrono::milliseconds	ms;
	typedef std::chrono::microseconds	us;
	typedef std::chrono::nanoseconds	ns;
	typedef std::chrono::steady_clock::time_point timePoint;

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
		float						  m_timer;
		std::chrono::duration<double> m_timerDuration;
		float						  m_deltaTime;
		std::int32_t				  m_fps;
		std::int32_t				  m_frameCount;
		// On start tick
		timePoint					  m_start;
		// Saved previous tick
		timePoint					  m_then;

		timePoint					  m_tickEnd;
	};
}