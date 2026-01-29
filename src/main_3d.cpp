#include "3d/gui/OpenGLApplication3D.h"
#include "tests/TestRunner3D.h"
#include <string>
#include <algorithm>

int main(int argc, char** argv) {
	// By default run automated 3D tests. Pass `--app` to start the interactive app.
	bool startApp = false;
	for (int i = 1; i < argc; ++i) {
		std::string a = argv[i];
		if (a == "--app") startApp = true;
	}

	if (startApp) {
		OpenGLApplication3D app(1024, 768);
		return app.run();
	}

	TestRunner3D runner(1024, 768);
	runner.runAll("output_3d_tests");
	return 0;
}
