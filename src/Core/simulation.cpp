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

    void Simulator::init()
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
        
        updateAges(dt);
        kill(dt);
        emitNew(dt);
        solveForces(dt);
        updatePositions(dt);
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

            // look for empty spot
            if (data.active[idx])
                continue;

            // random speed and initial direction
            const float speed = std::lerp(m_config.minSpeed, m_config.maxSpeed, random::uniform01(m_rng));
            const vec3 dir = random::randomUnitVector(m_rng);

            const vec3 initialVel = speed * dir;

            data.active[idx] = true;
            data.positions[idx] = { 0.0f, 0.0f, 0.0f };
            data.velocities[idx] = { dir.x * speed, dir.y * speed, dir.z * speed };
            data.ages[idx] = 0.0f;
            data.lifespans[idx] = m_config.lifespanSec;

            ++data.aliveCount;
            m_emitBudget -= 1.0f;

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
        // TODO: use the simulation step to calculate the new age
    }

    void Simulator::kill(float dt)
    {
        // TODO: kill any particles that are past 
    }

}