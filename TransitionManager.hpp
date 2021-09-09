#pragma once

#include "SDL.h"

enum class TransitionState {
	OPENING,
	OPEN,
	CLOSING,
	CLOSED
};

class TransitionHandler {
public:
	TransitionHandler();
	TransitionHandler(SDL_Renderer* renderer);

	void update(float dt);
	void render(Spritesheet& spritesheet);

	uint8_t set_transition(void (*transition_update) (TransitionState* transition_state, float* timer, float dt), void (*transition_render) (SDLRenderer* renderer, Spritesheet& spritesheet, float timer));

	void open();
	void close();

	bool is_open();
	bool is_closed();

private:
	void (*transition_update) (TransitionState* transition_state, float* timer, float dt) = nullptr;
	void (*transition_render) (TransitionState* transition_state, float* timer, SDLRenderer* renderer, Spritesheet& spritesheet) = nullptr;

	TransitionState transition_state = TransitionState::OPEN;

	SDL_Renderer* renderer = nullptr;
};