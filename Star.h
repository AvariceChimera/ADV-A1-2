#pragma once
#include "Texture.h"
#include "Random.h"
#include "Timer.h"

using namespace SDLFramework;

class Star : public Texture {
public:
	static void Scroll(bool scroll);

	Star(int layer);
	~Star();

	void Update() override;
	void Render() override;

private:
	Timer* mTimer;
	Random* mRandom;

	static bool sScroll;
	float mScrollSpeed;

	bool mVisible;
	float mFlickerTime;
	float mFlickerSpeed;

	void ScrollStar();
};