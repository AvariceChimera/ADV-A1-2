#include "StartScreen.h"

StartScreen::StartScreen() {
	mTimer = Timer::Instance();
	mInputManager = InputManager::Instance();

	//top bar
	mTopBar = new GameEntity(Graphics::SCREEN_WIDTH * 0.5f, 80.0f);
	mPlayerOne = new Texture("1UP", "emulogic.ttf", 32, { 200, 0, 0 });
	mPlayerTwo = new Texture("2UP", "emulogic.ttf", 32, { 200, 0, 0 });
	mHiScore = new Texture("HI-SCORE", "emulogic.ttf", 32, { 200, 0, 0 });
	mPlayerOneScore = new Scoreboard({255,255,255});
	mPlayerTwoScore = new Scoreboard({255,255,255});
	mTopScore = new Scoreboard();

	mTopBar->Parent(this);
	mPlayerOne->Parent(mTopBar);
	mPlayerTwo->Parent(mTopBar);
	mHiScore->Parent(mTopBar);
	mPlayerOneScore->Parent(mTopBar);
	mPlayerTwoScore->Parent(mTopBar);
	mTopScore->Parent(mTopBar);

	mPlayerOne->Position(-Graphics::SCREEN_WIDTH * 0.35f, 0.0f);
	mPlayerTwo->Position(Graphics::SCREEN_WIDTH * 0.3f, 0.0f);
	mHiScore->Position(-30, 0.0f);

	//PLAYER SCORES DON'T SHOW PROPERLY FIX IT (if it's not too much trouble)
	mPlayerOneScore->Position(-Graphics::SCREEN_WIDTH * 0.32f, 40.0f);
	mPlayerTwoScore->Position(Graphics::SCREEN_WIDTH * 0.32f, 40.0f);
	mTopScore->Position(Graphics::SCREEN_WIDTH * 0.05f, 40.0f);
	mTopScore->Score(999999);

	//logo
	mLogoHolder = new GameEntity(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.32);
	mLogo = new Texture("GalagaLogo.png", 0, 0, 360, 180);
	mAnimatedLogo = new AnimatedTexture("GalagaLogo.png", 0, 0, 360, 180, 3, 0.5f, AnimatedTexture::Vertical);

	mLogoHolder->Parent(this);
	mLogo->Parent(mLogoHolder);
	mAnimatedLogo->Parent(mLogoHolder);

	mLogo->Position(Vec2_Zero);
	mAnimatedLogo->Position(Vec2_Zero);

	//play mode
	mPlayModes = new GameEntity(Graphics::SCREEN_WIDTH * 0.5, Graphics::SCREEN_HEIGHT * 0.55f);
	mOnePlayerMode = new Texture("1 Player", "emulogic.ttf", 32, { 230, 230, 230});
	mTwoPlayerMode = new Texture("2 Player", "emulogic.ttf", 32, { 230, 230, 230 });
	mCursor = new Texture("Cursor.png");

	mPlayModes->Parent(this);
	mOnePlayerMode->Parent(mPlayModes);
	mTwoPlayerMode->Parent(mPlayModes);
	mCursor->Parent(mPlayModes);

	mOnePlayerMode->Position(0.0f, -35.0f);
	mTwoPlayerMode->Position(0.0f, 35.0f);
	mCursor->Position(-175.0f, -35.0f);

	//selection variables
	mSelectedMode = 0;
	mCursorOffsetPos = Vector2(0.0f, 70.0f);
	mCursorStartPos = mCursor->Position(Local);

	//bottom bar
	mBottomBar = new GameEntity(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.7f);
	mNamco = new Texture("namco", "namco__.tf", 36, { 200, 0, 0 });
	mDates = new Texture("1981 1985 NAMCO LTD.", "emulogic.ttf", 32, { 230, 230, 230 });
	mRights = new Texture("ALL RIGHTS RESERVED", "emulogic.ttf", 32, { 230, 230, 230 });

	mBottomBar->Parent(this);
	mNamco->Parent(mBottomBar);
	mDates->Parent(mBottomBar);
	mRights->Parent(mBottomBar);

	mNamco->Position(Vec2_Zero);
	mDates->Position(0.0f, 90.0f);
	mRights->Position(0.0f, 170.0f);

	// screen anim
	ResetAnimation();
}

void StartScreen::ResetAnimation() {
	mAnimationStartPos = Vector2(0.0f, Graphics::SCREEN_HEIGHT);
	mAnimationEndPos = Vec2_Zero;
	mAnimationTotalTime = 5.0f;
	mAnimationTimer = 0.0f;
	mAnimationDone = false;

	Position(mAnimationStartPos);
}

int StartScreen::SelectedMode() {
	return mSelectedMode;
}

void StartScreen::ChangeSelectedMode(int change) {
	mSelectedMode += change;

	if (mSelectedMode < 0) {
		//set mode to the last mode option
		mSelectedMode = 1;
	}
	else if (mSelectedMode > 1) {
		mSelectedMode = 0;
	}

	mCursor->Position(mCursorStartPos + mCursorOffsetPos * (float)mSelectedMode);
}

void StartScreen::Update() {
	if (!mAnimationDone) {
		mAnimationTimer += mTimer->DeltaTime();
		Position(Lerp(mAnimationStartPos, mAnimationEndPos, mAnimationTimer / mAnimationTotalTime));

		if (mAnimationTimer >= mAnimationTotalTime) {
			mAnimationDone = true;
		}

		if (mInputManager->KeyPressed(SDL_SCANCODE_Q)) {
			mAnimationTimer = mAnimationTotalTime;
		}
	}
	else {
		mAnimatedLogo->Update();

		if (mInputManager->KeyPressed(SDL_SCANCODE_DOWN)) {
			ChangeSelectedMode(1);
		}
		else if (mInputManager->KeyPressed(SDL_SCANCODE_UP)) {
			ChangeSelectedMode(-1);
		}
	}
}


void StartScreen::Render() {
	//top bar
	mPlayerOne->Render();
	mPlayerTwo->Render();
	mHiScore->Render();
	mPlayerOneScore->Render();
	mPlayerTwoScore->Render();
	mTopScore->Render();

	//logo
	if (!mAnimationDone) {
		mLogo->Render();
	}
	else {
		mAnimatedLogo->Render();
	}


	//play bar
	mOnePlayerMode->Render();
	mTwoPlayerMode->Render();
	mCursor->Render();

	//bottom bar
	mNamco->Render();
	mDates->Render();
	mRights->Render();
}

StartScreen::~StartScreen() {
	delete mTopBar;
	mTopBar = nullptr;
	delete mPlayerOne;
	mPlayerOne = nullptr;
	delete mPlayerTwo;
	mPlayerTwo = nullptr;
	delete mHiScore;
	mHiScore = nullptr;
	delete mPlayerOneScore;
	mPlayerOneScore = nullptr;
	delete mPlayerTwoScore;
	mPlayerTwoScore = nullptr;
	delete mTopScore;
	mTopScore = nullptr;

	delete mPlayModes;
	mPlayModes = nullptr;
	delete mOnePlayerMode;
	mOnePlayerMode = nullptr;
	delete mTwoPlayerMode;
	mTwoPlayerMode = nullptr;
	delete mCursor;
	mCursor = nullptr;

	delete mBottomBar;
	mBottomBar = nullptr;
	delete mNamco;
	mNamco = nullptr;
	delete mDates;
	mDates = nullptr;
	delete mRights;
	mRights = nullptr;

	delete mLogo;
	mLogo = nullptr;
	delete mLogoHolder;
	mLogoHolder = nullptr;
	delete mAnimatedLogo;
	mAnimatedLogo = nullptr;

	mTimer = nullptr;
	mInputManager = nullptr;
}