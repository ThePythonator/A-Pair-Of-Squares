#include "Spring.hpp"

Spring::Spring() : Entity() {

}

Spring::Spring(uint16_t sprite_index, float x, float y) : Entity(sprite_index, x, y) {

}

void Spring::update(float dt) {
	// launch_player is only set for 1 frame
	launch_player = false;

	animation_timer += dt;

	if (animation_timer >= GAME::SPRING::ANIMATION[current_frame_offset].second) {
		animation_timer -= GAME::SPRING::ANIMATION[current_frame_offset].second;

		current_frame_offset++;

		if (current_frame_offset == GAME::SPRING::FRAME_COUNT) {
			current_frame_offset = 0;
		}
		else if (current_frame_offset == GAME::SPRING::LAUNCH_FRAME) {
			// Launch player upwards (unfortunately happens next frame)
			launch_player = true;
		}
	}
}

void Spring::render(Spritesheet& spritesheet) {
	spritesheet.sprite_scaled(GAME::SPRING::ANIMATION[current_frame_offset].first, x, y);
}

bool Spring::should_launch() {
	return launch_player;
}

bool Spring::check_collision(float player_x, float player_y) {
	return player_x < x + GAME::SPRING::BORDER + GAME::SPRING::WIDTH && player_x + SPRITES::SIZE > x + GAME::SPRING::BORDER && player_y < y + SPRITES::SIZE && player_y + SPRITES::SIZE > get_top();
}

bool Spring::check_on_top(float player_x, float player_y){
	return player_x < x + GAME::SPRING::BORDER + GAME::SPRING::WIDTH && player_x + SPRITES::SIZE > x + GAME::SPRING::BORDER && player_y + SPRITES::SIZE == get_top();
}

float Spring::get_top() {
	return y + SPRITES::SIZE - GAME::SPRING::ANIMATION_HEIGHTS[current_frame_offset];
}