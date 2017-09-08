#include "Time.h"
#include "core/platform/Platform.h"
#include <stdio.h>



RE::Time::Time() {

}

RE::Time::~Time() {

}

void RE::Time::Update() {
	double curTime = Clock();
	elapse = curTime - _lastTime;
	time = curTime - _startTime;
}

void RE::Time::ResetElapse() {
	elapse = 0.0;
	_lastTime = Clock();
}

double RE::Time::Elapse() {
	return Clock() - _lastTime;
}

void RE::Time::Print(const char* msg /*= ""*/) {
	Log("%s%.6f", msg, Clock() - _lastTime);
}

double RE::Time::Clock() {
	timeval tmval;
	gettimeofday(&tmval, NULL);
	return tmval.tv_sec + tmval.tv_usec*0.000001;
}
