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

struct SunCircle 
{
	PhysBody* round;
	Animation idleSunAnim;
	Animation collisionSunAnim;
};

struct PachinkoCircle
{
	PhysBody* round;
	Animation idlePachinkoAnim;
	Animation randomPachinkoAnim;
	Animation collisionPachinkoAnim;
	enum Type
	{
		pachinko1Collision,
		pachinko2Collision
	};
};

struct Columns
{
	PhysBody col;
	Animation idleColumnAnim;
};

struct Sheen
{
	PhysBody* sheen;
	Animation idleSheenAnim;
	Animation despawnSheenAnim;
	Animation spawnSheenAnim;
	Animation collisionSheenAnim;
	enum State
	{
		IDLE,
		DESPAWN,
		SPAWN,
		COLLISION
	};
};

struct DCircle {
	PhysBody* round;
	Animation idleBallAnim;
	Animation deathBallAnim;
	Animation spawnBallAnim;
	enum State
	{
		IDLE,
		DEAD,
		SPAWN
	};
	float power;
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

//rottenshit
struct circlerot 
{
	PhysBody* circlerot;
	PhysBody* centercircle;
};

//rottenshit

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
	DCircle* ball;
	SunCircle* sun;
	Columns* columns;
	Sheen* sheen;
	p2List<PachinkoCircle*> pachinkos;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> backgrounds;

	p2List<PhysBody*> circles;


	enum class gameState {
		START,
		GAME
	};

	gameState GameState;

	PhysBody* sensor;
	bool sensed;

	SDL_Texture* StartScreen;
	SDL_Texture* background;
	SDL_Texture* backgroundGame;
	SDL_Texture* ballTexture;
	SDL_Texture* sunTexture;
	SDL_Texture* box;
	SDL_Texture* pachinkoTexture;
	SDL_Texture* columnsTexture;
	SDL_Texture* sheenTexture;
	PhysBody* right;
	PhysBody* point_right;
	PhysBody* left;
	PhysBody* point_left;
	b2RevoluteJointDef revoluteJointDef_right;
	b2RevoluteJointDef revoluteJointDef_left;

	//rottenshit
	circlerot* a;
	circlerot* a2;
	//rottenshit

	p2List<Flipper*> flippers;
	SDL_Rect rectSect = { 48, 250, 100, 25 };
	Kicker* kicker;
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

	int bouncer[18] = {
	562, 869,
	660, 869,
	660, 772,
	636, 778,
	617, 787,
	599, 800,
	586, 814,
	575, 830,
	566, 851
	};

	int rpartition[30] = {
	559, 430,
	662, 430,
	647, 419,
	636, 406,
	627, 388,
	624, 374,
	624, 253,
	558, 253,
	550, 268,
	546, 282,
	544, 296,
	544, 325,
	547, 342,
	552, 359,
	559, 373
	};

	int lpartition[34] = {
	388, 429,
	481, 429,
	481, 372,
	486, 362,
	492, 345,
	496, 325,
	496, 293,
	492, 274,
	487, 259,
	483, 252,
	416, 252,
	416, 373,
	413, 387,
	406, 401,
	398, 413,
	387, 423,
	378, 429
	};

	int rhombus1[8] = {
	182, 532,
	206, 565,
	257, 565,
	233, 532
	};

	int rhombus2[8] = {
	295, 477,
	319, 510,
	370, 510,
	347, 477
	};

	int rhombus3[8] = {
	321, 598,
	345, 631,
	396, 631,
	372, 598
	};

	enum BallState
	{
		BALL_IDLE,
		BALL_DEATH,
		BALL_SPAWN
	};

	enum SheenState
	{
		SHEEN_IDLE,
		SHEEN_DESPAWN,
		SHEEN_SPAWN,
		SHEEN_COLLISION
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

	SheenState sheenState;
	BallState ballState;
	SunState sunState;
	PachinkoState pachinkoState;

	int counter = 0;

private:
	Animation* currentAnim = nullptr;
	Animation idleBallAnim, deathBallAnim, spawnBallAnim, idleSheenAnim, despawnSheenAnim, SpawnSheenAnim, collisionSheenAnim, idleSunAnim, collisionSunAnim,
			  idlePachinkoAnim, randomPachinkoAnim, collisionPachinkoAnim, idleColumnsAnim;
};
