#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Animation.h"

class PhysBody;

class ModuleSceneGame : public Module
{
public:
	ModuleSceneGame(Application* app, bool start_enabled = true);
	~ModuleSceneGame();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;

	PhysBody* sensor;
	bool sensed;

	SDL_Texture* background;
	SDL_Texture* ball;
	SDL_Texture* box;
	SDL_Texture* rick;
	uint intro;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;

	enum BallState
	{
		IDLE,
		DEATH,
		SPAWN
	};

	BallState ballState;

private:
	Animation* currentAnim = nullptr;
	Animation idleAnim, collisionAnim, deathAnim, spawnAnim;
};
