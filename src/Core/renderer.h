#pragma once

namespace sparkflow::render {
	using GLProc = void (*)();
	using GLProcLoader = GLProc(*)(const char*);

	bool LoadGL(GLProcLoader loader);
	void Render();
}