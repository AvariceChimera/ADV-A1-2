#pragma once
#include "GameEntity.h"
#include "Timer.h"

using namespace SDLFramework;

class Formation : public GameEntity {
public:
	Formation();
	~Formation();

	void Update() override;

	int GetTick();

	Vector2 GridSize();

	bool Locked();
	void Lock();

private:
	Timer* mTimer;

	// side to side while they fly in
	float mOffsetAmount;
	float mOffsetTimer;
	float mOffsetDelay;

	int mOffsetCounter;
	int mOffsetDirection;

	// pulse when formation is locked
	float mPulseTimer;
	float mPulseDelay;

	int mPulseCount;
	int mPulseDirection;

	Vector2 mGridSize;

	bool mLocked;
};