#include "SDL.h"

#include <vector>
#include <utility>

typedef std::pair<uint8_t, float> AnimationFrame;
typedef std::vector<AnimationFrame> AnimationFrames;

class Entity {
public:
	Entity();
	Entity(float x, float y, AnimationFrames animation_frames);

	void update(float dt);

	void render();

protected:
	float x, y;
	float x_vel, y_vel;

	AnimationFrames animation_frames;

	uint8_t current_frame;
	float animation_timer;
};