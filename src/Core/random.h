#pragma once

#include <cmath>
#include <cstdint>
#include <numbers>
#include <random>

#include <glm/vec3.hpp>

// Seeded random numbers that are identical on every compiler and OS for the same seed.
namespace sfx::random {

    // Its output sequence is fully specified by the standard.
    using Engine = std::mt19937;

    // Generators covering the full range [0, 2^32 - 1], which uniform01() relies on by keeping the top 24 bits.
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

} // namespace sfx::random