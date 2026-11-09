#include "viewportRenderer.h"

#include <array>
#include <string>
#include <type_traits>

#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>

#include "camera.h"
#include "log.h"
#include "simulation.h"

namespace sfx::rendering {

    namespace {

        static_assert(std::is_same_v<GLuint, unsigned int>);

        constexpr GLint viewProjLocation = 0;

        constexpr const char* vertexSource = R"(#version 460 core
layout(location = 0) in vec3 aPosition;
layout(location = 0) uniform mat4 uViewProj;

void main()
{
    gl_Position = uViewProj * vec4(aPosition, 1.0);
    gl_PointSize = 8.0;
}
)";

        constexpr const char* fragmentSource = R"(#version 460 core
out vec4 fragColor;

void main()
{
    fragColor = vec4(0.8, 0.8, 0.8, 1.0);
}
)";

        GLuint compileShader(GLenum type, const char* source)
        {
            const GLuint shader = glCreateShader(type);
            glShaderSource(shader, 1, &source, nullptr);
            glCompileShader(shader);

            GLint status = GL_FALSE;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
            if (status != GL_TRUE)
            {
                GLint length = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
                std::string infoLog(static_cast<std::size_t>(length), '\0');
                glGetShaderInfoLog(shader, length, nullptr, infoLog.data());
                SFX_LOG_ERROR("Shader compilation failed: {}", infoLog);

                glDeleteShader(shader);
                return 0;
            }
            return shader;
        }

        GLuint linkProgram(GLuint vertexShader, GLuint fragmentShader)
        {
            const GLuint program = glCreateProgram();
            glAttachShader(program, vertexShader);
            glAttachShader(program, fragmentShader);
            glLinkProgram(program);
            glDetachShader(program, vertexShader);
            glDetachShader(program, fragmentShader);

            GLint status = GL_FALSE;
            glGetProgramiv(program, GL_LINK_STATUS, &status);
            if (status != GL_TRUE)
            {
                GLint length = 0;
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
                std::string infoLog(static_cast<std::size_t>(length), '\0');
                glGetProgramInfoLog(program, length, nullptr, infoLog.data());
                SFX_LOG_ERROR("Program link failed: {}", infoLog);

                glDeleteProgram(program);
                return 0;
            }
            return program;
        }

    } // namespace

    ViewportRenderer::~ViewportRenderer()
    {
        // GL functions are only loaded once initialize() succeeded
        if (m_program == 0)
        {
            return;
        }

        glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(1, &m_vbo);
        glDeleteProgram(m_program);
    }

    bool ViewportRenderer::initialize(GLProcLoader loader)
    {
        if (gladLoadGL(loader) == 0)
        {
            SFX_LOG_ERROR("Failed to load OpenGL functions");
            return false;
        }

        const GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
        const GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
        if (vertexShader != 0 && fragmentShader != 0)
        {
            m_program = linkProgram(vertexShader, fragmentShader);
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        if (m_program == 0)
        {
            return false;
        }

        const std::array<glm::vec3, 4> testPoints{ {
            { 0.0f, 0.0f,  0.0f },
            { 1.0f, 0.0f,  0.0f },
            { 0.0f, 1.0f,  0.0f },
            { 0.0f, 0.0f, -3.0f },
        } };
        m_pointCount = static_cast<int>(testPoints.size());

        glCreateBuffers(1, &m_vbo);
        glNamedBufferStorage(m_vbo, sizeof(testPoints), testPoints.data(), 0);

        constexpr GLuint positionAttrib = 0;
        constexpr GLuint bindingIndex = 0;

        glCreateVertexArrays(1, &m_vao);
        glVertexArrayVertexBuffer(m_vao, bindingIndex, m_vbo, 0, sizeof(glm::vec3));
        glEnableVertexArrayAttrib(m_vao, positionAttrib);
        glVertexArrayAttribFormat(m_vao, positionAttrib, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribBinding(m_vao, positionAttrib, bindingIndex);

        return true;
    }

    void ViewportRenderer::render([[maybe_unused]] const simulation::ParticleView& particles, const Camera& camera)
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        const glm::mat4 viewProj = camera.viewProjection();
        glProgramUniformMatrix4fv(m_program, viewProjLocation, 1, GL_FALSE, glm::value_ptr(viewProj));

        glEnable(GL_PROGRAM_POINT_SIZE);
        glUseProgram(m_program);
        glBindVertexArray(m_vao);
        glDrawArrays(GL_POINTS, 0, m_pointCount);
    }

} // namespace sfx::rendering