#pragma once
#include "Collider.h"
#include <vector>

namespace SDLFramework {

	class PhysEntity : public GameEntity {
	public:
		PhysEntity();
		virtual ~PhysEntity();

		unsigned long GetID();

		bool CheckCollision(PhysEntity* other);

		virtual void Hit(PhysEntity* other) { std::cout << "A collision has happened!" << std::endl; }

		virtual void Render() override;

	//protected:
		unsigned long mID;

		std::vector<Collider*> mColliders;

		Collider* mBroadPhaseCollider;

		virtual bool IgnoreCollisions();

		void AddCollider(Collider* collider, Vector2 localPos = Vec2_Zero);
	};
}