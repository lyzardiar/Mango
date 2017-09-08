#pragma once
#include <time.h>

namespace RE {
	class Time
	{
	public:
		Time();
		~Time();

	public:
		void Update();
		void ResetElapse();
		double Elapse();
		void Print(const char* msg = "");
	public:
		static double Clock();

	protected:

	public:
		double time = 0.0f;
		double elapse = 1.0f;

	private:
		double _lastTime = Clock();
		double _startTime = Clock();
	};
}