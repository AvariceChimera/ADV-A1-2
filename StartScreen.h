#pragma once
#include "AnimatedTexture.h"
#include "InputManager.h"
#include "Scoreboard.h"

using namespace SDLFramework;

class StartScreen : public GameEntity {
public:
	StartScreen();
	~StartScreen();

	void ChangeSelectedMode(int change);
	bool GetSelectedMode();

	void ResetAnimation();

	int SelectedMode();

	void Update() override;
	void Render() override;

private:
	Timer* mTimer;
	InputManager* mInputManager;

	//screen anim variables
	Vector2 mAnimationStartPos;
	Vector2 mAnimationEndPos;
	float mAnimationTotalTime;
	float mAnimationTimer;
	bool mAnimationDone;

	//Top Bar Stuff
	//empty game object/entity
	GameEntity* mTopBar;
	Texture* mPlayerOne;
	Texture* mPlayerTwo;
	Texture* mHiScore;
	Scoreboard* mPlayerOneScore;
	Scoreboard* mPlayerTwoScore;
	Scoreboard* mTopScore;

	//logos
	GameEntity* mLogoHolder;
	Texture* mLogo;
	AnimatedTexture* mAnimatedLogo;

	//Play Mode
	GameEntity* mPlayModes;
	Texture* mArcadeMode;
	Texture* mSurvivalMode;
	Texture* mCursor;
	Vector2 mCursorStartPos;
	Vector2 mCursorOffsetPos;
	int mSelectedMode;

	//bottom bar
	GameEntity* mBottomBar;
	Texture* mNamco;
	Texture* mDates;
	Texture* mRights;
};