#pragma once
#include "PlaySideBar.h"
#include "BackgroundStars.h"

using namespace SDLFramework;

class Level : public GameEntity {
public:
	Level(int stage, PlaySideBar* sidebar);
	~Level();

	void Update();
	void Render();

private:
	Timer* mTimer;
	PlaySideBar* mSideBar;
	BackgroundStars* mBackgroundStars;
	
	int mStage;
	bool mStageStarted;

	Texture* mReadyLabel;
	float mReadyLabelOnScreen;
	float mReadyLabelOffScreen;

	Texture* mStageLabel;
	Scoreboard* mStageNumber;
	float mStageLabelOnScreen;
	float mStageLabelOffScreen;

	float mLabelTimer;

	void StartStage();

};