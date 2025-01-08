#include "Formation.h"

Formation::Formation() {
	mTimer = Timer::Instance();

	//tinker to make these smoother

	mOffsetAmount = 10.0f;
	mOffsetDelay = 0.1f;
	mOffsetTimer = 0.0f;
	mOffsetDirection = 1;
	mOffsetCounter = 4;

	mPulseTimer = 0.0f;
	mPulseDelay = 0.04f;
	mPulseCount = 0;
	mPulseDirection = 1;

	mLocked = false;

	mGridSize = Vector2(32.0f, 64.0f);
}

Formation::~Formation() {
	mTimer = nullptr;
}

Vector2 Formation::GridSize() {
	return mGridSize;
}

int Formation::GetTick() {
	if (!mLocked || mOffsetCounter != 4) {
		return mOffsetCounter;
	}
	else {
		return mPulseCount;
	}
}

void Formation::Lock() {
	mLocked = true;
}

bool Formation::Locked() {
	return mLocked && mOffsetCounter == 4;
}

void Formation::Update() {
	if (!mLocked || mOffsetCounter != 4) {
		//Our side-to-side animation is playing
		mOffsetTimer += mTimer->DeltaTime();

		if (mOffsetTimer >= mOffsetDelay) {
			mOffsetCounter++;
			Translate(Vec2_Right * (float)mOffsetDirection * mOffsetAmount, World);

			if (mOffsetCounter == 8) {
				mOffsetCounter = 0;
				mOffsetDirection *= -1;
			}

			mOffsetTimer = 0.0f;
		}
	}

	else {
		// pulse anim
		mPulseTimer += mTimer->DeltaTime();

		if (mPulseTimer >= mPulseDelay) {
			mPulseCount += mPulseDirection;

			mGridSize.x += (mPulseDirection * ((mPulseCount % 2) ? 1 : 2));

			if (mPulseCount == 4 || mPulseCount == 0) {
				mPulseDirection *= -1;
			}

			mPulseTimer = 0.0f;
		}
	}
}