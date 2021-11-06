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

	extern const uint8_t TEXT_LOCKED_ALPHA;
}

namespace TILE_ID {
	namespace PLAYER {
		extern const uint16_t BLUE;
		extern const uint16_t PINK;
	}

	namespace SPIKE {
		extern const uint16_t DOUBLE_BOTTOM;
		extern const uint16_t DOUBLE_LEFT;
		extern const uint16_t DOUBLE_TOP;
		extern const uint16_t DOUBLE_RIGHT;

		extern const uint16_t SINGLE_BOTTOM_LEFT;
		extern const uint16_t SINGLE_BOTTOM_RIGHT;

		extern const uint16_t SINGLE_LEFT_TOP;
		extern const uint16_t SINGLE_LEFT_BOTTOM;

		extern const uint16_t SINGLE_TOP_LEFT;
		extern const uint16_t SINGLE_TOP_RIGHT;

		extern const uint16_t SINGLE_RIGHT_TOP;
		extern const uint16_t SINGLE_RIGHT_BOTTOM;
	}

	namespace FINISH {
		extern const uint16_t BLUE;
		extern const uint16_t PINK;
	}

	namespace ORB {
		extern const uint16_t BLUE;
		extern const uint16_t PINK;
	}

	namespace PARTICLE {
		extern const uint16_t SQUARE_DARK;
		extern const uint16_t SQUARE_LIGHT;

		extern const uint16_t FINISH_BLUE;
		extern const uint16_t FINISH_PINK;
	}


	namespace SPRING {
		extern const uint16_t BASE_FRAME;
	}

	namespace BUTTON {
		extern const uint16_t BLUE_RELEASED;
		extern const uint16_t BLUE_PRESSED;
		extern const uint16_t PINK_RELEASED;
		extern const uint16_t PINK_PRESSED;
	}

	namespace DOOR {
		namespace VERTICAL {
			extern const uint16_t BLUE_TOP;
			extern const uint16_t BLUE_MIDDLE;
			extern const uint16_t BLUE_BOTTOM;

			extern const uint16_t PINK_TOP;
			extern const uint16_t PINK_MIDDLE;
			extern const uint16_t PINK_BOTTOM;
		}

		namespace HORIZONTAL {
			extern const uint16_t BLUE_LEFT;
			extern const uint16_t BLUE_MIDDLE;
			extern const uint16_t BLUE_RIGHT;

			extern const uint16_t PINK_LEFT;
			extern const uint16_t PINK_MIDDLE;
			extern const uint16_t PINK_RIGHT;
		}
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

	extern const uint8_t MAX_FPS;
	//extern const float MIN_DT = 1.0f / FPS;

	//extern const float CAMERA_MOVEMENT_RATIO = 0.95f;
	
	extern const float MAX_DT;
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

		extern const uint8_t BORDER;
		extern const uint8_t WIDTH;
	}

	namespace FINISH {
		extern const uint8_t HEIGHT;

		extern const uint8_t BORDER;
		extern const uint8_t WIDTH;
	}

	namespace SPRING {
		extern const uint8_t FRAME_COUNT;

		extern const AnimationFrame ANIMATION[];

		extern const uint8_t ANIMATION_HEIGHTS[];

		extern const uint8_t LAUNCH_FRAME;

		extern const uint8_t BORDER;
		extern const uint8_t WIDTH;


		extern const float LAUNCH_VELOCITY;

		extern const float MAXIMUM_COLLISION_RESOLUTION_Y_VEL;

		extern const uint8_t FRAME_HEIGHT_DIFFERENCE;

		//extern const float ANIMATION_DURATION;
	}

	namespace BUTTON {
		extern const uint8_t HEIGHT_RELEASED;
		extern const uint8_t HEIGHT_PRESSED;

		extern const uint8_t BORDER;
		extern const uint8_t WIDTH;

		extern const float RELEASE_DELAY;
	}

	namespace SQUARE {
		extern const float ACCELERATION;
		extern const float DECELERATION;
		extern const float VELOCITY_MAX;

		extern const float IDLE_VELOCITY_MIN;

		extern const float JUMP_STRENGTH;

		extern const float MAXIMUM_STEP_HEIGHT;

		extern const uint8_t WIDTH;
		extern const uint8_t HEIGHT; // Only used for a few collisions because I added this pretty late in development

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

	namespace DOOR {
		extern const uint8_t OPEN_SIZE;
		extern const uint8_t CLOSE_SIZE;

		extern const uint8_t BORDER;

		extern const uint8_t WIDTH;

		extern const float OPEN_SPEED;
		extern const float CLOSE_SPEED;

		extern const uint8_t PLAYER_ALLOWANCE;
	}

	namespace SPIKE {
		extern const uint8_t TOLERANCE;
	}
}

namespace MENU {
	extern const uint8_t FLOAT_TEXT_PRECISION;

	extern const uint8_t LEVELS_PER_ROW;
	extern const uint8_t LEVEL_ROWS;

	extern const uint8_t PAUSED_BACKGROUND_ALPHA;

	namespace SHAPE_PARTICLE {
		extern const uint8_t COUNT;
		extern const uint8_t CHANNELS;
	}
}

namespace COLOURS {
	extern const Colour BLACK;
	extern const Colour WHITE;

	//extern const Colour GREEN;

	extern const Colour TRUE_WHITE;

	extern const Colour SELECTED;
}

namespace STRINGS {
	extern const std::string APP_TITLE;

	extern const std::string ON;
	extern const std::string OFF;

	extern const std::string COLON_SPACE;

	namespace MENU {
		namespace TITLE {
			extern const std::string HEADING_BLUE;
			extern const std::string HEADING_PINK;

			extern const std::string OPTION_PLAY;
			extern const std::string OPTION_SETTINGS;
			extern const std::string OPTION_QUIT;
		}

		namespace SETTINGS {
			extern const std::string HEADING;

			extern const std::string OPTION_MUSIC;
			extern const std::string OPTION_SFX;
			extern const std::string OPTION_BACK;
		}

		namespace LEVEL_SELECT {
			extern const std::string HEADING;

			extern const std::string OPTION_BACK;
		}

		namespace LEVEL_PAUSED {
			extern const std::string HEADING;

			extern const std::string OPTION_RESUME;
			extern const std::string OPTION_RESTART;
			extern const std::string OPTION_EXIT;
		}

		namespace LEVEL_COMPLETED {
			extern const std::string HEADING;

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
	extern const std::string TITLE_BLUE_FONT_SHEET;
	extern const std::string TITLE_PINK_FONT_SHEET;

	extern const std::string WINDOW_ICON;

	// extern const std::string FONT_SHEET = "font.png";

	namespace AUDIO {
		extern const std::string INTRO;

		extern const std::vector<std::string> GAME_TRACKS;

		extern const std::vector<std::string> SFX_SAMPLES;
	}
}

namespace DELAY {
	extern const float TRANSITION_FADE_LENGTH;

	extern const float MENU_INTRO_LENGTH;
	extern const float MENU_INTRO_FADE_LENGTH;

	extern const float MENU_BEZIER_LENGTH;

	extern const float MENU_SHAPE_GENERATION;

	extern const float GAME_FINISH_PARTICLE_GENERATION;

	extern const float PAUSE_FADE_LENGTH;
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

namespace AUDIO {
	extern const int DEFAULT_FREQUENCY;
	extern const uint8_t DEFAULT_CHANNELS;
	extern const int DEFAULT_CHUNKSIZE;

	extern const float SOUND_VOLUME;
	extern const float MUSIC_VOLUME;

	extern const float MUSIC_FADE_TIME;

	enum SFX { // Should it be enum class???
		JUMP,
		SPRING,
		ORB,
		DEATH,
		SELECT,
		DOOR,
		BUTTON
	};
}

// Nodes for bezier transitions
namespace BEZIER {
	extern const uint8_t MENU_BEZIER_NODE_COUNT;

	// 1st Node is doubled to make text transitions ease in/out more smoothly
	extern const std::vector<Node> FROM_LEFT;
	extern const std::vector<Node> FROM_RIGHT;

	float bezier_x(std::vector<Node> nodes, float ratio);
}