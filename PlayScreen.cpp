#include "PlayScreen.h"

PlayScreen::PlayScreen() {
	mTimer = Timer::Instance();
	mAudio = AudioManager::Instance();

	mStars = BackgroundStars::Instance();

	mSideBar = new PlaySideBar();
	mSideBar->Parent(this);
	mSideBar->Position(Graphics::SCREEN_WIDTH * 0.87f, Graphics::SCREEN_HEIGHT * 0.05f);

	mStartLabel = new Texture("START", "emulogic.ttf", 32, { 150,0,0 });
	mStartLabel->Parent(this);
	mStartLabel->Position(Graphics::SCREEN_WIDTH * 0.4f, Graphics::SCREEN_HEIGHT * 0.5f);

	mLevel = nullptr;
	mLevelStartDelay = 1.0f;
	mLevelStarted = false;
}

PlayScreen::~PlayScreen() {
	mTimer = nullptr;
	mAudio = nullptr;

	mStars = nullptr;

	delete mSideBar;
	mSideBar = nullptr;

	delete mStartLabel;
	mStartLabel = nullptr;

	delete mLevel;
	mLevel = nullptr;
}

void PlayScreen::StartNewGame() {
	mSideBar->SetHighScore(100000);
	mSideBar->SetShips(2);
	mStars->Scroll(false);
	mGameStarted = false;
	mLevelStarted = false;
	mLevelStartTimer = 0.0f;
	mCurrentStage = 0;

	mAudio->PlayMusic("GameStart.wav", 0);
}

void PlayScreen::StartNextLevel() {
	mCurrentStage++;
	mLevelStartTimer = 0.0f;
	mLevelStarted = true;

	delete mLevel;
	mLevel = new Level(mCurrentStage, mSideBar);
}

void PlayScreen::Update() {
	if (mGameStarted) {
		if (!mLevelStarted) {
			mLevelStartTimer += mTimer->DeltaTime();
			if (mLevelStartTimer >= mLevelStartDelay) {
				StartNextLevel();
			}
		}
		else {
			//level has started or in progress
			mLevel->Update();
		}
		//we are in a level of some kind
		if (mCurrentStage > 0) {
			mSideBar->Update();
		}
	}
	else {
		if (!Mix_PlayingMusic()) {
			//intro chime done
			mGameStarted = true;
		}
	}
}

void PlayScreen::Render() {
	if (!mGameStarted) {
		mStartLabel->Render();
	}

	if (mGameStarted && mLevelStarted) {
		mLevel->Render();
	}

	mSideBar->Render();
}