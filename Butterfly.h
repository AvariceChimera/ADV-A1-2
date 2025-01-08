#pragma once
#include "Enemy.h"

class Butterfly : public Enemy {
public:
	static void CreateDivePaths();

	Butterfly(int path, int index, bool challenge);
	~Butterfly();

	void Dive(int type = 0) override;

	void Hit(PhysEntity* other) override;

private:
	static std::vector<std::vector<Vector2>> sDivePaths;

	bool mEscort;

	Vector2 LocalFormationPosition() override;

	void HandleDivingState() override;

	void RenderDivingState() override;
};