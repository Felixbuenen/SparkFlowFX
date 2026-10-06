#pragma once

#include <chrono>

#include <vec3.h>

namespace sparkflow::simulation {

	using sparkflow::core::vec3;

	struct SimulationState
	{
		std::size_t step{ 0 };
	};

	struct SimulationConfig
	{
		std::chrono::milliseconds updateInterval{ 100 };
		std::size_t capacity{ 1'000 };
	};

	struct SimulationData
	{
		SimulationData(std::size_t capacity = 10)
			: positions(capacity)
			, velocities(capacity)
			, ages(capacity)
		{
		}

		std::vector<bool> active;
		std::vector<vec3> positions;
		std::vector<vec3> velocities;
		std::vector<float> ages;
		std::vector<float> lifespans;
		std::size_t aliveCount{ 0 };
	};

	class Simulator
	{
	public:
		explicit Simulator(SimulationConfig config = {}, SimulationData data = {});

		void init();
		void step(float dt);

		const SimulationConfig& config() const noexcept { return m_config; }

	private:
		void emitNew(float dt);
		void solveForces(float dt);
		void updateVelocities(float dt);
		void updatePositions(float dt);
		void updateAges(float dt);
		void kill(float dt);

	private:
		SimulationConfig m_config;

		// PARTICLE DATA
		SimulationData m_simData;
	};

}