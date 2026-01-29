#include "2d/gui/OpenGLApplication2D.h"
#include "tests/TestRunner2D.h"
#include <string>
#include <algorithm>

int main(int argc, char** argv) {
	// By default run the automated test suite and exit. Pass `--app` to start the interactive app.
	bool startApp = false;
	for (int i = 1; i < argc; ++i) {
		std::string a = argv[i];
		if (a == "--app") startApp = true;
	}

	if (startApp) {
		OpenGLApplication2D app(1024, 768);
		return app.run();
	}

	TestRunner2D runner(1024, 768);
	runner.runAll("output_2d_tests");
	return 0;
}
