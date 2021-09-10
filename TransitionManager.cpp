#include "TransitionManager.hpp"

TransitionHandler::TransitionHandler() {
	
}

TransitionHandler::TransitionHandler(SDL_Renderer* renderer) {
	this->renderer = renderer;
}

void TransitionHandler::update(float dt) {
	if (transition_update) {
		transition_update(&transition_state, &timer, dt);
	}
}

void TransitionHandler::render(Spritesheet& spritesheet) {
	if (transition_render && renderer) {
		transition_render(&transition_state, &timer, renderer, spritesheet);
	}
}

void TransitionHandler::set_transition(void (*transition_update) (TransitionState* transition_state, float* timer, float dt), void (*transition_render) (TransitionState* transition_state, float* timer, SDL_Renderer* renderer, Spritesheet& spritesheet)) {
	this->transition_update = transition_update;
	this->transition_render = transition_render;
}

void TransitionHandler::open() {
	timer = 0.0f;
	transition_state = TransitionState::OPENING;
}

void TransitionHandler::close() {
	timer = 0.0f;
	transition_state = TransitionState::CLOSING;
}

bool TransitionHandler::is_open() {
	return transition_state == TransitionState::OPEN;
}

bool TransitionHandler::is_closed() {
	return transition_state == TransitionState::CLOSED;
}