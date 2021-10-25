#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "ModulePhysics.h"
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

	PhysBody* sensor;
	bool sensed;

	SDL_Texture* background;
	SDL_Texture* ball;
	SDL_Texture* sun;
	SDL_Texture* box;
	SDL_Texture* pachinko;
	PhysBody* right;
	PhysBody* point_right;
	PhysBody* left;
	PhysBody* point_left;
	b2RevoluteJointDef revoluteJointDef_right;
	b2RevoluteJointDef revoluteJointDef_left;

	uint intro;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;

	enum BallState
	{
		BALL_IDLE,
		BALL_DEATH,
		BALL_SPAWN
	};

	enum SunState
	{
		SUN_IDLE,
		SUN_COLLISION
	};

	enum PachinkoState
	{
		PACHINKO_IDLE,
		PACHINKO_RANDOM,
		PACHINKO_COLLISION
	};

	BallState ballState;
	SunState sunState;
	PachinkoState pachinkoState;

private:
	Animation* currentAnim = nullptr;
	Animation idleBallAnim, deathBallAnim, spawnBallAnim, idleSunAnim, collisionSunAnim, 
			  idlePachinkoAnim, randomPachinkoAnim, collisionPachinkoAnim;
};
