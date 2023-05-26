#pragma once
#include "Base/Main/Common.h"
#include <chrono>

namespace S2DE::Core
{
	class S2DE_API GameTime : public Core::Utils::Singleton<GameTime>
	{
	public:
		GameTime();
		~GameTime();

		void Begin();
		void End();

		[[nodiscard]] inline float GetTime()		const;
		[[nodiscard]] inline float GetDeltaTime()  const;
		[[nodiscard]] inline std::int32_t GetFPS()		const;
		[[nodiscard]] inline std::int32_t GetFrameCount()		const;

	private:
		float						  m_timer;
		std::chrono::duration<double> m_timerDuration;
		float						  m_deltaTime;
		std::int32_t				  m_fps;
		std::int32_t				  m_frameCount;
		
		std::chrono::steady_clock::time_point m_now;
		std::chrono::steady_clock::time_point m_then;
		std::chrono::steady_clock::time_point m_tickEnd;
	};
}