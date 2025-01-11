#include "Player.h"
#include "PhysicsManager.h"

void Player::HandleMovement() {
	if (mInput->KeyDown(SDL_SCANCODE_A) || mInput->KeyDown(SDL_SCANCODE_LEFT)) {
		Translate(-Vec2_Right * mMoveSpeed * mTimer->DeltaTime(), World);
	}
	else if (mInput->KeyDown(SDL_SCANCODE_D) || mInput->KeyDown(SDL_SCANCODE_RIGHT)) {
		Translate(Vec2_Right * mMoveSpeed * mTimer->DeltaTime(), World);
	}

	Vector2 pos = Position(Local);
	if (pos.x < mMoveBounds.x) {
		pos.x = mMoveBounds.x;
	}//small bug of going half off screen not major but it should be fixed if there's time
	else if (pos.x > mMoveBounds.y) {
		pos.x = mMoveBounds.y;
	}

	Position(pos);
}

void Player::HandleFiring() {
	if (mInput->KeyPressed(SDL_SCANCODE_SPACE)) {
		for (int i = 0; i < MAX_BULLETS; i++) {
			if (!mBullets[i]->Active()) {
				mBullets[i]->Fire(Position());
				mAudio->PlaySFX("Fire.wav");
				break;
			}
		}
	}
}

Player::Player() {
	mTimer = Timer::Instance();
	mInput = InputManager::Instance();
	mAudio = AudioManager::Instance();

	mVisible = false;
	mAnimating = false;
	mWasHit = false;

	mScore = 0;
	mLives = 2;

	mMoveSpeed = 200.0f;
	//the below vector is a min/max not a coordinate location
	mMoveBounds = Vector2(0.0f, 750.0f);

	mShip = new Texture("PlayerShips.png", 0, 0, 60, 64);
	mShip->Parent(this);
	mShip->Position(Vec2_Zero);

	mDeathAnimation = new AnimatedTexture("PlayerExplosion.png", 0, 0, 128, 128, 4, 1.0f, AnimatedTexture::Horizontal);
	mDeathAnimation->Parent(this);
	mDeathAnimation->Position(Vec2_Zero);
	mDeathAnimation->SetWrapMode(AnimatedTexture::Once);

	//PhysEnt ops
	AddCollider(new BoxCollider(Vector2(16.0f,67.0f)));
	AddCollider(new BoxCollider(Vector2(20.0f, 37.0f)), Vector2(18.0f, 10.0f));
	AddCollider(new BoxCollider(Vector2(20.0f, 37.0f)), Vector2(-18.0f, 10.0f));

	mID = PhysicsManager::Instance()->RegisterEntity(this, PhysicsManager::CollisionLayers::Friendly);

	for (int i = 0; i < MAX_BULLETS; i++) {
		mBullets[i] = new Bullet(true);
	}

	mTag = "Player";

	mCaptureTimer = 0.0f;
	mCaptureTotalTime = 2.5f;

	mCoPilot = new CoPilot();
	mCoPilot->Parent(this);
	mCoPilot->Position(Vector2(65.0f, 0.0f));
	mCoPilot->Active(false);

}

Player::~Player() {
	mTimer = nullptr;
	mInput = nullptr;
	mAudio = nullptr;

	delete mShip;
	mShip = nullptr;

	delete mDeathAnimation;
	mDeathAnimation = nullptr;

	for (auto bullet : mBullets) {
		delete bullet;
		bullet = nullptr;
	}
}

void Player::Visible(bool visible) {
	mVisible = visible;
}

void Player::AddScore(int change) {
 	mScore += change;
}

bool Player::IsAnimating() {
	return mAnimating;
}

int Player::Score() {
	return mScore;
}

int Player::Lives() {
	return mLives;
}

bool Player::WasHit() {
	return mWasHit;
}

bool Player::IgnoreCollisions() {
	return !mVisible || mAnimating || !Active();
}

void Player::Hit(PhysEntity* other) {
	if (other->Tag() == "CaptureBeam") {
		mCaptured = true;
		mCaptureBeam = other->Position(World);
		std::cout << "we've been captured" << std::endl;
	}
	else {
		if (mCoPilot->Active()) {
			mCoPilot->Active(false);
		}
		else {
			//mLives -= 1;
			//mAnimating = true;
			//mDeathAnimation->ResetAnimation();
			//mWasHit = true;
			//mAudio->PlaySFX("PlayerExplosion.wav");
		}
	}
}

void Player::HandleCapturedState() {
	Active(false);
	mCaptureTimer += mTimer->DeltaTime();
	if (mCaptureTimer <= mCaptureTotalTime) {
		if ((mCaptureBeam - Position()).MagnitudeSqr() > EPSILON * mMoveSpeed / 25.0f) {
			//Player::Rotate(atan2(dist.y, dist.x) * RAD_TO_DEG + 90.0f);
			Vector2 dist = mCaptureBeam - Position(World);
			Translate(dist.Normalized() * mMoveSpeed * mTimer->DeltaTime(), World);
		}
		else {
			Rotate(100.0f);
		}
	}
	else {
		if (mCoPilot->Active()) {
			mCoPilot->Active(false);
		}
		else {
			mLives -= 1;
			mAnimating = true;
			mDeathAnimation->ResetAnimation();
			mWasHit = true;
			mAudio->PlaySFX("PlayerExplosion.wav", 0, -1);
			mCaptured = false;
			mCaptureTimer = 0.0f;
		}
	}
}

void Player::DisplayCoPilot() {
	if (!mCoPilot->Active()) {
		mCoPilot->Active(true);

	}
}

void Player::Update() {
	if (mAnimating) {
		if (mWasHit) {
			mWasHit = false;
		}

		mDeathAnimation->Update();
		mAnimating = mDeathAnimation->IsAnimating();
	}
	else {
		if (mCaptured) {
			HandleCapturedState();
		}
		if (Active()) {
			if (mCoPilot->Active()) {
				mCoPilot->Update();
			}
			HandleMovement();
			HandleFiring();
		}
	}

	for (int i = 0; i < MAX_BULLETS; i++) {
		mBullets[i]->Update();
	}
}

void Player::Render() {
	if (mVisible) {
		if (mAnimating) {
			mDeathAnimation->Render();
		}
		else {
			mShip->Render();
			if (mCoPilot->Active()) {
				mCoPilot->Render();
			}
		}

		PhysEntity::Render();
	}

	for (int i = 0; i < MAX_BULLETS; i++) {
		mBullets[i]->Render();
	}
}