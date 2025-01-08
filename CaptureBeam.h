#pragma once
#include "AnimatedTexture.h"

using namespace SDLFramework;

class CaptureBeam : public AnimatedTexture {
public:

	CaptureBeam();
	~CaptureBeam();

	void ResetAnimation() override;
	void Render() override;

private:

	void RunAnimation() override;

	float mTotalCaptureTime;
	float mCaptureTimer;

};