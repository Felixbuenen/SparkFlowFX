#pragma once

#include <cstdint>
#include <span>
#include <vector>

#include <glm/vec3.hpp>

#include "random.h"

namespace sfx::simulation {

    struct SimulationConfig
    {
        // FPS is the real-time playback rate, substeps the number of simulation steps per frame.
        std::uint32_t framesPerSecond{ 24 };
        std::uint32_t substeps{ 1 };

        std::size_t capacity{ 2'000 };

        // temporary emitter settings
        float emitRateSec{ 10.0f };
        float minSpeed{ 1.0f };
        float maxSpeed{ 5.0f };
        float lifespanSec{ 4.0f };
        std::uint32_t seed{ 1 };

        // temporary forces
        glm::vec3 gravity{ 0.0f, -9.81f, 0.0f };
        float drag{ 0.5f };
    };

    struct SimulationData
    {
        explicit SimulationData(std::size_t capacity)
            : active(capacity, false)
            , positions(capacity)
            , velocities(capacity)
            , forces(capacity)
            , ages(capacity)
            , lifespans(capacity)
        {
        }

        // TODO: should probably become array?
        std::vector<std::uint8_t> active;
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> velocities;
        std::vector<glm::vec3> forces;
        std::vector<float> ages;
        std::vector<float> lifespans;

        std::size_t aliveCount{ 0 };
        std::size_t activeEnd{ 0 };
    };

    // Temporary minimal view used by the renderer.
    struct ParticleView
    {
        std::span<const glm::vec3> positions;
        std::span<const std::uint8_t> active;
    };

    class Simulator
    {
    public:
        explicit Simulator(SimulationConfig config = {});

        void advanceFrame();

        std::size_t frame() const noexcept
        {
            return m_frame;
        }

        const SimulationConfig& config() const noexcept
        {
            return m_config;
        }

        // TODO: temporary particle getter, to be refactored.
        ParticleView particlesView() const noexcept
        {
            const std::size_t count = m_simData.activeEnd;
            return {
                std::span{ m_simData.positions }.first(count),
                std::span{ m_simData.active }.first(count)
            };
        }

    private:
        void substep(float dt);

        // operators
        void emitNew(float dt);
        void solveForces() noexcept;
        void updateVelocities(float dt) noexcept;
        void updatePositions(float dt) noexcept;
        void updateAges(float dt);
        void kill();

        // utility
        void killParticle(std::size_t idx);

    private:
        SimulationConfig m_config;
        random::Engine m_rng;
        std::size_t m_frame{ 0 };
        float m_emitBudget{ 0.0f };

        SimulationData m_simData;
    };

} // namespace sfx::simulation