#pragma once
#include <random>
#include <chrono>

class Random {
public:
	static Random* Instance();
	static void Release();

	unsigned RandomInt(); //return random # from 0 to the unsigned max
	float RandomFloat(); // from 0.0 to 1.0
	int RandomRange(int lo, int hi); // anything from lo to hi
	float RandomRange(float lo, float hi); //same as above but floats

private:
	static Random* sInstance;
	std::mt19937 mGenerator;

	Random();
	~Random();
};