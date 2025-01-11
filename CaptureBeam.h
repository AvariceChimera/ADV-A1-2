#pragma once
#include "AnimatedTexture.h"
#include "PhysEntity.h"

using namespace SDLFramework;

class CaptureBeam : public AnimatedTexture, public PhysEntity {
public:

	CaptureBeam();
	~CaptureBeam();

	bool Catch();
	bool IgnoreCollisions() override;
	void Hit(PhysEntity* other) override;
	void ResetAnimation() override;
	void Render() override;

private:

	bool mCatch;
	void Release();
	void RunAnimation() override;
	float mTotalCaptureTime;
	float mCaptureTimer;

};