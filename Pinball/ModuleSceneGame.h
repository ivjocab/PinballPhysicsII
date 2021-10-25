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

	// Pivot 0, 0
	int pachinko1[28] = {
		206, 632,
		206, 643,
		228, 643,
		238, 647,
		246, 653,
		253, 662,
		257, 671,
		257, 917,
		271, 917,
		271, 672,
		266, 659,
		258, 647,
		246, 637,
		233, 632		
	};

	int pachinko2[68] = {
	154, 631,
	130, 631,
	120, 633,
	108, 641,
	98, 651,
	91, 664,
	90, 673,
	90, 839,
	95, 861,
	102, 883,
	114, 904,
	131, 923,
	149, 938,
	169, 951,
	194, 959,
	213, 964,
	235, 968,
	271, 968,
	271, 956,
	232, 956,
	207, 950,
	179, 938,
	156, 925,
	138, 909,
	124, 892,
	115, 874,
	105, 843,
	104, 833,
	104, 668,
	109, 658,
	117, 650,
	127, 644,
	135, 642,
	154, 642
	};

	int cloud[120] = {
	231, 437,
	219, 432,
	205, 423,
	190, 409,
	180, 393,
	177, 383,
	173, 377,
	165, 370,
	157, 356,
	153, 346,
	153, 328,
	167, 299,
	169, 294,
	169, 281,
	165, 272,
	156, 264,
	156, 252,
	158, 245,
	167, 230,
	184, 212,
	205, 198,
	229, 192,
	240, 192,
	248, 197,
	250, 202,
	247, 209,
	243, 215,
	243, 224,
	251, 230,
	263, 230,
	270, 226,
	282, 226,
	289, 221,
	295, 214,
	295, 205,
	292, 197,
	287, 192,
	289, 188,
	289, 183,
	284, 176,
	273, 171,
	261, 171,
	246, 176,
	230, 184,
	211, 190,
	186, 203,
	170, 214,
	152, 234,
	143, 248,
	135, 274,
	132, 289,
	132, 317,
	135, 339,
	140, 355,
	149, 370,
	160, 381,
	173, 401,
	190, 420,
	203, 427,
	225, 437
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
