#include "Boss.h"
#include "PhysicsManager.h"

std::vector<std::vector<Vector2>> Boss::sDivePaths;

void Boss::CreateDivePaths() {
    int currentPath = 0;
    BezierPath* path = new BezierPath();

    path->AddCurve({
        Vector2(0.0f, 0.0f),
        Vector2(0.0f, -60.0f),
        Vector2(-90.0f, -60.0f),
        Vector2(-90.0f, 0.0f) }, 15);
    path->AddCurve({
        Vector2(-90.0f, 0.0f),
        Vector2(-90.0f, 60.0f),
        Vector2(-100.0f, 272.0f),
        Vector2(-15.0f, 275.0f) }, 15);
    path->AddCurve({
        Vector2(-15.0f, 275.0f),
        Vector2(85.0f, 275.0f),
        Vector2(85.0f, 125.0f),
        Vector2(-15.0f, 125.0f) }, 15);
    path->AddCurve({
        Vector2(-15.0f, 125.0f),
        Vector2(-175.0f, 125.0f),
        Vector2(0.0f, 450.0f),
        Vector2(125.0f, 450.0f) }, 25);
    path->AddCurve({
        Vector2(120.0f, 450.0f),
        Vector2(160.0f, 450.0f),
        Vector2(200.0f, 500.0f),
        Vector2(200.0f, 550.0f) }, 15);
    path->AddCurve({
        Vector2(200.0f, 550.0f),
        Vector2(200.0f, 540.0f),
        Vector2(200.0f, 810.0f),
        Vector2(200.0f, 800.0f) }, 15);

    sDivePaths.push_back(std::vector<Vector2>());
    path->Sample(&sDivePaths[currentPath]);
    delete path;

    currentPath = 1;
    path = new BezierPath();

    path->AddCurve({
        Vector2(0.0f, 0.0f),
        Vector2(0.0f, -60.0f),
        Vector2(90.0f, -60.0f),
        Vector2(90.0f, 0.0f) }, 15);
    path->AddCurve({
        Vector2(90.0f, 0.0f),
        Vector2(90.0f, 60.0f),
        Vector2(100.0f, 272.0f),
        Vector2(15.0f, 275.0f) }, 15);
    path->AddCurve({
        Vector2(15.0f, 275.0f),
        Vector2(-85.0f, 275.0f),
        Vector2(-85.0f, 125.0f),
        Vector2(15.0f, 125.0f) }, 15);
    path->AddCurve({
        Vector2(15.0f, 125.0f),
        Vector2(175.0f, 125.0f),
        Vector2(0.0f, 450.0f),
        Vector2(-125.0f, 450.0f) }, 25);
    path->AddCurve({
        Vector2(-120.0f, 450.0f),
        Vector2(-160.0f, 450.0f),
        Vector2(-200.0f, 500.0f),
        Vector2(-200.0f, 550.0f) }, 15);
    path->AddCurve({
        Vector2(-200.0f, 550.0f),
        Vector2(-200.0f, 540.0f),
        Vector2(-200.0f, 810.0f),
        Vector2(-200.0f, 800.0f) }, 15);

    sDivePaths.push_back(std::vector<Vector2>());
    path->Sample(&sDivePaths[currentPath]);
    delete path;

    currentPath = 2;
    path = new BezierPath();

    path->AddCurve({
        Vector2(0.0f, 0.0f),
        Vector2(0.0f, -60.0f),
        Vector2(-90.0f, -60.0f),
        Vector2(-90.0f, 0.0f) }, 15);
    path->AddCurve({
        Vector2(-90.0f, 0.0f),
        Vector2(-90.0f, 60.0f),
        Vector2(100.0f, 340.0f),
        Vector2(100.0f, 400.0f) }, 15);

    sDivePaths.push_back(std::vector<Vector2>());
    path->Sample(&sDivePaths[currentPath]);
    delete path;

    currentPath = 3;
    path = new BezierPath();

    path->AddCurve({
        Vector2(0.0f, 0.0f),
        Vector2(0.0f, -60.0f),
        Vector2(90.0f, -60.0f),
        Vector2(90.0f, 0.0f) }, 15);
    path->AddCurve({
        Vector2(90.0f, 0.0f),
        Vector2(90.0f, 60.0f),
        Vector2(-100.0f, 340.0f),
        Vector2(-100.0f, 400.0f) }, 15);

    sDivePaths.push_back(std::vector<Vector2>());
    path->Sample(&sDivePaths[currentPath]);
    delete path;
}

void Boss::Dive(int type) {
    mCaptureDive = type != 0;

    Enemy::Dive();

    if (mCaptureDive) {
        mCapturing = false;
        mCurrentPath = 2 + Random::Instance()->RandomRange(0, 1);
        mCaptureBeam->ResetAnimation();
    }
    else {
        mCurrentPath = mIndex % 2;
    }
}

Vector2 Boss::LocalFormationPosition() {
	Vector2 retVal;

	int dir = mIndex % 2 == 0 ? -1 : 1;

	retVal.x = (sFormation->GridSize().x + sFormation->GridSize().x * 2 * (mIndex / 2)) * (float)dir;
	retVal.y = -sFormation->GridSize().y;

	return retVal;
}

Boss::Boss(int path, int index, bool challenge) : Enemy(path, index, challenge) {
	mTextures[0] = new Texture("Bosses.png", 0, 0, 64, 64);
	mTextures[1] = new Texture("Bosses.png", 64, 0, 64, 64);
	for (auto texture : mTextures) {
		texture->Parent(this);
		texture->Position(Vec2_Zero);
	}

    mHostage = new Texture("PlayerShips.png", 60, 0, 60, 64);
    mHostage->Parent(this);
    mHostage->Position(0.0f, -64.0f);
    mDisplayHostage = false;

	mType = Enemy::Boss;

    mCaptureDive = false;
    mCurrentPath = 0;
    mCapturing = false;
    mCaptureSuccess = false;

    mCaptureBeam = new CaptureBeam();
    mCaptureBeam->AnimatedTexture::Parent(this);
    mCaptureBeam->AnimatedTexture::Position(0.0f, -190.0f);
    mCaptureBeam->AnimatedTexture::Rotation(180.0f);

    mCaptureBeam->PhysEntity::Parent(this);
    mCaptureBeam->PhysEntity::Position(0.0f, -190.0f);
    mCaptureBeam->PhysEntity::Rotation(180.0f);

    AddCollider(new BoxCollider(mTextures[1]->ScaledDimensions()));
    mID = PhysicsManager::Instance()->RegisterEntity(this, PhysicsManager::CollisionLayers::Hostile);

    mWasHit = false;

    mTag = "Boss";
}

Boss::~Boss() {
    delete mCaptureBeam;
    mCaptureBeam = nullptr;

    delete mHostage;
    mHostage = nullptr;
}

void Boss::Hit(PhysEntity* other) {
    if (mWasHit) {
        AudioManager::Instance()->PlaySFX("BossDestroyed.wav", 0, 2);
        sPlayer->AddScore(mCurrentState == Enemy::InFormation ? 150 : mCaptureDive ? 400 : 800);
        Enemy::Hit(other);
        if (mDisplayHostage) {
            sPlayer->DisplayCoPilot();
            mDisplayHostage = false;
        }
    }
    else {
        mWasHit = true;
        SDL_Rect temp = {0, 64, 60, 64};
        mTextures[0]->SetSourceRect(&temp);
        temp.x = 66;
        temp.y = 68;
        mTextures[1]->SetSourceRect(&temp);
        AudioManager::Instance()->PlaySFX("BossInjured.wav", 0, 2);
    }
}

void Boss::HandleDivingState() {
    if (mCurrentWaypoint < sDivePaths[mCurrentPath].size()) {
        //follow dive path
        Vector2 waypointPos = mDiveStartPosition + sDivePaths[mCurrentPath][mCurrentWaypoint];
        //auto dist = waypointPos - Position() same as below
        Vector2 dist = waypointPos - Position();

        Translate(dist.Normalized() * mSpeed * mTimer->DeltaTime(), World);
        Rotation(atan2(dist.y, dist.x) * RAD_TO_DEG + 90.0f);

        if ((waypointPos - Position()).MagnitudeSqr() < EPSILON * mSpeed / 25) {
            mCurrentWaypoint++;
        }

        if (mCurrentWaypoint == sDivePaths[mCurrentPath].size()) {
            if (mCaptureDive) {
                mCapturing = true;
                Rotation(180.0f);
            }
            else {
                Position(Vector2(WorldFormationPosition().x, 20.0f));
            }
        }

    }
    else {

        if (!mCaptureDive || !mCapturing) {
            //return to formation
            Vector2 dist = WorldFormationPosition() - Position();

            Translate(dist.Normalized() * mSpeed * mTimer->DeltaTime(), World);
            Rotation(atan2(dist.y, dist.x) * RAD_TO_DEG + 90.0f);

            if (dist.MagnitudeSqr() < EPSILON * mSpeed / 25) {
                JoinFormation();
            }
        }
        else {
            HandleCaptureBeam();
        }

    }
}

void Boss::HandleCaptureBeam() {
    mCaptureBeam->AnimatedTexture::Update();
    mCaptureBeam->PhysEntity::Update();
    if (mCaptureBeam->Catch()) {
        mDisplayHostage = true;
    }
    if (!mCaptureBeam->IsAnimating()) {
        mCapturing = false;
        /*Translate(Vec2_Up * mSpeed * mTimer->DeltaTime());
        if (Position().y >= 910.0f) {
            Position(WorldFormationPosition().x, -20.0f);
            mCapturing = false;
        }*/

        Vector2 dist = WorldFormationPosition() - Position();

        Translate(dist.Normalized() * mSpeed * mTimer->DeltaTime(), World);
        Rotation(atan2(dist.y, dist.x) * RAD_TO_DEG + 90.0f);

        if (dist.MagnitudeSqr() < EPSILON * mSpeed / 25) {
            JoinFormation();
        }
    }
}

void Boss::RenderDivingState() {
    mTextures[0]->Render();

    if (mCapturing && mCaptureBeam->IsAnimating()) {
        mCaptureBeam->Render();
    }

}

void Boss::FlyInComplete() {
	Enemy::FlyInComplete();
}

void Boss::Render() {
    if (mDisplayHostage) {
        mHostage->Render();
    }
    Enemy::Render();
}