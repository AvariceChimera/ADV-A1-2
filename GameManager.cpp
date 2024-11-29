#include "GameManager.h"
#include "BoxCollider.h"

namespace SDLFramework {
    GameManager* GameManager::sInstance = nullptr;

    GameManager* GameManager::Instance() {
        if (sInstance == nullptr) {
            sInstance = new GameManager();
        }

        return sInstance;
    }

    void GameManager::Release() {
        delete sInstance;
        sInstance = nullptr;
    }

    void GameManager::Run() {
        //Main Game Loop
        while (!mQuit) {
            mTimer->Update();
            //Event Polling Loop
            //While there are events inside of our events varaible...
            while (SDL_PollEvent(&mEvents)) {
                //Handle each and every event we want to look for
                switch (mEvents.type) {
                case SDL_QUIT:
                    mQuit = true;
                    break;
                }
            }

            if (mTimer->DeltaTime() >= 1.0f / FRAME_RATE) {
                mTimer->Reset();
                Update();
                LateUpdate();
                Render();
            }
        }
    }

    void GameManager::Update() {
        mInputManager->Update();
        //mScreenManager->Update();

        if (mInputManager->KeyDown(SDL_SCANCODE_W)) {
            mGreenGalaga->Translate(Vector2(0.0f, -40.0f) * mTimer->DeltaTime(), GameEntity::Local);
            mFriendly->Translate(Vector2(0.0f, -40.0f) * mTimer->DeltaTime(), GameEntity::Local);
        }
        else if (mInputManager->KeyDown(SDL_SCANCODE_S)) {
            mGreenGalaga->Translate(Vector2(0.0f, 40.0f) * mTimer->DeltaTime(), GameEntity::Local);
            mFriendly->Translate(Vector2(0.0f, 40.0f) * mTimer->DeltaTime(), GameEntity::Local);
        }
        //To prevent diagonal movement, add an else to the if statement below
        if (mInputManager->KeyDown(SDL_SCANCODE_A)) {
            mGreenGalaga->Translate(Vector2(-40.0f, 0.0f) * mTimer->DeltaTime(), GameEntity::Local);
            mFriendly->Translate(Vector2(-40.0f, 0.0f) * mTimer->DeltaTime(), GameEntity::Local);
        }
        else if (mInputManager->KeyDown(SDL_SCANCODE_D)) {
            mGreenGalaga->Translate(Vector2(40.0f, 0.0f) * mTimer->DeltaTime(), GameEntity::Local);
            mFriendly->Translate(Vector2(40.0f, 0.0f) * mTimer->DeltaTime(), GameEntity::Local);
        }

        if (mInputManager->KeyDown(SDL_SCANCODE_Q)) {
            mGreenGalaga->Rotation(mGreenGalaga->Rotation(GameEntity::Local) + 180.0f * mTimer->DeltaTime());
        }
        else if (mInputManager->KeyDown(SDL_SCANCODE_E)) {
            mGreenGalaga->Rotation(mGreenGalaga->Rotation(GameEntity::Local) + -180.0f * mTimer->DeltaTime());
        }

        if (mInputManager->KeyDown(SDL_SCANCODE_Z)) {
            mGreenGalaga->Scale(mGreenGalaga->Scale() + Vector2(0.1f, 0.1f));
        }
        else if (mInputManager->KeyDown(SDL_SCANCODE_C)) {
            mGreenGalaga->Scale(mGreenGalaga->Scale() - Vector2(0.1f, 0.1f));
        }

        if (mInputManager->KeyDown(SDL_SCANCODE_KP_8)) {
            mPurpleGalaga->Translate(Vector2(0.0f, -40.0f) * mTimer->DeltaTime(), GameEntity::Local);
            mHostile->Translate(Vector2(0.0f, -40.0f) * mTimer->DeltaTime(), GameEntity::Local);
        }
        else if (mInputManager->KeyDown(SDL_SCANCODE_KP_2)) {
            mPurpleGalaga->Translate(Vector2(0.0f, 40.0f) * mTimer->DeltaTime(), GameEntity::Local);
            mHostile->Translate(Vector2(0.0f, 40.0f) * mTimer->DeltaTime(), GameEntity::Local);
        }

        if (mInputManager->KeyDown(SDL_SCANCODE_KP_4)) {
            mPurpleGalaga->Translate(Vector2(-40.0f, 0.0f) * mTimer->DeltaTime(), GameEntity::Local);
            mHostile->Translate(Vector2(-40.0f, 0.0f) * mTimer->DeltaTime(), GameEntity::Local);
        }
        else if (mInputManager->KeyDown(SDL_SCANCODE_KP_6)) {
            mPurpleGalaga->Translate(Vector2(40.0f, 0.0f) * mTimer->DeltaTime(), GameEntity::Local);
            mHostile->Translate(Vector2(40.0f, 0.0f) * mTimer->DeltaTime(), GameEntity::Local);
        }

        if (mInputManager->KeyDown(SDL_SCANCODE_KP_7)) {
            mPurpleGalaga->Rotation(mPurpleGalaga->Rotation(GameEntity::Local) + 180.0f * mTimer->DeltaTime());
        }
        else if (mInputManager->KeyDown(SDL_SCANCODE_KP_9)) {
            mPurpleGalaga->Rotation(mPurpleGalaga->Rotation(GameEntity::Local) + -180.0f * mTimer->DeltaTime());
        }

        if (mInputManager->KeyDown(SDL_SCANCODE_KP_1)) {
            mPurpleGalaga->Scale(mPurpleGalaga->Scale() + Vector2(0.1f, 0.1f));
        }
        else if (mInputManager->KeyDown(SDL_SCANCODE_KP_3)) {
            mPurpleGalaga->Scale(mPurpleGalaga->Scale() - Vector2(0.1f, 0.1f));
        }

        if (mInputManager->KeyPressed(SDL_SCANCODE_RETURN)) {
            mAudioManager->PlaySFX("coin_credit.wav", 0, -1);
        }

        mFriendly->Update();
        mFriendlyShot->Update();
        mHostile->Update();
        mHostileShot->Update();
    }

    void GameManager::LateUpdate() {
        mPhysicsManager->Update();
        mInputManager->UpdatePrevInput();
    }

    void GameManager::Render() {
        //This is the old frame we need to clear
        mGraphics->ClearBackBuffer();
        mScreenManager->Render();
        mGreenGalaga->Render();
        mPurpleGalaga->Render();
        mFriendly->Render();
        mFriendlyShot->Render();
        mHostile->Render();
        mHostileShot->Render();
        //Actually showing everthing that we have told to render
        mGraphics->Render();
    }

    GameManager::GameManager() : mQuit(false) {
        //calling to our Graphics Singleton
        mGraphics = Graphics::Instance();

        if (!Graphics::Initialized()) {
            mQuit = true;
        }

        //Initialize all other modules
        mTimer = Timer::Instance();
        mAssetManager = AssetManager::Instance();
        mInputManager = InputManager::Instance();
        mAudioManager = AudioManager::Instance();
        mPhysicsManager = PhysicsManager::Instance();
        mScreenManager = ScreenManager::Instance();

        //Create the Physics Layers
        mPhysicsManager->SetLayerCollisionMask(PhysicsManager::CollisionLayers::Friendly,
            PhysicsManager::CollisionFlags::Hostile |
            PhysicsManager::CollisionFlags::HostileProjectile);

        mPhysicsManager->SetLayerCollisionMask(PhysicsManager::CollisionLayers::Hostile,
            PhysicsManager::CollisionFlags::Friendly |
            PhysicsManager::CollisionFlags::FriendlyProjectile);

        mPhysicsManager->SetLayerCollisionMask(PhysicsManager::CollisionLayers::FriendlyProjectile,
            PhysicsManager::CollisionFlags::Hostile |
            PhysicsManager::CollisionFlags::HostileProjectile);

        mPhysicsManager->SetLayerCollisionMask(PhysicsManager::CollisionLayers::HostileProjectile,
            PhysicsManager::CollisionFlags::Friendly |
            PhysicsManager::CollisionFlags::FriendlyProjectile);

        //Create 4 PhysEntity* objects, assign one to each layer, and have one be able to move so we can test collisions
        mFriendly = new PhysEntity();
        mFriendly->AddCollider(new BoxCollider(Vector2(20.0f, 20.0f)));
        mFriendly->Parent(mGreenGalaga);
        mFriendly->Position(Vector2(Graphics::SCREEN_WIDTH * 0.3f, Graphics::SCREEN_HEIGHT * 0.49f));
        mFriendly->mID = PhysicsManager::Instance()->RegisterEntity(mFriendly, PhysicsManager::CollisionLayers::Friendly);

        mHostile = new PhysEntity();
        mHostile->AddCollider(new BoxCollider(Vector2(20.0f, 20.0f)));
        mHostile->Position(Vector2(Graphics::SCREEN_WIDTH * 0.2f, Graphics::SCREEN_HEIGHT * 0.49f));
        mHostile->Parent(mPurpleGalaga);
        mHostile->mID = PhysicsManager::Instance()->RegisterEntity(mHostile, PhysicsManager::CollisionLayers::Hostile);

        mFriendlyShot = new PhysEntity();
        mFriendlyShot->AddCollider(new BoxCollider(Vector2(20.0f, 20.0f)));
        mFriendlyShot->Position(Vector2(Graphics::SCREEN_WIDTH * 0.4f, Graphics::SCREEN_HEIGHT * 0.49f));
        mFriendlyShot->mID = PhysicsManager::Instance()->RegisterEntity(mFriendlyShot, PhysicsManager::CollisionLayers::FriendlyProjectile);

        mHostileShot = new PhysEntity();
        mHostileShot->AddCollider(new BoxCollider(Vector2(20.0f, 20.0f)));
        mHostileShot->Position(Vector2(Graphics::SCREEN_WIDTH * 0.5f, Graphics::SCREEN_HEIGHT * 0.49f));
        mHostileShot->mID = PhysicsManager::Instance()->RegisterEntity(mHostileShot, PhysicsManager::CollisionLayers::HostileProjectile);
        
        //game objects made below
        mGreenGalaga = new Texture("SpriteSheet.png", 182, 100, 20, 20);
        mGreenGalaga->Position(Vector2(Graphics::SCREEN_WIDTH * 0.3f, Graphics::SCREEN_HEIGHT * 0.49f));
        mGreenGalaga->Scale(Vector2(2.0f, 2.0f));

        mPurpleGalaga = new Texture("SpriteSheet.png", 182, 125, 20, 20);
        mPurpleGalaga->Position(Vector2(Graphics::SCREEN_WIDTH * 0.2f, Graphics::SCREEN_HEIGHT * 0.49f));
        mPurpleGalaga->Scale(Vector2(2.0f, 2.0f));
    }

        GameManager::~GameManager() {

        //Release Modules
        Graphics::Release();
        mGraphics = nullptr;

        Timer::Release();
        mTimer = nullptr;

        AssetManager::Release();
        mAssetManager = nullptr;

        InputManager::Release();
        mInputManager = nullptr;

        AudioManager::Release();
        mAudioManager = nullptr;

        PhysicsManager::Realease();
        mPhysicsManager = nullptr;

        ScreenManager::Release();
        mScreenManager = nullptr;

        //Quit SDl Subsystems
        SDL_Quit();
    }
    //Namespace bracket is below
}