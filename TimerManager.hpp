#pragma once

#include <vector>

class TimerHandler {
public:
	TimerHandler();

	void update(float dt);

	uint8_t add_timer();
	float get_timer(uint8_t id);
	void reset_timer(uint8_t id);

private:
	std::vector<float> timers;
};