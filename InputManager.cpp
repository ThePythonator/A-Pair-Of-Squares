#include "InputManager.hpp"

bool KeyHandler::is_up(const KeyState& key_state) {
	return key_state == KeyState::STILL_UP || key_state == KeyState::JUST_RELEASED;
}

bool KeyHandler::is_down(const KeyState& key_state) {
	return key_state == KeyState::STILL_DOWN || key_state == KeyState::JUST_PRESSED;
}

bool KeyHandler::just_up(const KeyState& key_state) {
	return key_state == KeyState::JUST_RELEASED;
}

bool KeyHandler::just_down(const KeyState& key_state) {
	return key_state == KeyState::JUST_PRESSED;
}

void KeyHandler::update(KeyUnion& key_union) {
	for (uint8_t i = 0; i < KEY_COUNT; i++) {
		if (key_union.key_array[i] == KeyState::JUST_PRESSED) {
			key_union.key_array[i] == KeyState::STILL_DOWN;
		}
		else if (key_union.key_array[i] == KeyState::JUST_RELEASED) {
			key_union.key_array[i] == KeyState::STILL_UP;
		}
	}
}

void KeyHandler::update_key(KeyUnion& key_handler, const SDL_Event& sdl_event) {
	if (sdl_event.type == SDL_KEYDOWN) {
		// User has pressed a key

		// Check which key is pressed
		switch (sdl_event.key.keysym.sym) {
		case SDLK_0:
			key_handler.keys.NUM_0 = KeyState::JUST_PRESSED;
			break;
		case SDLK_1:
			key_handler.keys.NUM_1 = KeyState::JUST_PRESSED;
			break;
		case SDLK_2:
			key_handler.keys.NUM_2 = KeyState::JUST_PRESSED;
			break;
		case SDLK_3:
			key_handler.keys.NUM_3 = KeyState::JUST_PRESSED;
			break;
		case SDLK_4:
			key_handler.keys.NUM_4 = KeyState::JUST_PRESSED;
			break;
		case SDLK_5:
			key_handler.keys.NUM_5 = KeyState::JUST_PRESSED;
			break;
		case SDLK_6:
			key_handler.keys.NUM_6 = KeyState::JUST_PRESSED;
			break;
		case SDLK_7:
			key_handler.keys.NUM_7 = KeyState::JUST_PRESSED;
			break;
		case SDLK_8:
			key_handler.keys.NUM_8 = KeyState::JUST_PRESSED;
			break;
		case SDLK_9:
			key_handler.keys.NUM_9 = KeyState::JUST_PRESSED;
			break;
		case SDLK_a:
			key_handler.keys.A = KeyState::JUST_PRESSED;
			break;
		case SDLK_b:
			key_handler.keys.B = KeyState::JUST_PRESSED;
			break;
		case SDLK_c:
			key_handler.keys.C = KeyState::JUST_PRESSED;
			break;
			// TODO: finish
		default:
			break;
		}
	}
	else if (sdl_event.type == SDL_KEYUP) {
		// User has released a key

		// Check which key is pressed
		switch (sdl_event.key.keysym.sym) {
			// TODO: finish - can copy/paste from SDL_KEYDOWN and alter all JUST_PRESSED to JUST_RELEASED (find and replace all in selection)
		default:
			break;
		}
	}
}