#pragma once

#include <random>
#include <functional>
#include <variant>

#define MY_PI 3.14159265358979323846

inline auto randomBool = std::bind(std::uniform_int_distribution<>(0, 1), std::default_random_engine());

inline float getRandomFloat(float min, float max)
{
	float random = ((float)rand()) / (float)RAND_MAX;

	// generate (in your case) a float between 0 and (4.5-.78)
	// then add .78, giving you a float between .78 and 4.5
	float range = max - min;
	return (random * range) + min;
}