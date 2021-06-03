#include "TimerManager.hpp"

TimerHandler::TimerHandler() {

}

void TimerHandler::update(float dt) {
	for (uint8_t i = 0; i < timers.size(); i++) {
		if (timer_states[i] == TimerState::RUNNING) {
			timers[i] += dt;
		}
	}
}

uint8_t TimerHandler::add_timer() {
	timers.push_back(0.0f);
	timer_states.push_back(TimerState::RUNNING);
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

TimerState TimerHandler::get_timer_state(uint8_t id) {
	if (id < timers.size()) {
		return timer_states[id];
	}
	else {
		return TimerState::UNKNOWN;
	}
}

void TimerHandler::set_timer_state(uint8_t id, TimerState state) {
	if (id < timers.size()) {
		timer_states[id] = state;
	}
}