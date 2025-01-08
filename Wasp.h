#pragma once
#include "Enemy.h"

class Wasp : public Enemy {
public:

	static void CreateDivePaths();

	Wasp(int path, int index, bool challenge, bool diver);
	~Wasp();

	void Hit(PhysEntity* other) override;

private:
	static std::vector<std::vector<Vector2>> sDivePaths;

	bool mDiver;

	void FlyInComplete() override;

	Vector2 LocalFormationPosition() override;

	void HandleDivingState() override;

	void RenderDivingState() override;
};