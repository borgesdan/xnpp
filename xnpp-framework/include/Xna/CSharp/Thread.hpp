#ifndef CSHARP_THREAD_HPP
#define CSHARP_THREAD_HPP

#include <chrono>
#include <thread>

namespace Xna::CSharp {
	class Thread {
	public:
		inline static void Sleep_d(double millisecondsTimeout) {
			std::this_thread::sleep_for(std::chrono::seconds(static_cast<long long>(millisecondsTimeout)));
		}

		static inline void Sleep(long long millisecondsTimeout) {
			std::this_thread::sleep_for(std::chrono::seconds(millisecondsTimeout));
		}
	};
}

#endif