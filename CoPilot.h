#pragma once
#include "Player.H"

class CoPilot : public PhysEntity {
public:
	CoPilot();
	~CoPilot();

	void Update() override;
	void Render() override;

	void Visible(bool visible);
	bool IsAnimating();

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

	int mScore;

	float mMoveSpeed;
 	Vector2 mMoveBounds;

	void HandleMovement();
	void HandleFiring();
};