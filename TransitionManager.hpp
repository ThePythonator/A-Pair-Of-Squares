#pragma once

#include "SDL.h"

#include "Spritesheet.hpp"

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

	void set_transition(void (*transition_update) (TransitionState* transition_state, float* timer, float dt), void (*transition_render) (TransitionState* transition_state, float* timer, SDL_Renderer* renderer, Spritesheet& spritesheet));

	void open();
	void close();

	bool is_open();
	bool is_closed();

private:
	void (*transition_update) (TransitionState* transition_state, float* timer, float dt) = nullptr;
	void (*transition_render) (TransitionState* transition_state, float* timer, SDL_Renderer* renderer, Spritesheet& spritesheet) = nullptr;

	TransitionState transition_state = TransitionState::OPEN;

	SDL_Renderer* renderer = nullptr;

	float timer = 0.0f;
};