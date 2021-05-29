#include "TimerManager.hpp"

TimerHandler::TimerHandler() {

}

void TimerHandler::update(float dt) {
	for (uint8_t i = 0; i < timers.size(); i++) {
		timers[i] += dt;
	}
}

uint8_t TimerHandler::add_timer() {
	timers.push_back(0.0f);
	return timers.size() - 1;
}

float TimerHandler::get_timer(uint8_t id) {
	if (id < timers.size()) {
		return timers[id];
	}
	else {
		return 0.0f;
	}
}

void TimerHandler::reset_timer(uint8_t id) {
	if (id < timers.size()) {
		timers[id] = 0.0f;
	}
}