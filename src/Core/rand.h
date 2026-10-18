#pragma once

#include <cmath>
#include <concepts>
#include <cstdint>
#include <numbers>
#include <random>

#include "glm/vec3.hpp"

// One of the features of SparkFlowFX is determinism: the same seed must produce the same particles
// on every compiler and OS. That requires two things:
//   1. An engine whose algorithm is fully specified by the standard (Engine).
//   2. Distributions implemented here instead of taken from <random>, because
//      the standard library's distributions may differ between implementations.
namespace sparkflow::random {

    // The std::mt19937 standard fully specifies its output sequence, unlike std::default_random_engine
    // (implementation-defined) or std::random_device (non-deterministic).
    using Engine = std::mt19937;

    // uniform01() keeps the top 24 bits of each value, so the generator must cover
    // the full range [0, 2^32 - 1].
    template <typename G>
    concept Uniform32BitGenerator =
        std::uniform_random_bit_generator<G> &&
        G::min() == 0 && G::max() == 0xFFFF'FFFFu;

    float uniform01(Uniform32BitGenerator auto& rng)
    {
        return static_cast<float>(static_cast<std::uint32_t>(rng()) >> 8) * 0x1p-24f;
    }

    glm::vec3 randomUnitVector(Uniform32BitGenerator auto& rng)
    {
        const float z = 2.0f * uniform01(rng) - 1.0f;
        const float phi = 2.0f * std::numbers::pi_v<float> *uniform01(rng);
        const float r = std::sqrt(1.0f - z * z);
        return { r * std::cos(phi), r * std::sin(phi), z };
    }

}