#include "Wasp.h"
#include "PhysicsManager.h"

std::vector<std::vector<Vector2>> Wasp::sDivePaths;

void Wasp::CreateDivePaths() {
	int currentPath = 0;
	BezierPath* path = new BezierPath();

	path->AddCurve({
		Vector2(Vec2_Zero),
		Vector2(0.0f,-45.0f),
		Vector2(-60.0f,-45.0f),
		Vector2(-60.0f,0.0f)

		}, 15);
	path->AddCurve({
		Vector2(-60.0f,0.0f),
		Vector2(-60.0f, 80.0f),
		Vector2(0.0f,150.0f),
		Vector2(100.0f,150.0f)
		}, 15);
	path->AddCurve({
		Vector2(100.0f,150.0f),
		Vector2(250.0f,150.0f),
		Vector2(350.0f, 200.0f),
		Vector2(350.0f, 350.0f)
		}, 15);
	path->AddCurve({
		Vector2(350.0f, 350.0f),
		Vector2(350.0f, 575.0f),
		Vector2(100.0f, 575.0f),
		Vector2(100.0f, 350.0f)
		}, 15);

	sDivePaths.push_back(std::vector<Vector2>());
	path->Sample(&sDivePaths[currentPath]);

	delete path;

	currentPath++;

	path = new BezierPath();

	path->AddCurve({
	Vector2(0.0f, 0.0f),
	Vector2(0.0f, -45.0f),
	Vector2(60.0f, -45.0f),
	Vector2(60.f, 0.0f) }, 15);
	path->AddCurve({
		Vector2(60.0f, 0.0f),
		Vector2(60.0f, 80.0f),
		Vector2(0.0f, 150.0f),
		Vector2(-100.0f, 150.0f) }, 15);
	path->AddCurve({
		Vector2(-100.0f, 150.0f),
		Vector2(-250.0f, 150.0f),
		Vector2(-350.0f, 200.0f),
		Vector2(-350.0f, 350.0f) }, 15);
	path->AddCurve({
		Vector2(-350.0f, 350.0f),
		Vector2(-350.0f, 575.0f),
		Vector2(-100.0f, 575.0f),
		Vector2(-100.0f, 350.0f) }, 15);

	sDivePaths.push_back(std::vector<Vector2>());
	path->Sample(&sDivePaths[currentPath]);
	delete path;
}

Wasp::Wasp(int path, int index, bool challenge, bool diver) : Enemy(path, index, challenge), mDiver(diver) {
	mTextures[0] = new Texture("AnimatedEnemies.png", 0, 40, 52, 40);
	mTextures[1] = new Texture("AnimatedEnemies.png", 52, 40, 52, 40);
	for (auto texture : mTextures) {
		texture->Parent(this);
		texture->Position(Vec2_Zero);
	}

	mType = Enemy::Wasp;

	AddCollider(new BoxCollider(mTextures[1]->ScaledDimensions()));
	mID = PhysicsManager::Instance()->RegisterEntity(this, PhysicsManager::CollisionLayers::Hostile);

	mTag = "Wasp";
}

Wasp::~Wasp() {

}

void Wasp::Hit(PhysEntity* other) {
	AudioManager::Instance()->PlaySFX("WaspDestroyed.wav",0,4);
	sPlayer->AddScore(mCurrentState == Enemy::InFormation ? 50 : 100);
	Enemy::Hit(other);
}

void Wasp::FlyInComplete() {
	if (mDiver) {
		mCurrentState = Dead;
	}
	else {
		JoinFormation();
	}
}

Vector2 Wasp::LocalFormationPosition() {
	Vector2 retVal;

	int dir = mIndex % 2 == 0 ? -1 : 1;

	retVal.x = (sFormation->GridSize().x + sFormation->GridSize().x * 2 * (mIndex / 4)) * (float)dir;
	retVal.y = sFormation->GridSize().y * 2 + sFormation->GridSize().y * ((mIndex % 4) / 2);

	return retVal;
}

void Wasp::HandleDivingState() {
	int currentPath = mIndex % 2;

	if (mCurrentWaypoint < sDivePaths[currentPath].size()) {
		//follow dive path
		Vector2 waypointPos = mDiveStartPosition + sDivePaths[currentPath][mCurrentWaypoint];
		//auto dist = waypointPos - Position() same as below
		Vector2 dist = waypointPos - Position();

		Translate(dist.Normalized() * mSpeed * mTimer->DeltaTime(), World);
		Rotation(atan2(dist.y, dist.x) * RAD_TO_DEG + 90.0f);

		if ((waypointPos - Position()).MagnitudeSqr() < EPSILON * mSpeed / 25) {
			mCurrentWaypoint++;
		}
	}
	else {
		//return to formation
		Vector2 dist = WorldFormationPosition() - Position();

		Translate(dist.Normalized() * mSpeed * mTimer->DeltaTime(), World);
		Rotation(atan2(dist.y, dist.x) * RAD_TO_DEG + 90.0f);

		if (dist.MagnitudeSqr() < EPSILON * mSpeed / 25) {
			JoinFormation();
		}
	}
}

void Wasp::RenderDivingState() {
	mTextures[0]->Render();

	//debug render of dive
	//REMEMBER TO COM OUT WHEN FINISHED
	//int currentPath = mIndex % 2;
	//for (int i = 0; i < sDivePaths[currentPath].size() - 1; i++) {
		//Graphics::Instance()->DrawLine(mDiveStartPosition.x + sDivePaths[currentPath][i].x,
			//mDiveStartPosition.y + sDivePaths[currentPath][i].y,
			//mDiveStartPosition.x + sDivePaths[currentPath][i + 1].x,
			//mDiveStartPosition.y + sDivePaths[currentPath][i + 1].y
		//);
	//}

	//debug return path render
	//Vector2 finalPos = WorldFormationPosition();
	//Vector2 pathEndPos = mDiveStartPosition + sDivePaths[currentPath][sDivePaths[currentPath].size() - 1];

	//Graphics::Instance()->DrawLine(
		//pathEndPos.x,
		//pathEndPos.y,
		//finalPos.x,
		//finalPos.y
	//);
}