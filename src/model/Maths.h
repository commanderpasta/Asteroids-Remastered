#pragma once

#include <random>
#include <functional>
#include <variant>

#define MY_PI 3.14159265358979323846

/**
 * Returns a random true or false value when called.
 */
inline auto randomBool = std::bind(std::uniform_int_distribution<>(0, 1), std::default_random_engine());

/**
 * Returns a random float between a range of two values.
 * 
 * \param min The minimum value of the range.
 * \param max The maximum value of the range.
 * \return The resulting randomly selected value.
 */
inline float getRandomFloat(float min, float max)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float range = max - min;
	return (random * range) + min;
}