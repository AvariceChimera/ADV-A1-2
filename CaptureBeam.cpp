#include "CaptureBeam.h"
#include "PhysicsManager.h"
#include "BoxCollider.h"

CaptureBeam::CaptureBeam() : AnimatedTexture("CaptureBeam.png", 0, 0, 184, 320, 3, 0.5f, Horizontal) {
	mTotalCaptureTime = 6.0f;
	ResetAnimation();

	AddCollider(new BoxCollider(Vector2(160.0f, 60.0f)), Vector2(0.0f, -100.0f));
	mID = PhysicsManager::Instance()->RegisterEntity(this, PhysicsManager::CollisionLayers::Hostile);

	bool mCatch = false;

	mTag = "CaptureBeam";
}

CaptureBeam::~CaptureBeam() { }

bool CaptureBeam::Catch() {
	return mCatch;
}

void CaptureBeam::Release() {
	if (mCatch == true) {
		mCatch = false;
	}
}

void CaptureBeam::RunAnimation() {
	mCaptureTimer += mTimer->DeltaTime();
	if (mCaptureTimer >= mTotalCaptureTime) {
		mAnimationDone = true;
	}
	else {
		mAnimationTimer += mTimer->DeltaTime();
		if (mAnimationTimer >= mAnimationSpeed) {
			mAnimationTimer -= mAnimationSpeed;
		}

		mSourceRect.x = (int)(mAnimationTimer / mTimePerFrame) * mWidth;

		if (mCaptureTimer < 2.0f) {
			float temp = (int)(mCaptureTimer * 3.5f);
			mSourceRect.h = (int)(temp / 7.0f * mHeight);
		}
		else if (mCaptureTimer > mTotalCaptureTime - 2.0f) {
			float temp = (int)((mTotalCaptureTime - mCaptureTimer) * 3.5f);
			mSourceRect.h = (int)(temp / 7.0f * mHeight);
		}
		else {
			mSourceRect.h = mHeight;
		}
	}
}

void CaptureBeam::ResetAnimation() {
	AnimatedTexture::ResetAnimation();
	mCaptureTimer = 0.0f;
	mSourceRect.h = 0;
}

void CaptureBeam::Render() {
	Vector2 pos = AnimatedTexture::Position(World);
	Vector2 scale = AnimatedTexture::Scale(World);
	mDestinationRect.x = (int)(pos.x - mWidth * scale.x * 0.5f);
	mDestinationRect.y = (int)(pos.y - mHeight * scale.y * 0.5f);
	mDestinationRect.w = (int)(mWidth * scale.x);
	mDestinationRect.h = mSourceRect.h;

	mGraphics->DrawTexture(mTex, mClipped ? &mSourceRect : nullptr, &mDestinationRect, AnimatedTexture::Rotation(World));

	PhysEntity::Render();
}

void CaptureBeam::Hit(PhysEntity* other) {
	if (other->Tag() == "Player") {
		mCatch = true;
	}
}

bool CaptureBeam::IgnoreCollisions() {
	return mCaptureTimer <= 2.1f || mCaptureTimer >= mTotalCaptureTime - 2.0;
}