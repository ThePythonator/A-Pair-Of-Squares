#pragma once

#include <cstdint>
#include <vector>
#include <string>

#include "Colour.hpp"
#include "Bezier.hpp"
#include "Entity.hpp"

// Constants

namespace SPRITES {
	extern const uint8_t SCALE;

	extern const uint8_t TEXT_SCALE;

	extern const uint8_t SIZE;
	extern const uint8_t SIZE_HALF;

	extern const uint8_t SPACE_WIDTH;

	extern const uint8_t TEXT_OFFSET_X;// 5 or 6 work too, but text looks too wide when beziering

	namespace ID {
		extern const uint16_t BLUE_SQUARE;
		extern const uint16_t PINK_SQUARE;

		extern const uint16_t SQUARE_PARTICLE;

		extern const uint16_t FINISH_PARTICLE_BLUE;
	}
}

namespace WINDOW {
	extern const uint16_t WIDTH;
	extern const uint16_t HEIGHT;

	extern const uint16_t SCALED_WIDTH;
	extern const uint16_t SCALED_HEIGHT;

	extern const uint16_t SCALED_WIDTH_HALF;
	extern const uint16_t SCALED_HEIGHT_HALF;

	extern const uint16_t TEXT_SCALED_WIDTH;
	extern const uint16_t TEXT_SCALED_HEIGHT;

	extern const uint16_t TEXT_SCALED_WIDTH_HALF;
	extern const uint16_t TEXT_SCALED_HEIGHT_HALF;

	extern const uint8_t FPS;
	//extern const float MIN_DT = 1.0f / FPS;

	//extern const float CAMERA_MOVEMENT_RATIO = 0.95f;
}

namespace GAME {
	extern const uint8_t LEVEL_COUNT; // This one is declared in Game.cpp instead

	namespace SCORE {
		extern const uint16_t INITIAL;
		extern const uint8_t DEATH;
		extern const uint8_t ORB;
		extern const uint8_t TIME;
	}

	namespace ENVIRONMENT {
		namespace GRAVITY {
			extern const float ACCELERATION;
			extern const float MAX;
		}
	}

	namespace ORB {
		namespace FADE {
			extern const float MAX_SCALE_BOOST;
			extern const float MAX_AGE;
		}
	}

	namespace SQUARE {
		extern const float ACCELERATION;
		extern const float DECELERATION;
		extern const float VELOCITY_MAX;

		extern const float IDLE_VELOCITY_MIN;

		extern const float JUMP_STRENGTH;

		namespace FADE {
			extern const float MAX_SCALE_BOOST;
			extern const float MAX_AGE;
		}

		namespace BLINK {
			/*extern const float FRAME_0;
			extern const float FRAME_1;*/

			extern const uint8_t FRAME_COUNT;

			extern const float FRAME_DURATIONS[];

			extern const float DURATION;

			extern const float DELAY_MIN;
			extern const float DELAY_MAX;

			//extern const AnimationFrames SQUARE_BLINK;
		}
	}

	namespace FINISH {
		extern const float PULL_VELOCITY;
		extern const float PULL_MIN_VELOCITY;
		extern const float MIN_DISTANCE;
	}
}

namespace MENU {
	extern const uint8_t FLOAT_TEXT_PRECISION;

	extern const uint8_t LEVELS_PER_ROW;
	extern const uint8_t LEVEL_ROWS;

	namespace SHAPE_PARTICLE {
		extern const uint8_t COUNT;
		extern const uint8_t CHANNELS;
	}
}

namespace COLOURS {
	extern const Colour BLACK;
	extern const Colour WHITE;

	extern const Colour SELECTED;
}

namespace STRINGS {
	extern const std::string APP_TITLE;

	extern const std::string ON;
	extern const std::string OFF;

	extern const std::string COLON_SPACE;

	namespace MENU {
		namespace TITLE {
			extern const std::string OPTION_PLAY;
			extern const std::string OPTION_SETTINGS;
			extern const std::string OPTION_QUIT;
		}

		namespace SETTINGS {
			extern const std::string OPTION_MUSIC;
			extern const std::string OPTION_SFX;
			extern const std::string OPTION_BACK;
		}

		namespace LEVEL_SELECT {
			extern const std::string OPTION_BACK;
		}

		namespace LEVEL_PAUSED {
			extern const std::string OPTION_RESUME;
			extern const std::string OPTION_EXIT;
		}

		namespace LEVEL_COMPLETED {
			extern const std::string TEXT_TIME_TAKEN;
			extern const std::string TEXT_ORBS_COLLECTED;
			extern const std::string TEXT_NUMBER_OF_DEATHS;
			extern const std::string TEXT_SCORE;
			extern const std::string OPTION_CONTINUE;
		}
	}
}

namespace FILES {
	extern const std::string SPRITESHEET;

	extern const std::string FONT_SHEET;
	// extern const std::string FONT_SHEET = "font.png";
}

namespace DELAY {
	extern const float TRANSITION_FADE_LENGTH;

	extern const float MENU_INTRO_LENGTH;
	extern const float MENU_INTRO_FADE_LENGTH;

	extern const float MENU_BEZIER_LENGTH;

	extern const float MENU_SHAPE_GENERATION;

	extern const float GAME_FINISH_PARTICLE_GENERATION;
}

// Timer IDs (are set later in program)
namespace TIMER_ID {
	extern const uint8_t UNINITIALISED;

	extern uint8_t INTRO_LENGTH;

	extern uint8_t MENU_TRANSITION_FADE;
	extern uint8_t MENU_BEZIER_TEXT;
	extern uint8_t MENU_SHAPE_GENERATION;

	extern uint8_t GAME_FINISH_PARTICLE_GENERATION;
	extern uint8_t GAME_DURATION;
}

// Nodes for bezier transitions
namespace BEZIER {
	extern const uint8_t MENU_BEZIER_NODE_COUNT;

	// 1st Node is doubled to make text transitions ease in/out more smoothly
	extern const std::vector<Node> FROM_LEFT;
	extern const std::vector<Node> FROM_RIGHT;

	float bezier_x(std::vector<Node> nodes, float ratio);
}