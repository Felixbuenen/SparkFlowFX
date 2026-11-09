#pragma once

namespace sparkflow::simulation {
    struct ParticleView;
}

namespace sparkflow::render {

    class Camera;

    class ViewportRenderer
    {
    public:
        using GLProc = void (*)();
        using GLProcLoader = GLProc(*)(const char*);

        ViewportRenderer() = default;
        ~ViewportRenderer();

        ViewportRenderer(const ViewportRenderer&) = delete;
        ViewportRenderer& operator=(const ViewportRenderer&) = delete;

        bool initialize(GLProcLoader loader);
        void render(const simulation::ParticleView& particles, const Camera& camera);

    private:
        // TEMP OpenGL buffers for minimal renderer
        unsigned int m_program{ 0 };
        unsigned int m_vao{ 0 };
        unsigned int m_vbo{ 0 };
        int m_pointCount{ 0 };
    };
}