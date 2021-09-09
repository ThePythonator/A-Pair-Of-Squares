#pragma once

#include <vector>

#include "Utilities.hpp"

enum class TimerState : uint8_t {
	PAUSED,
	RUNNING,

	UNKNOWN
};

class TimerHandler {
public:
	TimerHandler();

	void update(float dt);

	uint8_t add_timer();
	float get_timer(uint8_t id);
	void reset_timer(uint8_t id);

	TimerState get_timer_state(uint8_t id);
	void set_timer_state(uint8_t id, TimerState state);

private:
	std::vector<float> timers;
	std::vector<TimerState> timer_states;
};