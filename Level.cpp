#include "Level.h"

void Level::StartStage() {
	mStageStarted = true;
}

Level::Level(int stage, PlaySideBar* sideBar, Player* player) {
	mTimer = Timer::Instance();
	mSideBar = sideBar;
	mSideBar->SetLevel(stage);
	mBackgroundStars = BackgroundStars::Instance();

	mStage = stage;
	mStageStarted = false;

	mLabelTimer = 0.0f;

	mStageLabel = new Texture("STAGE", "emulogic.ttf", 32, { 75, 75, 200 });
	mStageLabel->Parent(this);
	mStageLabel->Position(Graphics::SCREEN_WIDTH * 0.35f, Graphics::SCREEN_HEIGHT * 0.5f);

	mStageNumber = new Scoreboard({ 75, 75, 200 });
	mStageNumber->Score(stage);
	mStageNumber->Parent(this);
	mStageNumber->Position(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.5f);

	mStageLabelOnScreen = 0.0f;
	mStageLabelOffScreen = 1.5f;

	mReadyLabel = new Texture("READY?", "emulogic.ttf", 32, { 150,0,0 });
	mReadyLabel->Parent(this);
	mReadyLabel->Position(Graphics::SCREEN_WIDTH * 0.4f, Graphics::SCREEN_HEIGHT * 0.5f);

	mReadyLabelOnScreen = mStageLabelOffScreen;
	mReadyLabelOffScreen = mReadyLabelOnScreen + 3.0f;

	mPlayer = player;
	mPlayerHit = false;
	mRespawnDelay = 3.0f;
	mRespawnLabelOnScreen = 2.0f;

	mCoPilot = new CoPilot();

	mGameOverLabel = new Texture("GAME OVER!", "emulogic.ttf", 32, { 150,0,0 });
	mGameOverLabel->Parent(this);
	mGameOverLabel->Position(Graphics::SCREEN_WIDTH * 0.4f, Graphics::SCREEN_HEIGHT * 0.5f);
	
	mGameOverDelay = 6.0f;
	mGameOverTimer = 0.0f;
	mGameOverLabelOnScreen = 1.0f;

	mCurrentState = Running;

	mButterflyCount = 0;
	mWaspCount = 0;
	mBossCount = 0;

	std::string fullpath = SDL_GetBasePath();
	fullpath.append("Data/LevelOne.xml");
	mSpawningPatterns.LoadFile(fullpath.c_str());

	mChallengeStage = mSpawningPatterns.FirstChildElement("Level")->FirstChildElement()->BoolAttribute("value");

	if (!mChallengeStage) {
		//create formation and initialize enemy arrays
		mFormation = new Formation();
		mFormation->Position(Graphics::SCREEN_WIDTH * 0.4f, 150.0f);
		Enemy::SetFormation(mFormation);

		for (int i = 0; i < MAX_BUTTERFLIES; i++) {
			mFormationButterflies[i] = nullptr;
		}

		for (int i = 0; i < MAX_WASPS; i++) {
			mFormationWasps[i] = nullptr;
		}

		for (int i = 0; i < MAX_BOSSES; i++) {
			mFormationBosses[i] = nullptr;
		}
	}

	mCurrentFlyInPiority = 0;
	mCurrentFlyInIndex = 0;
	mSpawnDelay = 0.2f;
	mSpawnTimer = 0.0f;
	mSpawningFinished = false;

	mDivingButterfly = nullptr;
	mSkipFirstButterfly = false;
	mButterflyDiveDelay = 1.0F;
	mButterflyDiveTimer = 0.0f;

	mDivingWasp = nullptr;
	mDivingWasp2 = nullptr;
	mWaspDiveDelay = 1.0f;
	mWaspDiveTimer = 0.0f;

	mDivingBoss = nullptr;
	mSkipFirstBoss = true;
	mCaptureDive = true;
	mBossDiveDelay = 5.0f;
	mBossDiveTimer = 0.0f;

	Enemy::CurrentPlayer(mPlayer);
}

Level::~Level() {
	mTimer = nullptr;
	mSideBar = nullptr;
	mBackgroundStars = nullptr;

	delete mStageLabel;
	mStageLabel = nullptr;
	delete mStageNumber;
	mStageNumber = nullptr;
	delete mReadyLabel;
	mReadyLabel = nullptr;

	delete mGameOverLabel;
	mGameOverLabel = nullptr;

	mPlayer = nullptr;

	delete mFormation;
	mFormation = nullptr;

	for (int i = 0; i < MAX_BUTTERFLIES; i++) {
		delete mFormationButterflies[i];
		mFormationButterflies[i] = nullptr;
	}

	for (int i = 0; i < MAX_WASPS; i++) {
		delete mFormationWasps[i];
		mFormationWasps[i] = nullptr;
	}

	for (int i = 0; i < MAX_BOSSES; i++) {
		delete mFormationBosses[i];
		mFormationBosses[i] = nullptr;
	}

	for (auto enemy : mEnemies) {
		delete enemy;
		enemy = nullptr;
	}
	mEnemies.clear();
}

void Level::HandleStartLabels() {
	mLabelTimer += mTimer->DeltaTime();

	if (mLabelTimer >= mStageLabelOffScreen) {
		mBackgroundStars->Scroll(true);
		mPlayer->Active(true);
		mPlayer->Visible(true);

		if (mStage > 1) {
			StartStage();
		}
		else {
			//ready only shows at the start of the gameplay
			if (mLabelTimer >= mReadyLabelOffScreen) {
				StartStage();
			}
		}
	}
}

Level::LevelStates Level::State() {
	return mCurrentState;
}

void Level::HandleCollisions() {
	if (!mPlayerHit) {
		if (mPlayer->WasHit()) {
			mSideBar->SetShips(mPlayer->Lives());

			mPlayerHit = true;
			mRespawnTimer = 0.0f;
			mPlayer->Active(false);
			mBackgroundStars->Scroll(false);
		}
		if (mCoPilot->WasHit()) {
			mCoPilot->Active(false);
		}
	}
}

void Level::HandlePlayerDeath() {
	if (!mPlayer->IsAnimating()) {
		if (mPlayer->Lives() > 0) {
			if (mRespawnTimer == 0.0f) {
				//player done death anim and will now respawn
				mPlayer->Visible(false);
			}

			mRespawnTimer += mTimer->DeltaTime();
			if (mRespawnTimer >= mRespawnDelay) {
				//respawning the player
				mPlayer->Active(true);
				mPlayer->Visible(true);
				mPlayerHit = false;
				mBackgroundStars->Scroll(true);
			}
		}
		else {
			//This is GameOver
			if (mGameOverTimer == 0.0f) {
				mPlayer->Visible(false);
			}

			mGameOverTimer += mTimer->DeltaTime();
			if (mGameOverTimer >= mGameOverDelay) {
				mCurrentState = GameOver;
			}
		}
	}
}

void Level::HandleEnemySpawning() {
	mSpawnTimer += mTimer->DeltaTime();

	if (mSpawnTimer >= mSpawnDelay) {
		XMLElement* element = mSpawningPatterns.FirstChildElement("Level")->FirstChild()->NextSiblingElement();
		bool spawned = false;
		bool prioityFound = false;

		while (element != nullptr) {
			//extract xml data

			int priority = element->IntAttribute("priority");
			if (mCurrentFlyInPiority == priority) {
				prioityFound = true;
				int path = element->IntAttribute("path");
				XMLElement* child = element->FirstChildElement();
				int random = 50;
				//this loop will always give the next/last "child" based on the FlyInIndex
				for (int i = 0; i < mCurrentFlyInIndex && child != nullptr; i++) {
					child = child->NextSiblingElement();
				}

				if (child != nullptr) {
					std::string type = child->Attribute("type");
					int index = child->IntAttribute("index");

					if (type.compare("Butterfly") == 0) {
						if (!mChallengeStage) {
							//add butterfly to formation
							mFormationButterflies[index] = new Butterfly(path, index, false);
							mButterflyCount++;
						}
						else {
							// change to true if/when challenge stages are made
							mEnemies.push_back(new Butterfly(path, index, false));
						}
					}
					else if (type.compare("Wasp") == 0) {
						if (!mChallengeStage) {
							mFormationWasps[index] = new Wasp(path, index, false, false);
							mWaspCount++;
						}
						else {
							mEnemies.push_back(new Wasp(path, index, false, false));
						}
					}
					else if (type.compare("Boss") == 0) {
						if (!mChallengeStage) {
							mFormationBosses[index] = new Boss(path, index, false);
							mBossCount++;
						}
						else {
							mEnemies.push_back(new Boss(path, index, false));
						}
					}

					spawned = true;
				}
			}

			element = element->NextSiblingElement();
		}

		if (!prioityFound) {
			//no more enemies in the line
			mSpawningFinished = true;
		}
		else {
			if (!spawned) {
				//enemie sets waiting to be spawned
				if (!EnemyFlyingIn()) {
					mCurrentFlyInPiority++;
					mCurrentFlyInIndex = 0;
				}
			}
			else {
				//havent finished spawning the set
				mCurrentFlyInIndex++;
			}
		}

		mSpawnTimer = 0.0f;
	}
}

bool Level::EnemyFlyingIn() {
	for (Butterfly* butterfly : mFormationButterflies) {
		if (butterfly != nullptr &&
			butterfly->CurrentState() == Enemy::FlyIn) {
			return true;
		}
	}

	for (Wasp* wasp : mFormationWasps) {
		if (wasp != nullptr &&
			wasp->CurrentState() == Enemy::FlyIn) {
			return true;
		}
	}

	for (Boss* boss : mFormationBosses) {
		if (boss != nullptr &&
			boss->CurrentState() == Enemy::FlyIn) {
			return true;
		}
	}

	return false;
}

void Level::HandleEnemyFormation() {
	mFormation->Update();

	bool levelCleared = mSpawningFinished;

	for (Butterfly* butterfly : mFormationButterflies) {
		if (butterfly != nullptr) {
			butterfly->Update();
			if (butterfly->CurrentState() != Enemy::Dead || butterfly->InDeathAnimation()){
				levelCleared = false;
			}
		}
	}

	for (Wasp* wasp : mFormationWasps) {
		if (wasp != nullptr) {
			wasp->Update();
			if (wasp->CurrentState() != Enemy::Dead || wasp->InDeathAnimation()) {
				levelCleared = false;
			}
		}
	}

	for (Boss* boss : mFormationBosses) {
		if (boss != nullptr) {
			boss->Update();
			if (boss->CurrentState() != Enemy::Dead || boss->InDeathAnimation()) {
				levelCleared = false;
			}
		}
	}

	if (!mFormation->Locked()) {
		if (mButterflyCount == MAX_BUTTERFLIES && mWaspCount == MAX_WASPS && mBossCount == MAX_BOSSES) {
			if (!EnemyFlyingIn()) {
				mFormation->Lock();
			}
		}
	}
	else {
		HandleEnemyDiving();
	}

	if (levelCleared) {
		mCurrentState = Finished;
	}

}

void Level::HandleEnemyDiving() {
	//butterfly dive logic
	if (mDivingButterfly == nullptr) {
		mButterflyDiveTimer += mTimer->DeltaTime();

		if (mButterflyDiveTimer >= mButterflyDiveDelay) {
			bool skipped = false;

			for (int i = MAX_BUTTERFLIES - 1; i >= 0; i--) {
				if (mFormationButterflies[i] != nullptr &&
					mFormationButterflies[i]->CurrentState() == Enemy::InFormation) {
					if (!mSkipFirstButterfly || (mSkipFirstButterfly && skipped)) {
						mDivingButterfly = mFormationButterflies[i];
						mDivingButterfly->Dive();
						mSkipFirstButterfly = !mSkipFirstButterfly;
						break; //jobs done, leave loop
					}
				}
				skipped = true;
			}

			mButterflyDiveTimer = 0.0f;
		}
	}
	else {
		if (mDivingButterfly->CurrentState() != Enemy::Diving) {
			mDivingButterfly = nullptr;
		}
	}

	//wasp dive

	mWaspDiveTimer += mTimer->DeltaTime();
	if (mWaspDiveTimer >= mWaspDiveDelay) {
		for (int i = MAX_WASPS - 1; i >= 0; i--) {
			if (mFormationWasps[i]->CurrentState() == Enemy::InFormation) {
				if (mDivingWasp == nullptr) {
					mDivingWasp = mFormationWasps[i];
					mDivingWasp->Dive();
				}
				else if (mDivingWasp2 == nullptr) {
					mDivingWasp2 = mFormationWasps[i];
					mDivingWasp2->Dive();
				}
				break;
			}
		}
		mWaspDiveTimer = 0.0f;
	}

	if (mDivingWasp != nullptr && mDivingWasp->CurrentState() != Enemy::Diving) {
		mDivingWasp = nullptr;
	}
	if (mDivingWasp2 != nullptr && mDivingWasp2->CurrentState() != Enemy::Diving) {
		mDivingWasp2 = nullptr;
	}

	//boss dive

	if (mDivingBoss == nullptr) {
		mBossDiveTimer += mTimer->DeltaTime();
		if (mBossDiveTimer >= mBossDiveDelay) {

			bool skipped = false;

			for (int i = MAX_BOSSES - 1; i >= 0; i--) {
				if (mFormationBosses[i]->CurrentState() == Enemy::InFormation) {
					if (!mSkipFirstBoss || (mSkipFirstBoss && skipped)) {
						mDivingBoss = mFormationBosses[i];

						if (mCaptureDive) {
							mDivingBoss->Dive(1);
						}
						else {
							mDivingBoss->Dive();

							int index = mDivingBoss->Index();
							int firstEscortIndex = (index % 2 == 0) ? (index * 2) : (index * 2 - 1);
							int secondEscortIndex = firstEscortIndex + 4;

							if (mFormationButterflies[firstEscortIndex]->CurrentState() == Enemy::InFormation) {
								mFormationButterflies[firstEscortIndex]->Dive(1);
							}
							if (mFormationButterflies[secondEscortIndex]->CurrentState() == Enemy::InFormation) {
								mFormationButterflies[secondEscortIndex]->Dive(1);
							}
						}

						mSkipFirstBoss = !mSkipFirstBoss;
						mCaptureDive = !mCaptureDive;
						break;
					}
					skipped = true;
				}
			}

			mBossDiveTimer = 0.0f;
		}
	}
	else {
		if (mDivingBoss->CurrentState() != Enemy::Diving) {
			mDivingBoss = nullptr;
		}
	}

}

void Level::Update() {
	if (!mStageStarted) {
		HandleStartLabels();
	}
	else {
		if (!mSpawningFinished) {
			HandleEnemySpawning();
		}

		if (!mChallengeStage) {
			HandleEnemyFormation();
		}
		else {
			for (auto enemy : mEnemies) {
				enemy->Update();
			}
		}

		HandleCollisions();

		if (mPlayerHit) {
			HandlePlayerDeath();
		}
		else {
			if (InputManager::Instance()->KeyPressed(SDL_SCANCODE_P)) {
				mCurrentState = Finished;
			}
		}
	}
}

void Level::Render() {
	if (!mStageStarted) {
		if (mLabelTimer > mStageLabelOnScreen &&
			mLabelTimer < mStageLabelOffScreen) {

			mStageLabel->Render();
			mStageNumber->Render();
		}

		else if (mLabelTimer > mReadyLabelOnScreen && mLabelTimer < mReadyLabelOffScreen) {
			mReadyLabel->Render();
		}
	}

	else {
		if (!mChallengeStage) {
			for (Butterfly* butterfly : mFormationButterflies) {
				if (butterfly != nullptr) {
					butterfly->Render();
				}
			}

			for (Wasp* wasp : mFormationWasps) {
				if (wasp != nullptr) {
					wasp->Render();
				}
			}

			for (Boss* boss : mFormationBosses) {
				if (boss != nullptr) {
					boss->Render();
				}
			}
		}

		else {
			for (auto enemy : mEnemies) {
				enemy->Render();
			}
		}

		if (mPlayerHit) {
			if (mRespawnTimer >= mRespawnLabelOnScreen) {
				mReadyLabel->Render();
			}

			if (mGameOverTimer >= mGameOverLabelOnScreen) {
				mGameOverLabel->Render();
			}
		}
	}
}