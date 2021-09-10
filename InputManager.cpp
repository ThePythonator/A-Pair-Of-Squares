#include "InputManager.hpp"

bool KeyHandler::is_up(KeyState key_state) {
	return key_state == KeyState::STILL_UP || key_state == KeyState::JUST_RELEASED;
}

bool KeyHandler::is_down(KeyState key_state) {
	return key_state == KeyState::STILL_DOWN || key_state == KeyState::JUST_PRESSED;
}

bool KeyHandler::just_up(KeyState key_state) {
	return key_state == KeyState::JUST_RELEASED;
}

bool KeyHandler::just_down(KeyState key_state) {
	return key_state == KeyState::JUST_PRESSED;
}

void KeyHandler::update(KeyUnion& key_union) {
	for (uint8_t i = 0; i < KEY_COUNT; i++) {
		if (key_union.key_array[i] == KeyState::JUST_PRESSED) {
			key_union.key_array[i] = KeyState::STILL_DOWN;
		}
		else if (key_union.key_array[i] == KeyState::JUST_RELEASED) {
			key_union.key_array[i] = KeyState::STILL_UP;
		}
	}
}

void KeyHandler::update_key(KeyUnion& key_handler, const SDL_Event& sdl_event) {
	// Check to make sure key hasn't been auto-repeated by being held down
	if (!sdl_event.key.repeat) {

		// Is key pressed or released?
		KeyState value = sdl_event.type == SDL_KEYDOWN ? KeyState::JUST_PRESSED : KeyState::JUST_RELEASED;

		// Check which key is pressed
		switch (sdl_event.key.keysym.sym) {
		case SDLK_0:
			key_handler.keys.NUM_0 = value;
			break;
		case SDLK_1:
			key_handler.keys.NUM_1 = value;
			break;
		case SDLK_2:
			key_handler.keys.NUM_2 = value;
			break;
		case SDLK_3:
			key_handler.keys.NUM_3 = value;
			break;
		case SDLK_4:
			key_handler.keys.NUM_4 = value;
			break;
		case SDLK_5:
			key_handler.keys.NUM_5 = value;
			break;
		case SDLK_6:
			key_handler.keys.NUM_6 = value;
			break;
		case SDLK_7:
			key_handler.keys.NUM_7 = value;
			break;
		case SDLK_8:
			key_handler.keys.NUM_8 = value;
			break;
		case SDLK_9:
			key_handler.keys.NUM_9 = value;
			break;
		case SDLK_a:
			key_handler.keys.A = value;
			break;
		case SDLK_b:
			key_handler.keys.B = value;
			break;
		case SDLK_c:
			key_handler.keys.C = value;
			break;
		case SDLK_d:
			key_handler.keys.D = value;
			break;
		case SDLK_e:
			key_handler.keys.E = value;
			break;
		case SDLK_f:
			key_handler.keys.F = value;
			break;
		case SDLK_g:
			key_handler.keys.H = value;
			break;
		case SDLK_h:
			key_handler.keys.H = value;
			break;
		case SDLK_i:
			key_handler.keys.I = value;
			break;
		case SDLK_j:
			key_handler.keys.J = value;
			break;
		case SDLK_k:
			key_handler.keys.K = value;
			break;
		case SDLK_l:
			key_handler.keys.L = value;
			break;
		case SDLK_m:
			key_handler.keys.M = value;
			break;
		case SDLK_n:
			key_handler.keys.N = value;
			break;
		case SDLK_o:
			key_handler.keys.O = value;
			break;
		case SDLK_p:
			key_handler.keys.P = value;
			break;
		case SDLK_q:
			key_handler.keys.Q = value;
			break;
		case SDLK_r:
			key_handler.keys.R = value;
			break;
		case SDLK_s:
			key_handler.keys.S = value;
			break;
		case SDLK_t:
			key_handler.keys.T = value;
			break;
		case SDLK_u:
			key_handler.keys.U = value;
			break;
		case SDLK_v:
			key_handler.keys.V = value;
			break;
		case SDLK_w:
			key_handler.keys.W = value;
			break;
		case SDLK_x:
			key_handler.keys.X = value;
			break;
		case SDLK_y:
			key_handler.keys.Y = value;
			break;
		case SDLK_z:
			key_handler.keys.Z = value;
			break;
		case SDLK_SPACE:
			key_handler.keys.SPACE = value;
			break;
		case SDLK_RETURN:
			key_handler.keys.RETURN = value;
			break;
		case SDLK_ESCAPE:
			key_handler.keys.ESCAPE = value;
			break;
		case SDLK_UP:
			key_handler.keys.UP = value;
			break;
		case SDLK_DOWN:
			key_handler.keys.DOWN = value;
			break;
		case SDLK_LEFT:
			key_handler.keys.LEFT = value;
			break;
		case SDLK_RIGHT:
			key_handler.keys.RIGHT = value;
			break;
		default:
			break;
		}
	}
}


void KeyHandler::set_key(KeyUnion& key_handler, Key key, KeyState key_state) {
	key_handler.key_array[(uint8_t)key] = key_state;
}



InputHandler::InputHandler() {
	key_data = { KeyHandler::KeyState::STILL_UP };
}

void InputHandler::update() {
	KeyHandler::update(key_data);
}

void InputHandler::handle_sdl_event(const SDL_Event& sdl_event) {
	if (sdl_event.type == SDL_KEYDOWN || sdl_event.type == SDL_KEYUP) {
		KeyHandler::update_key(key_data, sdl_event);
	}
}

void InputHandler::set_key(KeyHandler::Key key, KeyHandler::KeyState key_state) {
	KeyHandler::set_key(key_data, key, key_state);
}

KeyHandler::KeyUnion InputHandler::get_key_union() {
	return key_data;
}