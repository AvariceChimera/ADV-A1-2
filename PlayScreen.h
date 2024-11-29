#pragma once
#include "Level.h"

using namespace SDLFramework;

class PlayScreen : public GameEntity {
public:
	PlayScreen();
	~PlayScreen();

	void StartNewGame();
	void StartNextLevel();

	void Update() override;
	void Render() override;

private:
	Timer* mTimer;
	AudioManager* mAudio;

	BackgroundStars* mStars;
	PlaySideBar* mSideBar;

	Texture* mStartLabel;

	Level* mLevel;

	float mLevelStartTimer;
	float mLevelStartDelay;

	bool mGameStarted;
	bool mLevelStarted;
	int mCurrentStage;
};