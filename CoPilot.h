#pragma once
#include "PhysEntity.h"
#include "Bullet.h"
#include "InputManager.h"

using namespace SDLFramework;

class CoPilot : public PhysEntity {
public:
	void Hit(PhysEntity* other) override;
	bool IgnoreCollisions() override;
	CoPilot();
	~CoPilot();
	bool GetWasHit();
	void ResetWasHit();
	void Update() override;
	void Render() override;
	void HandleFiring();

private:
	static const int MAX_BULLETS = 5;
	Bullet* mBullets[MAX_BULLETS];
	bool mWasHit;
	InputManager* mInput;
	Texture* mShip;
};