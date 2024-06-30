#pragma once
#include <chrono>

namespace U::Engine::Util {
	class Timer {
		std::chrono::system_clock::time_point _prevFrameTime = std::chrono::system_clock::now();
		std::chrono::system_clock::time_point _timeSinceStart = std::chrono::system_clock::now();
		
	public:

		float msSinceStart = 0;
		float msDeltaTime = 1 / 60.f;

		inline void start() noexcept {
			_prevFrameTime = std::chrono::system_clock::now();
		}

		inline void tick() noexcept {
			std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
			msDeltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - _prevFrameTime).count() / 1000.0f;
			msSinceStart = std::chrono::duration_cast<std::chrono::microseconds>(now - _timeSinceStart).count() / 1000.f;
			_prevFrameTime = now;
		}
	};

	extern Timer GlobalTimer;
}