#include "CoPilot.h"
#include "PhysicsManager.h"
#include "BoxCollider.h"

void CoPilot::HandleFiring() {
	if (mInput->KeyPressed(SDL_SCANCODE_SPACE)) {
		for (int i = 0; i < MAX_BULLETS; i++) {
			if (!mBullets[i]->Active()) {
				mBullets[i]->Fire(Position());
				break;
			}
		}
	}
}

CoPilot::CoPilot() {
	mInput = InputManager::Instance();
	mWasHit = false;

	mShip = new Texture("PlayerShips.png", 0, 0, 60, 64);
	mShip->Parent(this);
	mShip->Position(Vec2_Zero);

	//PhysEnt ops
	AddCollider(new BoxCollider(Vector2(16.0f, 67.0f)));
	AddCollider(new BoxCollider(Vector2(20.0f, 37.0f)), Vector2(18.0f, 10.0f));
	AddCollider(new BoxCollider(Vector2(20.0f, 37.0f)), Vector2(-18.0f, 10.0f));

	mID = PhysicsManager::Instance()->RegisterEntity(this, PhysicsManager::CollisionLayers::Friendly);

	for (int i = 0; i < MAX_BULLETS; i++) {
		mBullets[i] = new Bullet(true);
	}

	mTag = "CoPilot";
}

CoPilot::~CoPilot() {
	mInput = nullptr;

	delete mShip;
	mShip = nullptr;

	for (auto bullet : mBullets) {
		delete bullet;
		bullet = nullptr;
	}
}

bool CoPilot::IgnoreCollisions() {
	return !Active();
}

bool CoPilot::GetWasHit() {
	return mWasHit;
}

void CoPilot::Hit(PhysEntity* other) {
	mWasHit = true;
	Active(false);
}

void CoPilot::ResetWasHit() {
	mWasHit = false;
}

void CoPilot::Update() {
	if (Active()) {
		HandleFiring();
	}
	for (int i = 0; i < MAX_BULLETS; i++) {
		mBullets[i]->Update();
	}
}

void CoPilot::Render() {
	if (Active()) {
		mShip->Render();
	}
	for (int i = 0; i < MAX_BULLETS; i++) {
		mBullets[i]->Render();
	}
	PhysEntity::Render();
}