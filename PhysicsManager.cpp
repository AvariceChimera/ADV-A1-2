#include "PhysicsManager.h"

namespace SDLFramework {
	PhysicsManager* PhysicsManager::sInstance = nullptr;

	PhysicsManager* PhysicsManager::Instance() {
		if (sInstance == nullptr) {
			sInstance = new PhysicsManager();
		}

		return sInstance;
	}

	void PhysicsManager::Realease() {
		delete sInstance;
		sInstance = nullptr;
	}

	PhysicsManager::PhysicsManager() {
		mLastID = 0;

		for (int i = 0; i < static_cast<unsigned int>(CollisionLayers::MaxLayers); i++)
			mLayerMasks[i] = std::bitset < static_cast<unsigned int>(CollisionLayers::MaxLayers) > (static_cast<unsigned int>(CollisionFlags::None));
	}

	PhysicsManager::~PhysicsManager() {
		for (auto layer : mCollisionLayers)
			layer.clear();
	}

	void PhysicsManager::SetLayerCollisionMask(CollisionLayers layer, CollisionFlags flag) {
		mLayerMasks[static_cast<unsigned int>(layer)] =
			std::bitset<static_cast<unsigned int>(CollisionLayers::MaxLayers)>(static_cast<unsigned int>(flag));
	}

	unsigned long PhysicsManager::RegisterEntity(PhysEntity* entity, CollisionLayers layer) {
		mCollisionLayers[static_cast<unsigned int>(layer)].push_back(entity);
		mLastID++;
		return mLastID;
	}

	void PhysicsManager::UnregisterEntity(unsigned long id) {
		bool found = false;

		//loops through all layers
		for (int i = 0; i < static_cast<unsigned int>(CollisionLayers::MaxLayers) && !found; i++) {
			//loop through all OBJECTS on the layer
			for (int j = 0; j < mCollisionLayers[i].size() && !found; j++) {
				//i == layer j == entity
				if (mCollisionLayers[i][j]->GetID() == id)
					mCollisionLayers[i].erase(mCollisionLayers[i].begin() + j);
				found = true;
			}
		}
	}
	void PhysicsManager::Update() {
		for (int i = 0; i < static_cast<unsigned int>(CollisionLayers::MaxLayers); i++) {

			for (int j = 0; j < static_cast<unsigned int>(CollisionLayers::MaxLayers); j++) {

				if (mLayerMasks[i].test(j) && i <= j) {

					for (int k = 0; k < mCollisionLayers[i].size(); k++) {

						for (int l = 0; l < mCollisionLayers[j].size(); l++) {

							if (mCollisionLayers[i][k]->CheckCollision(mCollisionLayers[j][l])) {


								mCollisionLayers[i][k]->Hit(mCollisionLayers[j][l]);

								mCollisionLayers[j][l]->Hit(mCollisionLayers[i][k]);
							}
						}
					}
				}
			}
		}
	}

}