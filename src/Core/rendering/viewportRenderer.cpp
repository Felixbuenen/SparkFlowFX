#include "viewportRenderer.h"

#include <cstdint>
#include <string>
#include <type_traits>
#include <algorithm>

#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>

#include "camera.h"
#include "log.h"
#include "simulation.h"

namespace sfx::rendering {

    // shader code
    namespace {

        static_assert(std::is_same_v<GLuint, unsigned int>);

        constexpr GLint viewProjLocation = 0;

        constexpr GLuint positionAttrib = 0;
        constexpr GLuint posBindingIndex = 0;
        constexpr GLuint aliveAttrib = 1;
        constexpr GLuint aliveBindingIndex = 1;

        constexpr const char* vertexSource = R"(#version 460 core
layout(location = 0) in vec3 aPosition;
layout(location = 1) in uint aActive;
layout(location = 0) uniform mat4 uViewProj;

void main()
{
    if(aActive == 0u) {
        gl_Position = vec4(2.0, 2.0, 2.0, 1.0);
        return;
    };
    gl_Position = uViewProj * vec4(aPosition, 1.0);
    gl_PointSize = 1.0;
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
                std::string infoLog(static_cast<size_t>(length), '\0');
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
                std::string infoLog(static_cast<size_t>(length), '\0');
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
        glDeleteBuffers(1, &m_posBuffer);
        glDeleteBuffers(1, &m_aliveBuffer);
        glDeleteProgram(m_program);
    }

    bool ViewportRenderer::initialize(GLProcLoader loader, size_t initParticleCapacity)
    {
        if (gladLoadGL(loader) == 0)
        {
            SFX_LOG_ERROR("Failed to load OpenGL functions");
            return false;
        }

        if (initParticleCapacity == 0)
        {
            SFX_LOG_ERROR("Cannot initialize renderer with a capacity of 0");
            return false;
        }

        m_capacity = initParticleCapacity;

        // compile, link and cleanup shaders
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

        glCreateVertexArrays(1, &m_vao);

        glEnableVertexArrayAttrib(m_vao, positionAttrib);
        glVertexArrayAttribFormat(m_vao, positionAttrib, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribBinding(m_vao, positionAttrib, posBindingIndex);

        glEnableVertexArrayAttrib(m_vao, aliveAttrib);
        glVertexArrayAttribIFormat(m_vao, aliveAttrib, 1, GL_UNSIGNED_BYTE, 0);
        glVertexArrayAttribBinding(m_vao, aliveAttrib, aliveBindingIndex);

        if (initParticleCapacity > 0)
        {
            resizeParticleBuffer(initParticleCapacity);
        }

        return true;
    }

    void ViewportRenderer::render(const simulation::ParticleView& particles, const Camera& camera)
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        const size_t count = particles.positions.size();
        if (count == 0) return;

        if (count > m_capacity)
        {
            resizeParticleBuffer(std::max(count, 2 * m_capacity));
        }

        // copy particle GPU buffer data
        glNamedBufferSubData(m_posBuffer, 0, static_cast<GLsizeiptr>(particles.positions.size_bytes()), particles.positions.data());
        glNamedBufferSubData(m_aliveBuffer, 0, static_cast<GLsizeiptr>(particles.active.size_bytes()), particles.active.data());

        // set camera
        const glm::mat4 viewProj = camera.viewProjection();
        glProgramUniformMatrix4fv(m_program, viewProjLocation, 1, GL_FALSE, glm::value_ptr(viewProj));

        glEnable(GL_PROGRAM_POINT_SIZE);
        glUseProgram(m_program);
        glBindVertexArray(m_vao);
        glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(count));
    }

    void ViewportRenderer::resizeParticleBuffer(size_t capacity)
    {
        glDeleteBuffers(1, &m_posBuffer);
        glDeleteBuffers(1, &m_aliveBuffer);

        glCreateBuffers(1, &m_posBuffer);
        glNamedBufferStorage(m_posBuffer, static_cast<GLsizeiptr>(capacity * sizeof(glm::vec3)), nullptr, GL_DYNAMIC_STORAGE_BIT);
        glCreateBuffers(1, &m_aliveBuffer);
        glNamedBufferStorage(m_aliveBuffer, static_cast<GLsizeiptr>(capacity * sizeof(std::uint8_t)), nullptr, GL_DYNAMIC_STORAGE_BIT);

        glVertexArrayVertexBuffer(m_vao, posBindingIndex, m_posBuffer, 0, sizeof(glm::vec3));
        glVertexArrayVertexBuffer(m_vao, aliveBindingIndex, m_aliveBuffer, 0, sizeof(std::uint8_t));

        m_capacity = capacity;
    }


} // namespace sfx::rendering