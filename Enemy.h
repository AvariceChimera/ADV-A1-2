#pragma once
#include "BezierPath.h"
#include "AnimatedTexture.h"
#include "PhysEntity.h"
#include "Player.h"
#include "Formation.h"

using namespace SDLFramework;

class Enemy : public PhysEntity {
public:
	enum States {FlyIn, InFormation, Diving, Dead };
	enum Types { Butterfly, Wasp, Boss};

	static void CurrentPlayer(Player* player);
	static void CreatePaths();
	static void SetFormation(Formation* formation);

	States CurrentState();
	Types Type();
	int Index();


	Enemy(int path, int index, bool challenge);
	virtual ~Enemy();

	virtual void Dive(int type = 0);

	bool InDeathAnimation();

	void Update() override;
	virtual void Render() override;

	virtual void Hit(PhysEntity* other) override;

protected:
	static Formation* sFormation;
	static std::vector<std::vector<Vector2>> sPaths;
	static Player* sPlayer;

	Timer* mTimer;

	Texture* mTextures[2];

	AnimatedTexture* mDeathAnimation;

	States mCurrentState;
	Types mType;

	int mIndex;

	bool mChallengeStage;

	Vector2 mDiveStartPosition;

	unsigned mCurrentPath;

	unsigned mCurrentWaypoint;
	const float EPSILON = 50.0f;

	float mSpeed;

	virtual void PathComplete();
	virtual void FlyInComplete();
	
	void JoinFormation();

	virtual Vector2 WorldFormationPosition();
	virtual Vector2 LocalFormationPosition() = 0;

	virtual void HandleFlyInState();
	virtual void HandleInFormationState();
	virtual void HandleDivingState() = 0;
	virtual void HandleDeadState();

	void HandleStates();

	virtual void RenderFlyInState();
	virtual void RenderInFormationState();
	virtual void RenderDivingState() = 0;
	virtual void RenderDeadState();

	virtual void RenderStates();

	bool IgnoreCollisions() override;
};