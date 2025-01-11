#pragma once
#include "AnimatedTexture.h"
#include "InputManager.h"
#include "AudioManager.h"
#include "BoxCollider.h"
#include "Bullet.h"
#include "CoPilot.h"

using namespace SDLFramework;

class Player : public PhysEntity {
public:
	Player();
	~Player();

	void Update() override;
	void Render() override;
	void DisplayCoPilot();

	void Visible(bool visible);
	bool IsAnimating();

	int Score();
	int Lives();

	void AddScore(int change);

	bool WasHit();

	//Inhetited from PhysEnt
	bool IgnoreCollisions() override;
	void Hit(PhysEntity* other) override;

private:
	static const int MAX_BULLETS = 5;
	Bullet* mBullets[MAX_BULLETS];

	bool mWasHit;

	Timer* mTimer;
	InputManager* mInput;
	AudioManager* mAudio;

	bool mVisible;
	bool mAnimating;

	Texture* mShip;
	AnimatedTexture* mDeathAnimation;

	CoPilot* mCoPilot;

	int mScore;
	int mLives;

	float mCaptureTimer;
	float mCaptureTotalTime;
	bool mCaptured;

	const float EPSILON = 0.05f;

	float mMoveSpeed;
	Vector2 mMoveBounds;

	Vector2 mCaptureBeam;

	void HandleCapturedState();
	void HandleMovement();
	void HandleFiring();
};