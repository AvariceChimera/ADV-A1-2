#pragma once
#include "Enemy.h"
#include "CaptureBeam.h"
#include "Random.h"



class Boss : public Enemy {
public:
	static void CreateDivePaths();

	void Dive(int type = 0) override;

	void Hit(PhysEntity* other) override;

	void Render() override;

	Boss(int path, int index, bool challenge);
	~Boss();

private:
	static std::vector<std::vector<Vector2>> sDivePaths;
	
	bool mCaptureDive;
	int mCurrentPath;
	bool mCapturing;
	
	void CaptureSuccess();
	bool mCaptureSuccess;

	Texture* mHostage;

	CaptureBeam* mCaptureBeam;

	bool mWasHit;

	void FlyInComplete() override;

	Vector2 LocalFormationPosition() override;

	void HandleDivingState() override;

	void HandleCaptureBeam();

	void RenderDivingState() override;
};