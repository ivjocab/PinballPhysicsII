#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "ModulePhysics.h"
#include "Animation.h"

struct Flipper
{
	PhysBody* Rect;
	PhysBody* Circle;
	bool rightSide;
};

struct Kicker
{
	PhysBody* pivot;
	PhysBody* mobile;
	b2PrismaticJoint* kickerJoint;
};

struct SCircle {
	PhysBody* round;
	Animation idleBallAnim;
	Animation deathBallAnim;
	Animation spawnBallAnim;
	Animation idleSunAnim;
	Animation collisionSunAnim;
	Animation idlePachinkoAnim;
	Animation randomPachinkoAnim;
	Animation collisionPachinkoAnim;
};

struct DCircle {
	PhysBody* round;
	Animation idleBallAnim;
	Animation deathBallAnim;
	Animation spawnBallAnim;
	enum State
	{
		Ready,
		Alive,
		Dead
	};
};

struct recSensor {
	enum sensorValue
	{
		NONE = -1,
		DEATH,
		CARD,
		EGG,
		TP,
		PAC_MAN,
		HUNDREDS,
		THOUSAND,
		POINT_BUTTON,
		SAFETY_BUTTON,
		NUMBER_BUTTON
	};
	PhysBody* sensor;
	sensorValue value;
	bool isActive;
	int sensorTimer = 0;
};

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
	p2List<PhysBody*> backgrounds;

	PhysBody* sensor;
	bool sensed;

	SDL_Texture* background;
	SDL_Texture* ballTexture;
	SDL_Texture* sunTexture;
	SDL_Texture* box;
	SDL_Texture* pachinkoTexture;
	PhysBody* right;
	PhysBody* point_right;
	PhysBody* left;
	PhysBody* point_left;
	b2RevoluteJointDef revoluteJointDef_right;
	b2RevoluteJointDef revoluteJointDef_left;

	p2List<Flipper*> flippers;
	SDL_Rect rectSect = { 48, 250, 100, 25 };
	Kicker kicker;
	SDL_Rect kickerSect = { 302, 706, 22, 96 };
	SDL_Rect kickerSectBonus = { 1002, 706, 22, 96 };

	uint clickFx;
	uint kickerFx;

	uint intro;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;

	int bg[106] = {
		560, 999,
		560, 956,
		593, 956,
		617, 952,
		638, 941,
		659, 929,
		680, 909,
		693, 890,
		703, 872,
		705, 858,
		705, 281,
		719, 281,
		719, 994,
		765, 994,
		765, 278, 
		764, 252,
		759, 223,
		748, 194,
		727, 164,
		697, 139,
		658, 119,
		629, 112,
		610, 110, 
		172, 110,
		138, 116,
		116, 125,
		92, 143,
		74, 165,
		56, 194,
		47, 220,
		45, 237,
		45, 277,
		49, 304,
		57, 336,
		76, 378,
		98, 414,
		127, 443,
		45, 564,
		32, 589,
		21, 618,
		18, 629,
		18, 905,
		21, 917,
		28, 936,
		44, 957,
		66, 978,
		88, 989,
		110, 996,
		132, 999,
		0, 1000,
		0, 0,
		780, 0,
		780, 999
	};

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
