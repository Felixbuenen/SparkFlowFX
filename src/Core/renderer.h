#pragma once

namespace sparkflow::render {
	using GLProc = void (*)();
	using GLProcLoader = GLProc(*)(const char*);

	bool InitializeGL(GLProcLoader loader);
	void Render();
}