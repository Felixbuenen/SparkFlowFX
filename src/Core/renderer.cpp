#include "renderer.h"
#include <glad/gl.h>

namespace sparkflow::render {

    bool InitializeGL(GLProcLoader loader)
    {
        return gladLoadGL(loader) != 0;
    }

    void Render()
    {
        glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

}