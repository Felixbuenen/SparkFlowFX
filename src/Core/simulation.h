#pragma once

#include <chrono>
#include <cstdint>
#include <random>
#include <vector>

#include <vec3.h>
#include "rand.h"

namespace sparkflow::simulation {

	using sparkflow::core::vec3;

	struct SimulationConfig
	{
		// FPS is the real-time playback, substeps are how many simulation steps are taken per frame.
		std::uint32_t framesPerSecond{ 24 };
		std::uint32_t substeps{ 1 };

		std::size_t capacity{ 1'000 };

		// TEMP particle simulation config
		float emitRateSec{ 5.0f };
		float minSpeed{ 1.0f };
		float maxSpeed{ 5.0f };
		float lifespanSec{ 2.0f };
		std::uint32_t seed{ 1 };
	};

	struct SimulationData
	{
		SimulationData(std::size_t capacity = 1'000)
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
		std::vector<vec3> positions;
		std::vector<vec3> velocities;
		std::vector<vec3> forces;
		std::vector<float> masses;
		std::vector<float> ages;
		std::vector<float> lifespans;
		std::size_t aliveCount{ 0 };
	};

	class Simulator
	{
	public:
		explicit Simulator(SimulationConfig config = {}, SimulationData data = {});

		void init();
		void advanceFrame();

		std::size_t frame() const noexcept { return m_frame; }
		const SimulationConfig& config() const noexcept { return m_config; }

	private:
		void substep(float dt);

		// operators
		void emitNew(float dt);
		void solveForces(float dt);
		void updateVelocities(float dt);
		void updatePositions(float dt);
		void updateAges(float dt);
		void kill(float dt);

	private:
		SimulationConfig m_config;
		random::Engine m_rng; // random 32-bit integers
		std::size_t m_frame{ 0 };
		float m_emitBudget{ 0.0f };

		// PARTICLE DATA
		SimulationData m_simData;
	};

}