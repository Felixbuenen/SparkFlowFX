#include "simulation.h"

#include <iostream>

namespace sparkflow::simulation {

    Simulator::Simulator(SimulationConfig config, SimulationData data)
        : m_config{ config }, m_simData{ data }
    {
    }

    void Simulator::init()
    {

    }

    void Simulator::step(float dt)
    {
        std::cout << "hello world\n";

        // For now a basic simulation:
        
        // first kill
        updateAges(dt);
        kill(dt);

        emitNew(dt);
        solveForces(dt);
        updatePositions(dt);
    }

    // OPERATORS

    void Simulator::emitNew(float dt)
    {
        // TODO: for emit X particles per second at the origin and give them a random impulse force
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