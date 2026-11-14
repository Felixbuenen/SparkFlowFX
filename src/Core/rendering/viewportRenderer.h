#pragma once

namespace sfx::simulation {
    struct ParticleView;
} // namespace sfx::simulation

namespace sfx::rendering {

    class Camera;

    class ViewportRenderer
    {
    public:
        using GLProc = void (*)();
        using GLProcLoader = GLProc (*)(const char*);

        ViewportRenderer() = default;
        ~ViewportRenderer();

        ViewportRenderer(const ViewportRenderer&) = delete;
        ViewportRenderer& operator=(const ViewportRenderer&) = delete;

        bool initialize(GLProcLoader loader, size_t initParticleCapacity = 1024);
        void render(const simulation::ParticleView& particles, const Camera& camera);

    private:
        void resizeParticleBuffer(size_t capacity);

    private:
        // temporary GL objects for the minimal renderer
        unsigned int m_program{ 0 };
        unsigned int m_vao{ 0 };
        unsigned int m_posBuffer{ 0 };
        unsigned int m_aliveBuffer{ 0 };
        size_t m_capacity{ 0 };
    };

} // namespace sfx::rendering