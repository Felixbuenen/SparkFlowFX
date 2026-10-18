#include "simulation.h"

#include <iostream>
#include <cmath>
#include <numbers>

#include "rand.h"

namespace sparkflow::simulation {

    Simulator::Simulator(SimulationConfig config, SimulationData data)
        : m_config{ config }
        , m_simData{ std::move(data) }
        , m_rng{ config.seed }
    {
    }

    void Simulator::advanceFrame()
    {
        const float dt = 1.0f / static_cast<float>(m_config.framesPerSecond * m_config.substeps);

        for (std::uint32_t i = 0; i < m_config.substeps; ++i)
            substep(dt);

        ++m_frame;
    }


    void Simulator::substep(float dt)
    {
        std::cout << "Do step: " << m_frame << "\n";
        
        // 13-10-26 next step: add integration/position updates. Then we can start drawing GL_POINTS.

        solveForces(dt);
        updateVelocities(dt);
        updatePositions(dt);
        updateAges(dt);
        kill();
        emitNew(dt);
    }

    // OPERATORS

    void Simulator::emitNew(float dt)
    {
        m_emitBudget += m_config.emitRateSec * dt;

        auto& data = m_simData;
        const std::size_t capacity = data.positions.size();

        for (std::size_t idx = 0; m_emitBudget >= 1.0f; ++idx)
        {
            // TODO: resize buffers
            if (idx >= capacity)
            {
                break;
            }

            //std::cout << "emit: look for empty spot " << idx << std::endl;
            // look for empty spot
            if (data.active[idx])
                continue;

            // random speed and initial direction
            const float speed = std::lerp(m_config.minSpeed, m_config.maxSpeed, random::uniform01(m_rng));
            const vec3 dir = random::randomUnitVector(m_rng);

            const vec3 initialVel = speed * dir;

            //std::cout << "emit: create new data " << idx << std::endl;

            data.active[idx] = true;
            data.positions[idx] = { 0.0f, 0.0f, 0.0f };
            data.velocities[idx] = { dir.x * speed, dir.y * speed, dir.z * speed };
            data.ages[idx] = 0.0f;
            data.lifespans[idx] = m_config.lifespanSec;

            ++data.aliveCount;
            m_emitBudget -= 1.0f;
            data.activeEnd = std::max(data.activeEnd, idx + 1);

            std::cout << "Particle " << idx << " emitted!" << std::endl;
        }

        // Pool full.
        // TODO: resize. Currently, we discard any particles that we would've emitted this
        //  step. We still keep the fraction so that we keep the emission flow.
        m_emitBudget -= std::floor(m_emitBudget);
    }

    void Simulator::solveForces(float dt)
    {
        // TODO: for now, apply a simple dampening and gravity force
    }

    void Simulator::updateVelocities(float dt)
    {
        // TODO: based on the forces, calculate the new velocities
    }

    void Simulator::updatePositions(float dt)
    {
        // TODO: based on the velocities, update the positions
    }

    void Simulator::updateAges(float dt)
    {
        auto& data = m_simData;

        for (size_t idx = 0; idx < data.activeEnd; ++idx)
        {
            //std::cout << "updateAges: check active " << idx << std::endl;
            // only active agents
            if (!data.active[idx])
                continue;

            //std::cout << "updateAges: update age " << idx << std::endl;
            data.ages[idx] += dt;
        }
    }

    void Simulator::kill()
    {
        auto& data = m_simData;
        for (std::size_t idx = 0; idx < data.activeEnd; ++idx)
        {
            if (data.active[idx] && data.ages[idx] >= data.lifespans[idx])
                killParticle(idx);
        }

        // update active end
        while (data.activeEnd > 0 && !data.active[data.activeEnd - 1])
            --data.activeEnd;
    }

    void Simulator::killParticle(size_t idx)
    {
        //std::cout << "killParticle: trying to kill " << idx << std::endl;
        auto& data = m_simData;
        data.aliveCount--;
        data.active[idx] = false;

        std::cout << "Particle " << idx << " killed!" << std::endl;
    }


}