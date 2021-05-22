#pragma once

#include "SDL.h"

#include <utility>

//enum class Keys {
//	UP, DOWN, LEFT, RIGHT,
//	A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
//	N1, N2, N3, N4, N5, N6, N7, N8, N9, N0,
//	SPACE, RETURN, SHIFT, TAB, ESCAPE
//};


//typedef std::pair<Keys, KeyState> KeyData;

namespace KeyHandler {
	enum class KeyState {
		STILL_UP, // Key isn't pressed (has already been up for at least one frame)
		JUST_RELEASED, // Key has just been released
		STILL_DOWN, // Key is pressed (has already been down for at least one frame)
		JUST_PRESSED // Key has been pressed that frame
	};

	struct Keys {
		KeyState UP, DOWN, LEFT, RIGHT,
			A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
			NUM_0, NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9,
			SPACE, RETURN, SHIFT, TAB, ESCAPE;
	};

	const uint8_t KEY_COUNT = sizeof(Keys) / sizeof(KeyState);

	union KeyUnion {
		Keys keys;
		KeyState key_array[KEY_COUNT];
	};

	bool is_up(const KeyState& key_state); // Returns true if STILL_UP or JUST_RELEASED
	bool is_down(const KeyState& key_state); // Returns true if STILL_DOWN or JUST_PRESSED
	bool just_up(const KeyState& key_state); // Returns true if JUST_RELEASED
	bool just_down(const KeyState& key_state); // Returns true if JUST_PRESSED

	void update(KeyUnion& key_handler);

	void update_key(KeyUnion& key_handler, const SDL_Event& sdl_event);
}