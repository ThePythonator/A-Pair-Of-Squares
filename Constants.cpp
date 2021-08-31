#include "Constants.hpp"

// Constants

namespace SPRITES {
	const uint8_t SCALE = 2;

	const uint8_t TEXT_SCALE = 4;

	const uint8_t SIZE = 16;
	const uint8_t SIZE_HALF = SIZE / 2;

	const uint8_t SPACE_WIDTH = 1;

	const uint8_t TEXT_OFFSET_X = SIZE * 4;// 5 or 6 work too, but text looks too wide when beziering

	namespace ID {
		const uint16_t BLUE_SQUARE = 0;
		const uin16_t PINK_SQUARE = 4;

		const uint16_t SQUARE_PARTICLE = 228;

		const uint16_t FINISH_PARTICLE_BLUE = 230;
	}
}

namespace WINDOW {
	const uint16_t WIDTH = 960;
	const uint16_t HEIGHT = 640; //540

	// Levels should be 20x13 max if scale == 3

	const uint16_t SCALED_WIDTH = WIDTH / SPRITES::SCALE;
	const uint16_t SCALED_HEIGHT = HEIGHT / SPRITES::SCALE;

	const uint16_t SCALED_WIDTH_HALF = SCALED_WIDTH / 2;
	const uint16_t SCALED_HEIGHT_HALF = SCALED_HEIGHT / 2;

	const uint16_t TEXT_SCALED_WIDTH = WIDTH / SPRITES::TEXT_SCALE;
	const uint16_t TEXT_SCALED_HEIGHT = HEIGHT / SPRITES::TEXT_SCALE;

	const uint16_t TEXT_SCALED_WIDTH_HALF = TEXT_SCALED_WIDTH / 2;
	const uint16_t TEXT_SCALED_HEIGHT_HALF = TEXT_SCALED_HEIGHT / 2;

	const uint8_t FPS = 120;
	//const float MIN_DT = 1.0f / FPS;

	//const float CAMERA_MOVEMENT_RATIO = 0.95f;
}

namespace GAME {
	namespace SCORE {
		const uint16_t INITIAL = 1000;
		const uint8_t DEATH = 30;
		const uint8_t ORB = 10;
		const uint8_t TIME = 1;
	}

	namespace ENVIRONMENT {
		namespace GRAVITY {
			const float ACCELERATION = 600.0f;
			const float MAX = 250.0f;
		}
	}

	namespace ORB {
		namespace FADE {
			const float MAX_SCALE_BOOST = 4.0f;
			const float MAX_AGE = 0.3f;
		}
	}

	namespace SQUARE {
		const float ACCELERATION = 500.0f;
		const float DECELERATION = 400.0f;
		const float VELOCITY_MAX = 120.0f;

		const float IDLE_VELOCITY_MIN = 40.0f;

		const float JUMP_STRENGTH = 210.0f;

		namespace FADE {
			const float MAX_SCALE_BOOST = 2.0f;
			const float MAX_AGE = 0.25f;
		}

		namespace BLINK {
			/*const float FRAME_0 = 0.3f;
			const float FRAME_1 = 0.3f;*/

			const uint8_t FRAME_COUNT = 2;

			const float FRAME_DURATIONS[FRAME_COUNT] = { 0.1f, 0.05f };

			const float DURATION = FRAME_DURATIONS[0] + FRAME_DURATIONS[1];

			const float DELAY_MIN = 1.5f;
			const float DELAY_MAX = 4.0f;

			//const AnimationFrames SQUARE_BLINK{ AnimationFrame{BLINK_FRAME_0, BLINK_FRAME_0_LENGTH}, AnimationFrame{BLINK_FRAME_1, BLINK_FRAME_1_LENGTH} };
		}
	}

	namespace FINISH {
		const float PULL_VELOCITY = 0.06f;//0.9f;//0.05f;//0.05f;// 0.5f;
		const float PULL_MIN_VELOCITY = 0.2f;
		const float MIN_DISTANCE = 1.0f;
	}
}

namespace MENU {
	const uint8_t FLOAT_TEXT_PRECISION = 2;

	const uint8_t LEVELS_PER_ROW = 4;
	const uint8_t LEVEL_ROWS = GAME::LEVEL_COUNT / LEVELS_PER_ROW;

	namespace SHAPE_PARTICLE {
		const uint8_t COUNT = 7;
		const uint8_t CHANNELS = 4;
	}
}

namespace COLOURS {
	const Colour BLACK = Colour(0x04, 0x07, 0x10);
	const Colour WHITE = Colour(0xFF, 0xFF, 0xE4);

	const Colour SELECTED = Colour(0x1C, 0x92, 0xA7);
}

namespace STRINGS {
	const std::string APP_TITLE = "A Pair of Squares";

	const std::string ON = "On";
	const std::string OFF = "Off";

	const std::string COLON_SPACE = ": ";

	namespace MENU {
		namespace TITLE {
			const std::string OPTION_PLAY = "Play";
			const std::string OPTION_SETTINGS = "Settings";
			const std::string OPTION_QUIT = "Quit";
		}

		namespace SETTINGS {
			const std::string OPTION_MUSIC = "Music";
			const std::string OPTION_SFX = "SFX";
			const std::string OPTION_BACK = "Back";
		}

		namespace LEVEL_SELECT {
			const std::string OPTION_BACK = "Back";
		}

		namespace LEVEL_PAUSED {
			const std::string OPTION_RESUME = "Resume";
			const std::string OPTION_EXIT = "Exit";
		}

		namespace LEVEL_COMPLETED {
			const std::string TEXT_TIME_TAKEN = "Time taken";
			const std::string TEXT_ORBS_COLLECTED = "Orbs collected";
			const std::string TEXT_NUMBER_OF_DEATHS = "Number of deaths";
			const std::string TEXT_SCORE = "Score";
			const std::string OPTION_CONTINUE = "Continue";
		}
	}
}

namespace FILES {
	const std::string SPRITESHEET = "spritesheet.png";

	const std::string FONT_SHEET = "font.png";//"another-font.png"
	// const std::string FONT_SHEET = "font.png";
}

namespace DELAY {
	const float TRANSITION_FADE_LENGTH = 0.6f;

	const float MENU_INTRO_LENGTH = 2.0f;
	const float MENU_INTRO_FADE_LENGTH = 1.0f;

	const float MENU_BEZIER_LENGTH = 1.0f;

	const float MENU_SHAPE_GENERATION = 3.0f;

	const float GAME_FINISH_PARTICLE_GENERATION = 0.9f;
}

// Timer IDs (are set later in program)
namespace TIMER_ID {
	const uint8_t UNINITIALISED = 255;

	uint8_t INTRO_LENGTH = UNINITIALISED;

	uint8_t MENU_TRANSITION_FADE = UNINITIALISED;
	uint8_t MENU_BEZIER_TEXT = UNINITIALISED;
	uint8_t MENU_SHAPE_GENERATION = UNINITIALISED;

	uint8_t GAME_FINISH_PARTICLE_GENERATION = UNINITIALISED;
	uint8_t GAME_DURATION = UNINITIALISED;
}

// Nodes for bezier transitions
namespace BEZIER {
	const uint8_t MENU_BEZIER_NODE_COUNT = 3;

	// 1st Node is doubled to make text transitions ease in/out more smoothly
	const std::vector<Node> FROM_LEFT{ Node{WINDOW::TEXT_SCALED_WIDTH_HALF, 0}, Node{WINDOW::TEXT_SCALED_WIDTH_HALF, 0}, Node{WINDOW::TEXT_SCALED_WIDTH, 0}, Node{-WINDOW::TEXT_SCALED_WIDTH, 0} };
	const std::vector<Node> FROM_RIGHT{ Node{WINDOW::TEXT_SCALED_WIDTH_HALF, 0}, Node{WINDOW::TEXT_SCALED_WIDTH_HALF, 0}, Node{0, 0}, Node{WINDOW::TEXT_SCALED_WIDTH * 2, 0} };

	float bezier_x(std::vector<Node> nodes, float ratio) {
		return bezier_curve(nodes, ratio).first;
	}
}