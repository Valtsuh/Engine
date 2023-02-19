#define ENGINE_VERSION_MINOR				0
#define ENGINE_VERSION_LESSER				0
#define ENGINE_VERSION_LARGE				1
#define ENGINE_VERSION_MAJOR				0


//Modify accordingly
#include "../../assets/engine.graphics.h"

using namespace drx::gfx;

struct ENGINE : drx::RUNNER {
	void Load() override {
		// Load stuff

	}

	void Draw() override {
		// Paint
		Clear();
		drx::tg::WRITER::_type("Hello, world!", 25, 25, 3, 1, W);
		drx::tg::WRITER::_type("Hello again.", 25, 50, 2, 2, W, true);
	}

	void Update() override {
		// Do stuff

	}

};

ENGINE engine;



int main(HINSTANCE hinst) {
	// X, Y, instance, name, fullscreen, width, height, show console, seed random
	engine.Setup(10, 10, hinst, "Engine base");
	engine.Run();

	return 0;
};
