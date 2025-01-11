#pragma once
#include "Level.h"
#include "Player.h"

using namespace SDLFramework;

class PlayScreen : public GameEntity {
public:
	PlayScreen();
	~PlayScreen();

	void StartNewGame(int modeSelected);
	void StartNextLevel();

	bool GameOver();

	void Update() override;
	void Render() override;

private:
	Timer* mTimer;
	AudioManager* mAudio;

	BackgroundStars* mStars;
	PlaySideBar* mSideBar;

	Texture* mStartLabel;

	Player* mPlayer;

	Level* mLevel;

	float mLevelStartTimer;
	float mLevelStartDelay;

	bool mGameStarted;
	bool mLevelStarted;
	int mCurrentStage;
	int mModeSelected;
};