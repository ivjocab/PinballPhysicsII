#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneGame.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "time.h"

ModuleSceneGame::ModuleSceneGame(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	background = ballTexture = pachinkoTexture = sunTexture = box = StartScreen = NULL;
	ray_on = false;
	sensed = false;
}

ModuleSceneGame::~ModuleSceneGame()
{}

// Load assets
bool ModuleSceneGame::Start()
{
	LOG("Loading Game assets & variables needed");

	b2Vec2 veca = { -0.80, 0 };
	b2Vec2 vecb = { 0, 0 };

	//CREATING BODIES AND OBSTACLES
	//BACKGROUND chains
	backgrounds.add(App->physics->CreateChain(0, 0, bg, 106, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, pachinko1, 28, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, pachinko2, 68, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, cloud, 120, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, bouncer, 18, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, rpartition, 30, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, lpartition, 34, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, rhombus1, 8, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, rhombus2, 8, b2_staticBody));
	backgrounds.add(App->physics->CreateChain(0, 0, rhombus3, 8, b2_staticBody));

	// Flippers --------------------------------------------------------------
	//Left Flipper
	Flipper* f1 = new Flipper;
	f1->Circle = App->physics->CreateCircle(285, 965, 4, b2_staticBody);
	f1->Rect = App->physics->CreateRectangle(265 + rectSect.w / 2, 945 + rectSect.h / 2, rectSect.w, rectSect.h - 15, b2_dynamicBody);
	f1->rightSide = false;
	App->physics->CreateRevoluteJoint(f1->Rect, veca, f1->Circle, vecb, 20.0f);
	flippers.add(f1);

	//Right Flipper
	veca = { 0.80,0 };

	Flipper* f2 = new Flipper;
	f2->Circle = App->physics->CreateCircle(545, 965, 4, b2_staticBody);
	f2->Rect = App->physics->CreateRectangle(525 - rectSect.w / 2, 945 + rectSect.h / 2, rectSect.w, rectSect.h - 15, b2_dynamicBody);
	f2->rightSide = true;
	App->physics->CreateRevoluteJoint(f2->Rect, veca, f2->Circle, vecb, 20.0f);
	flippers.add(f2);

	//rottenshit
	veca = { 1.5,0 };

	a = new circlerot;
	a->circlerot = App->physics->CreateCircle(500, 500, 10, b2_dynamicBody);
	a->centercircle = App->physics->CreateCircle(600, 600, 5, b2_staticBody);
	App->physics->CreateRevoluteJoint(a->circlerot, veca,a->centercircle,vecb, 180,true,false);
	a->circlerot->body->SetGravityScale(0);
	//a->circlerot->body->SetAngularDamping(0);
	//rottenshit
	a->circlerot->body->SetAngularVelocity(100);
	//rottenshit
	//rottenshit

	//Create Kicker
	/*kicker = new Kicker;
	kicker->mobile = App->physics->CreateRectangle(742, 950, 40, 20, b2_kinematicBody);
	kicker->pivot = App->physics->CreateRectangle(742, 970, 20, 20, b2_staticBody);
	kicker->kickerJoint = App->physics->CreatePrismaticJoint(kicker->mobile, { 0, 3 }, kicker->pivot, { 0, 1 }, { 0, 1 }, 20.0f, true, true);*/
	

	//Create BALL
	ball = new DCircle;
	ball->round = App->physics->CreateCircle(742, 835, 12, b2_dynamicBody);

	//Ball Animations
	//Idle ball animations
	ball->idleBallAnim.PushBack({ 0, 0, 39, 38 });
	ball->idleBallAnim.PushBack({ 38, 0, 39, 38 });
	ball->idleBallAnim.PushBack({ 76, 0, 39, 38 });
	ball->idleBallAnim.PushBack({ 114, 0, 39, 38 });
	ball->idleBallAnim.PushBack({ 152, 0, 39, 38 });
	ball->idleBallAnim.PushBack({ 190, 0, 39, 38 });
	ball->idleBallAnim.PushBack({ 228, 0, 39, 38 });
	ball->idleBallAnim.PushBack({ 266, 0, 39, 38 });
	ball->idleBallAnim.PushBack({ 304, 0, 39, 38 });
	ball->idleBallAnim.PushBack({ 342, 0, 39, 38 });
	ball->idleBallAnim.PushBack({ 380, 0, 39, 38 });
	ball->idleBallAnim.PushBack({ 418, 0, 39, 38 });
	ball->idleBallAnim.PushBack({ 456, 0, 39, 38 });
	ball->idleBallAnim.PushBack({ 494, 0, 39, 38 });
	ball->idleBallAnim.PushBack({ 532, 0, 39, 38 });
	ball->idleBallAnim.PushBack({ 570, 0, 39, 38 });
	ball->idleBallAnim.loop = true;
	ball->idleBallAnim.mustFlip = false;
	ball->idleBallAnim.speed = 0.5f;
	//Death ball animations
	ball->deathBallAnim.PushBack({ 0, 39, 39, 38 });
	ball->deathBallAnim.PushBack({ 38, 39, 39, 38 });
	ball->deathBallAnim.PushBack({ 76, 39, 39, 38 });
	ball->deathBallAnim.PushBack({ 114, 39, 39, 38 });
	ball->deathBallAnim.PushBack({ 152, 39, 39, 38 });
	ball->deathBallAnim.PushBack({ 190, 39, 39, 38 });
	ball->deathBallAnim.PushBack({ 228, 39, 39, 38 });
	ball->deathBallAnim.PushBack({ 266, 39, 39, 38 });
	ball->deathBallAnim.PushBack({ 304, 39, 39, 38 });
	ball->deathBallAnim.PushBack({ 342, 39, 39, 38 });
	ball->deathBallAnim.PushBack({ 380, 39, 39, 38 });
	ball->deathBallAnim.PushBack({ 418, 39, 39, 38 });
	ball->deathBallAnim.PushBack({ 456, 39, 39, 38 });
	ball->deathBallAnim.PushBack({ 494, 39, 39, 38 });
	ball->deathBallAnim.PushBack({ 532, 39, 39, 38 });
	ball->deathBallAnim.PushBack({ 570, 39, 39, 38 });
	ball->deathBallAnim.PushBack({ 0, 78, 39, 38 });
	ball->deathBallAnim.PushBack({ 38, 78, 39, 38 });
	ball->deathBallAnim.PushBack({ 76, 78, 39, 38 });
	ball->deathBallAnim.PushBack({ 114, 78, 39, 38 });
	ball->deathBallAnim.PushBack({ 152, 78, 39, 38 });
	ball->deathBallAnim.PushBack({ 190, 78, 39, 38 });
	ball->deathBallAnim.PushBack({ 228, 78, 39, 38 });
	ball->deathBallAnim.PushBack({ 266, 78, 39, 38 });
	ball->deathBallAnim.loop = false;
	ball->deathBallAnim.mustFlip = false;
	ball->deathBallAnim.speed = 0.2f;
	//Spawn ball animations
	ball->spawnBallAnim.PushBack({ 266, 78, 39, 38 });
	ball->spawnBallAnim.PushBack({ 228, 78, 39, 38 });
	ball->spawnBallAnim.PushBack({ 190, 78, 39, 38 });
	ball->spawnBallAnim.PushBack({ 152, 78, 39, 38 });
	ball->spawnBallAnim.PushBack({ 114, 78, 39, 38 });
	ball->spawnBallAnim.PushBack({ 76, 78, 39, 38 });
	ball->spawnBallAnim.PushBack({ 38, 78, 39, 38 });
	ball->spawnBallAnim.PushBack({ 0, 78, 39, 38 });
	ball->spawnBallAnim.PushBack({ 570, 39, 39, 38 });
	ball->spawnBallAnim.PushBack({ 532, 39, 39, 38 });
	ball->spawnBallAnim.PushBack({ 494, 39, 39, 38 });
	ball->spawnBallAnim.PushBack({ 456, 39, 39, 38 });
	ball->spawnBallAnim.PushBack({ 418, 39, 39, 38 });
	ball->spawnBallAnim.PushBack({ 380, 39, 39, 38 });
	ball->spawnBallAnim.PushBack({ 342, 39, 39, 38 });
	ball->spawnBallAnim.PushBack({ 304, 39, 39, 38 });
	ball->spawnBallAnim.PushBack({ 266, 39, 39, 38 });
	ball->spawnBallAnim.PushBack({ 228, 39, 39, 38 });
	ball->spawnBallAnim.PushBack({ 190, 39, 39, 38 });
	ball->spawnBallAnim.PushBack({ 152, 39, 39, 38 });
	ball->spawnBallAnim.PushBack({ 114, 39, 39, 38 });
	ball->spawnBallAnim.PushBack({ 76, 39, 39, 38 });
	ball->spawnBallAnim.PushBack({ 38, 39, 39, 38 });
	ball->spawnBallAnim.PushBack({ 0, 39, 39, 38 });
	ball->spawnBallAnim.loop = false;
	ball->spawnBallAnim.mustFlip = false;
	ball->spawnBallAnim.speed = 0.2f;

	//Create SUN obstacle
	sun = new SunCircle;
	sun->round = App->physics->CreateCircle(334, 340, 56, b2_staticBody);
	//Idle sun animations
	sun->idleSunAnim.PushBack({ 0, 0, 220, 218 });
	sun->idleSunAnim.PushBack({ 218, 0, 220, 218 });
	sun->idleSunAnim.PushBack({ 436, 0, 220, 218 });
	sun->idleSunAnim.PushBack({ 0, 220, 220, 218 });
	sun->idleSunAnim.PushBack({ 218, 220, 220, 218 });
	sun->idleSunAnim.PushBack({ 436, 220, 220, 218 });
	sun->idleSunAnim.PushBack({ 0, 440, 220, 218 });
	sun->idleSunAnim.PushBack({ 218, 440, 220, 218 });
	sun->idleSunAnim.PushBack({ 436, 440, 220, 218 });
	sun->idleSunAnim.PushBack({ 218, 440, 220, 218 });
	sun->idleSunAnim.PushBack({ 0, 440, 220, 218 });
	sun->idleSunAnim.PushBack({ 436, 220, 220, 218 });
	sun->idleSunAnim.PushBack({ 218, 220, 220, 218 });
	sun->idleSunAnim.PushBack({ 0, 220, 220, 218 });
	sun->idleSunAnim.PushBack({ 436, 0, 220, 218 });
	sun->idleSunAnim.PushBack({ 218, 0, 220, 218 });
	sun->idleSunAnim.loop = true;
	sun->idleSunAnim.mustFlip = false;
	sun->idleSunAnim.speed = 0.12f;
	//Collision sun animations
	sun->collisionSunAnim.PushBack({ 0, 660, 220, 218 });
	sun->collisionSunAnim.PushBack({ 218, 660, 220, 218 });
	sun->collisionSunAnim.PushBack({ 436, 660, 220, 218 });
	sun->collisionSunAnim.PushBack({ 0, 880, 220, 218 });
	sun->collisionSunAnim.PushBack({ 218, 880, 220, 218 });
	sun->collisionSunAnim.PushBack({ 436, 880, 220, 218 });
	sun->collisionSunAnim.PushBack({ 0, 1100, 220, 218 });
	sun->collisionSunAnim.PushBack({ 218, 1100, 220, 218 });
	sun->collisionSunAnim.PushBack({ 436, 1100, 220, 218 });
	sun->collisionSunAnim.PushBack({ 218, 1100, 220, 218 });
	sun->collisionSunAnim.PushBack({ 0, 1100, 220, 218 });
	sun->collisionSunAnim.PushBack({ 436, 880, 220, 218 });
	sun->collisionSunAnim.PushBack({ 218, 880, 220, 218 });
	sun->collisionSunAnim.PushBack({ 0, 880, 220, 218 });
	sun->collisionSunAnim.PushBack({ 436, 660, 220, 218 });
	sun->collisionSunAnim.PushBack({ 218, 660, 220, 218 });
	sun->collisionSunAnim.PushBack({ 0, 660, 220, 218 });
	sun->collisionSunAnim.loop = false;
	sun->collisionSunAnim.mustFlip = false;
	sun->collisionSunAnim.speed = 0.28f;

	//Create PACHINKO1 obstacle
	PachinkoCircle* pachinko1 = new PachinkoCircle;
	pachinko1->round = App->physics->CreateCircle(180, 682, 15, b2_staticBody);
	//Pachinko idle animation
	pachinko1->idlePachinkoAnim.PushBack({ 0, 0, 29, 30 });
	pachinko1->idlePachinkoAnim.loop = false;
	pachinko1->idlePachinkoAnim.mustFlip = false;
	pachinko1->idlePachinkoAnim.speed = 0.01;
	//Pachinko random animation
	pachinko1->randomPachinkoAnim.PushBack({ 0, 0, 30, 30 });
	pachinko1->randomPachinkoAnim.PushBack({ 30, 0, 30, 30 });
	pachinko1->randomPachinkoAnim.PushBack({ 60, 0, 30, 30 });
	pachinko1->randomPachinkoAnim.PushBack({ 90, 0, 30, 30 });
	pachinko1->randomPachinkoAnim.PushBack({ 120, 0, 30, 30 });
	pachinko1->randomPachinkoAnim.PushBack({ 150, 0, 30, 30 });
	pachinko1->randomPachinkoAnim.PushBack({ 180, 0, 30, 30 });
	pachinko1->randomPachinkoAnim.PushBack({ 210, 0, 30, 30 });
	pachinko1->randomPachinkoAnim.PushBack({ 240, 0, 30, 30 });
	pachinko1->randomPachinkoAnim.PushBack({ 270, 0, 30, 30 });
	pachinko1->randomPachinkoAnim.PushBack({ 300, 0, 30, 30 });
	pachinko1->randomPachinkoAnim.PushBack({ 330, 0, 30, 30 });
	pachinko1->randomPachinkoAnim.PushBack({ 360, 0, 30, 30 });
	pachinko1->randomPachinkoAnim.PushBack({ 390, 0, 30, 30 });
	pachinko1->randomPachinkoAnim.PushBack({ 420, 0, 30, 30 });
	pachinko1->randomPachinkoAnim.PushBack({ 450, 0, 30, 30 });
	pachinko1->randomPachinkoAnim.PushBack({ 480, 0, 30, 30 });
	pachinko1->randomPachinkoAnim.PushBack({ 510, 0, 30, 30 });
	pachinko1->randomPachinkoAnim.PushBack({ 540, 0, 30, 30 });
	pachinko1->randomPachinkoAnim.loop = false;
	pachinko1->randomPachinkoAnim.mustFlip = false;
	pachinko1->randomPachinkoAnim.speed = 0.4;
	//Pachinko collision animation
	pachinko1->collisionPachinkoAnim.PushBack({ 0, 0, 30, 30 });
	pachinko1->collisionPachinkoAnim.PushBack({ 30, 0, 30, 30 });
	pachinko1->collisionPachinkoAnim.PushBack({ 60, 0, 30, 30 });
	pachinko1->collisionPachinkoAnim.PushBack({ 90, 0, 30, 30 });
	pachinko1->collisionPachinkoAnim.PushBack({ 120, 0, 30, 30 });
	pachinko1->collisionPachinkoAnim.PushBack({ 150, 0, 30, 30 });
	pachinko1->collisionPachinkoAnim.PushBack({ 180, 0, 30, 30 });
	pachinko1->collisionPachinkoAnim.PushBack({ 210, 0, 30, 30 });
	pachinko1->collisionPachinkoAnim.PushBack({ 240, 0, 30, 30 });
	pachinko1->collisionPachinkoAnim.PushBack({ 270, 0, 30, 30 });
	pachinko1->collisionPachinkoAnim.PushBack({ 300, 0, 30, 30 });
	pachinko1->collisionPachinkoAnim.PushBack({ 330, 0, 30, 30 });
	pachinko1->collisionPachinkoAnim.PushBack({ 360, 0, 30, 30 });
	pachinko1->collisionPachinkoAnim.PushBack({ 390, 0, 30, 30 });
	pachinko1->collisionPachinkoAnim.PushBack({ 420, 0, 30, 30 });
	pachinko1->collisionPachinkoAnim.PushBack({ 450, 0, 30, 30 });
	pachinko1->collisionPachinkoAnim.PushBack({ 480, 0, 30, 30 });
	pachinko1->collisionPachinkoAnim.PushBack({ 510, 0, 30, 30 });
	pachinko1->collisionPachinkoAnim.PushBack({ 540, 0, 30, 30 });
	pachinko1->collisionPachinkoAnim.loop = false;
	pachinko1->collisionPachinkoAnim.pingpong = true;
	pachinko1->collisionPachinkoAnim.mustFlip = false;
	pachinko1->collisionPachinkoAnim.speed = 0.4;
	//Save pachinkos to linked list
	pachinkos.add(pachinko1);

	//Create PACHINKO2 obstacle
	PachinkoCircle* pachinko2 = new PachinkoCircle;
	pachinko2->round = App->physics->CreateCircle(180, 774, 15, b2_staticBody);
	//Pachinko idle animation
	pachinko2->idlePachinkoAnim.PushBack({ 0, 0, 29, 30 });
	pachinko2->idlePachinkoAnim.loop = false;
	pachinko2->idlePachinkoAnim.mustFlip = false;
	pachinko2->idlePachinkoAnim.speed = 0.01;
	//Pachinko random animation
	pachinko2->randomPachinkoAnim.PushBack({ 0, 0, 30, 30 });
	pachinko2->randomPachinkoAnim.PushBack({ 30, 0, 30, 30 });
	pachinko2->randomPachinkoAnim.PushBack({ 60, 0, 30, 30 });
	pachinko2->randomPachinkoAnim.PushBack({ 90, 0, 30, 30 });
	pachinko2->randomPachinkoAnim.PushBack({ 120, 0, 30, 30 });
	pachinko2->randomPachinkoAnim.PushBack({ 150, 0, 30, 30 });
	pachinko2->randomPachinkoAnim.PushBack({ 180, 0, 30, 30 });
	pachinko2->randomPachinkoAnim.PushBack({ 210, 0, 30, 30 });
	pachinko2->randomPachinkoAnim.PushBack({ 240, 0, 30, 30 });
	pachinko2->randomPachinkoAnim.PushBack({ 270, 0, 30, 30 });
	pachinko2->randomPachinkoAnim.PushBack({ 300, 0, 30, 30 });
	pachinko2->randomPachinkoAnim.PushBack({ 330, 0, 30, 30 });
	pachinko2->randomPachinkoAnim.PushBack({ 360, 0, 30, 30 });
	pachinko2->randomPachinkoAnim.PushBack({ 390, 0, 30, 30 });
	pachinko2->randomPachinkoAnim.PushBack({ 420, 0, 30, 30 });
	pachinko2->randomPachinkoAnim.PushBack({ 450, 0, 30, 30 });
	pachinko2->randomPachinkoAnim.PushBack({ 480, 0, 30, 30 });
	pachinko2->randomPachinkoAnim.PushBack({ 510, 0, 30, 30 });
	pachinko2->randomPachinkoAnim.PushBack({ 540, 0, 30, 30 });
	pachinko2->randomPachinkoAnim.loop = false;
	pachinko2->randomPachinkoAnim.mustFlip = false;
	pachinko2->randomPachinkoAnim.speed = 0.4;
	//Pachinko collision animation
	pachinko2->collisionPachinkoAnim.PushBack({ 0, 0, 30, 30 });
	pachinko2->collisionPachinkoAnim.PushBack({ 30, 0, 30, 30 });
	pachinko2->collisionPachinkoAnim.PushBack({ 60, 0, 30, 30 });
	pachinko2->collisionPachinkoAnim.PushBack({ 90, 0, 30, 30 });
	pachinko2->collisionPachinkoAnim.PushBack({ 120, 0, 30, 30 });
	pachinko2->collisionPachinkoAnim.PushBack({ 150, 0, 30, 30 });
	pachinko2->collisionPachinkoAnim.PushBack({ 180, 0, 30, 30 });
	pachinko2->collisionPachinkoAnim.PushBack({ 210, 0, 30, 30 });
	pachinko2->collisionPachinkoAnim.PushBack({ 240, 0, 30, 30 });
	pachinko2->collisionPachinkoAnim.PushBack({ 270, 0, 30, 30 });
	pachinko2->collisionPachinkoAnim.PushBack({ 300, 0, 30, 30 });
	pachinko2->collisionPachinkoAnim.PushBack({ 330, 0, 30, 30 });
	pachinko2->collisionPachinkoAnim.PushBack({ 360, 0, 30, 30 });
	pachinko2->collisionPachinkoAnim.PushBack({ 390, 0, 30, 30 });
	pachinko2->collisionPachinkoAnim.PushBack({ 420, 0, 30, 30 });
	pachinko2->collisionPachinkoAnim.PushBack({ 450, 0, 30, 30 });
	pachinko2->collisionPachinkoAnim.PushBack({ 480, 0, 30, 30 });
	pachinko2->collisionPachinkoAnim.PushBack({ 510, 0, 30, 30 });
	pachinko2->collisionPachinkoAnim.PushBack({ 540, 0, 30, 30 });
	pachinko2->collisionPachinkoAnim.loop = false;
	pachinko2->collisionPachinkoAnim.pingpong = true;
	pachinko2->collisionPachinkoAnim.mustFlip = false;
	pachinko2->collisionPachinkoAnim.speed = 0.4;
	//Save pachinkos to linked list
	pachinkos.add(pachinko2);

	//Create PACHINKO3 obstacle
	PachinkoCircle* pachinko3 = new PachinkoCircle;
	pachinko3->round = App->physics->CreateCircle(180, 866, 15, b2_staticBody);
	//Pachinko idle animation
	pachinko3->idlePachinkoAnim.PushBack({ 0, 0, 29, 30 });
	pachinko3->idlePachinkoAnim.loop = false;
	pachinko3->idlePachinkoAnim.mustFlip = false;
	pachinko3->idlePachinkoAnim.speed = 0.01;
	//Pachinko random animation
	pachinko3->randomPachinkoAnim.PushBack({ 0, 0, 30, 30 });
	pachinko3->randomPachinkoAnim.PushBack({ 30, 0, 30, 30 });
	pachinko3->randomPachinkoAnim.PushBack({ 60, 0, 30, 30 });
	pachinko3->randomPachinkoAnim.PushBack({ 90, 0, 30, 30 });
	pachinko3->randomPachinkoAnim.PushBack({ 120, 0, 30, 30 });
	pachinko3->randomPachinkoAnim.PushBack({ 150, 0, 30, 30 });
	pachinko3->randomPachinkoAnim.PushBack({ 180, 0, 30, 30 });
	pachinko3->randomPachinkoAnim.PushBack({ 210, 0, 30, 30 });
	pachinko3->randomPachinkoAnim.PushBack({ 240, 0, 30, 30 });
	pachinko3->randomPachinkoAnim.PushBack({ 270, 0, 30, 30 });
	pachinko3->randomPachinkoAnim.PushBack({ 300, 0, 30, 30 });
	pachinko3->randomPachinkoAnim.PushBack({ 330, 0, 30, 30 });
	pachinko3->randomPachinkoAnim.PushBack({ 360, 0, 30, 30 });
	pachinko3->randomPachinkoAnim.PushBack({ 390, 0, 30, 30 });
	pachinko3->randomPachinkoAnim.PushBack({ 420, 0, 30, 30 });
	pachinko3->randomPachinkoAnim.PushBack({ 450, 0, 30, 30 });
	pachinko3->randomPachinkoAnim.PushBack({ 480, 0, 30, 30 });
	pachinko3->randomPachinkoAnim.PushBack({ 510, 0, 30, 30 });
	pachinko3->randomPachinkoAnim.PushBack({ 540, 0, 30, 30 });
	pachinko3->randomPachinkoAnim.loop = false;
	pachinko3->randomPachinkoAnim.mustFlip = false;
	pachinko3->randomPachinkoAnim.speed = 0.4;
	//Pachinko collision animation
	pachinko3->collisionPachinkoAnim.PushBack({ 0, 0, 30, 30 });
	pachinko3->collisionPachinkoAnim.PushBack({ 30, 0, 30, 30 });
	pachinko3->collisionPachinkoAnim.PushBack({ 60, 0, 30, 30 });
	pachinko3->collisionPachinkoAnim.PushBack({ 90, 0, 30, 30 });
	pachinko3->collisionPachinkoAnim.PushBack({ 120, 0, 30, 30 });
	pachinko3->collisionPachinkoAnim.PushBack({ 150, 0, 30, 30 });
	pachinko3->collisionPachinkoAnim.PushBack({ 180, 0, 30, 30 });
	pachinko3->collisionPachinkoAnim.PushBack({ 210, 0, 30, 30 });
	pachinko3->collisionPachinkoAnim.PushBack({ 240, 0, 30, 30 });
	pachinko3->collisionPachinkoAnim.PushBack({ 270, 0, 30, 30 });
	pachinko3->collisionPachinkoAnim.PushBack({ 300, 0, 30, 30 });
	pachinko3->collisionPachinkoAnim.PushBack({ 330, 0, 30, 30 });
	pachinko3->collisionPachinkoAnim.PushBack({ 360, 0, 30, 30 });
	pachinko3->collisionPachinkoAnim.PushBack({ 390, 0, 30, 30 });
	pachinko3->collisionPachinkoAnim.PushBack({ 420, 0, 30, 30 });
	pachinko3->collisionPachinkoAnim.PushBack({ 450, 0, 30, 30 });
	pachinko3->collisionPachinkoAnim.PushBack({ 480, 0, 30, 30 });
	pachinko3->collisionPachinkoAnim.PushBack({ 510, 0, 30, 30 });
	pachinko3->collisionPachinkoAnim.PushBack({ 540, 0, 30, 30 });
	pachinko3->collisionPachinkoAnim.loop = false;
	pachinko3->collisionPachinkoAnim.pingpong = true;
	pachinko3->collisionPachinkoAnim.mustFlip = false;
	pachinko3->collisionPachinkoAnim.speed = 0.4;
	//Save pachinkos to linked list
	pachinkos.add(pachinko3);

	//Create PACHINKO4 obstacle
	PachinkoCircle* pachinko4 = new PachinkoCircle;
	pachinko4->round = App->physics->CreateCircle(226, 728, 15, b2_staticBody);
	//Pachinko idle animation
	pachinko4->idlePachinkoAnim.PushBack({ 0, 0, 29, 30 });
	pachinko4->idlePachinkoAnim.loop = false;
	pachinko4->idlePachinkoAnim.mustFlip = false;
	pachinko4->idlePachinkoAnim.speed = 0.01;
	//Pachinko random animation
	pachinko4->randomPachinkoAnim.PushBack({ 0, 0, 30, 30 });
	pachinko4->randomPachinkoAnim.PushBack({ 30, 0, 30, 30 });
	pachinko4->randomPachinkoAnim.PushBack({ 60, 0, 30, 30 });
	pachinko4->randomPachinkoAnim.PushBack({ 90, 0, 30, 30 });
	pachinko4->randomPachinkoAnim.PushBack({ 120, 0, 30, 30 });
	pachinko4->randomPachinkoAnim.PushBack({ 150, 0, 30, 30 });
	pachinko4->randomPachinkoAnim.PushBack({ 180, 0, 30, 30 });
	pachinko4->randomPachinkoAnim.PushBack({ 210, 0, 30, 30 });
	pachinko4->randomPachinkoAnim.PushBack({ 240, 0, 30, 30 });
	pachinko4->randomPachinkoAnim.PushBack({ 270, 0, 30, 30 });
	pachinko4->randomPachinkoAnim.PushBack({ 300, 0, 30, 30 });
	pachinko4->randomPachinkoAnim.PushBack({ 330, 0, 30, 30 });
	pachinko4->randomPachinkoAnim.PushBack({ 360, 0, 30, 30 });
	pachinko4->randomPachinkoAnim.PushBack({ 390, 0, 30, 30 });
	pachinko4->randomPachinkoAnim.PushBack({ 420, 0, 30, 30 });
	pachinko4->randomPachinkoAnim.PushBack({ 450, 0, 30, 30 });
	pachinko4->randomPachinkoAnim.PushBack({ 480, 0, 30, 30 });
	pachinko4->randomPachinkoAnim.PushBack({ 510, 0, 30, 30 });
	pachinko4->randomPachinkoAnim.PushBack({ 540, 0, 30, 30 });
	pachinko4->randomPachinkoAnim.loop = false;
	pachinko4->randomPachinkoAnim.mustFlip = false;
	pachinko4->randomPachinkoAnim.speed = 0.4;
	//Pachinko collision animation
	pachinko4->collisionPachinkoAnim.PushBack({ 0, 0, 30, 30 });
	pachinko4->collisionPachinkoAnim.PushBack({ 30, 0, 30, 30 });
	pachinko4->collisionPachinkoAnim.PushBack({ 60, 0, 30, 30 });
	pachinko4->collisionPachinkoAnim.PushBack({ 90, 0, 30, 30 });
	pachinko4->collisionPachinkoAnim.PushBack({ 120, 0, 30, 30 });
	pachinko4->collisionPachinkoAnim.PushBack({ 150, 0, 30, 30 });
	pachinko4->collisionPachinkoAnim.PushBack({ 180, 0, 30, 30 });
	pachinko4->collisionPachinkoAnim.PushBack({ 210, 0, 30, 30 });
	pachinko4->collisionPachinkoAnim.PushBack({ 240, 0, 30, 30 });
	pachinko4->collisionPachinkoAnim.PushBack({ 270, 0, 30, 30 });
	pachinko4->collisionPachinkoAnim.PushBack({ 300, 0, 30, 30 });
	pachinko4->collisionPachinkoAnim.PushBack({ 330, 0, 30, 30 });
	pachinko4->collisionPachinkoAnim.PushBack({ 360, 0, 30, 30 });
	pachinko4->collisionPachinkoAnim.PushBack({ 390, 0, 30, 30 });
	pachinko4->collisionPachinkoAnim.PushBack({ 420, 0, 30, 30 });
	pachinko4->collisionPachinkoAnim.PushBack({ 450, 0, 30, 30 });
	pachinko4->collisionPachinkoAnim.PushBack({ 480, 0, 30, 30 });
	pachinko4->collisionPachinkoAnim.PushBack({ 510, 0, 30, 30 });
	pachinko4->collisionPachinkoAnim.PushBack({ 540, 0, 30, 30 });
	pachinko4->collisionPachinkoAnim.loop = false;
	pachinko4->collisionPachinkoAnim.pingpong = true;
	pachinko4->collisionPachinkoAnim.mustFlip = false;
	pachinko4->collisionPachinkoAnim.speed = 0.4;
	//Save pachinkos to linked list
	pachinkos.add(pachinko4);

	//Create PACHINKO5 obstacle
	PachinkoCircle* pachinko5 = new PachinkoCircle;
	pachinko5->round = App->physics->CreateCircle(134, 728, 15, b2_staticBody);
	//Pachinko idle animation
	pachinko5->idlePachinkoAnim.PushBack({ 0, 0, 29, 30 });
	pachinko5->idlePachinkoAnim.loop = false;
	pachinko5->idlePachinkoAnim.mustFlip = false;
	pachinko5->idlePachinkoAnim.speed = 0.01;
	//Pachinko random animation
	pachinko5->randomPachinkoAnim.PushBack({ 0, 0, 30, 30 });
	pachinko5->randomPachinkoAnim.PushBack({ 30, 0, 30, 30 });
	pachinko5->randomPachinkoAnim.PushBack({ 60, 0, 30, 30 });
	pachinko5->randomPachinkoAnim.PushBack({ 90, 0, 30, 30 });
	pachinko5->randomPachinkoAnim.PushBack({ 120, 0, 30, 30 });
	pachinko5->randomPachinkoAnim.PushBack({ 150, 0, 30, 30 });
	pachinko5->randomPachinkoAnim.PushBack({ 180, 0, 30, 30 });
	pachinko5->randomPachinkoAnim.PushBack({ 210, 0, 30, 30 });
	pachinko5->randomPachinkoAnim.PushBack({ 240, 0, 30, 30 });
	pachinko5->randomPachinkoAnim.PushBack({ 270, 0, 30, 30 });
	pachinko5->randomPachinkoAnim.PushBack({ 300, 0, 30, 30 });
	pachinko5->randomPachinkoAnim.PushBack({ 330, 0, 30, 30 });
	pachinko5->randomPachinkoAnim.PushBack({ 360, 0, 30, 30 });
	pachinko5->randomPachinkoAnim.PushBack({ 390, 0, 30, 30 });
	pachinko5->randomPachinkoAnim.PushBack({ 420, 0, 30, 30 });
	pachinko5->randomPachinkoAnim.PushBack({ 450, 0, 30, 30 });
	pachinko5->randomPachinkoAnim.PushBack({ 480, 0, 30, 30 });
	pachinko5->randomPachinkoAnim.PushBack({ 510, 0, 30, 30 });
	pachinko5->randomPachinkoAnim.PushBack({ 540, 0, 30, 30 });
	pachinko5->randomPachinkoAnim.loop = false;
	pachinko5->randomPachinkoAnim.mustFlip = false;
	pachinko5->randomPachinkoAnim.speed = 0.4;
	//Pachinko collision animation
	pachinko5->collisionPachinkoAnim.PushBack({ 0, 0, 30, 30 });
	pachinko5->collisionPachinkoAnim.PushBack({ 30, 0, 30, 30 });
	pachinko5->collisionPachinkoAnim.PushBack({ 60, 0, 30, 30 });
	pachinko5->collisionPachinkoAnim.PushBack({ 90, 0, 30, 30 });
	pachinko5->collisionPachinkoAnim.PushBack({ 120, 0, 30, 30 });
	pachinko5->collisionPachinkoAnim.PushBack({ 150, 0, 30, 30 });
	pachinko5->collisionPachinkoAnim.PushBack({ 180, 0, 30, 30 });
	pachinko5->collisionPachinkoAnim.PushBack({ 210, 0, 30, 30 });
	pachinko5->collisionPachinkoAnim.PushBack({ 240, 0, 30, 30 });
	pachinko5->collisionPachinkoAnim.PushBack({ 270, 0, 30, 30 });
	pachinko5->collisionPachinkoAnim.PushBack({ 300, 0, 30, 30 });
	pachinko5->collisionPachinkoAnim.PushBack({ 330, 0, 30, 30 });
	pachinko5->collisionPachinkoAnim.PushBack({ 360, 0, 30, 30 });
	pachinko5->collisionPachinkoAnim.PushBack({ 390, 0, 30, 30 });
	pachinko5->collisionPachinkoAnim.PushBack({ 420, 0, 30, 30 });
	pachinko5->collisionPachinkoAnim.PushBack({ 450, 0, 30, 30 });
	pachinko5->collisionPachinkoAnim.PushBack({ 480, 0, 30, 30 });
	pachinko5->collisionPachinkoAnim.PushBack({ 510, 0, 30, 30 });
	pachinko5->collisionPachinkoAnim.PushBack({ 540, 0, 30, 30 });
	pachinko5->collisionPachinkoAnim.loop = false;
	pachinko5->collisionPachinkoAnim.pingpong = true;
	pachinko5->collisionPachinkoAnim.mustFlip = false;
	pachinko5->collisionPachinkoAnim.speed = 0.4;
	//Save pachinkos to linked list
	pachinkos.add(pachinko5);

	//Create PACHINKO6 obstacle
	PachinkoCircle* pachinko6 = new PachinkoCircle;
	pachinko6->round = App->physics->CreateCircle(134, 820, 15, b2_staticBody);
	//Pachinko idle animation
	pachinko6->idlePachinkoAnim.PushBack({ 0, 0, 29, 30 });
	pachinko6->idlePachinkoAnim.loop = false;
	pachinko6->idlePachinkoAnim.mustFlip = false;
	pachinko6->idlePachinkoAnim.speed = 0.01;
	//Pachinko random animation
	pachinko6->randomPachinkoAnim.PushBack({ 0, 0, 30, 30 });
	pachinko6->randomPachinkoAnim.PushBack({ 30, 0, 30, 30 });
	pachinko6->randomPachinkoAnim.PushBack({ 60, 0, 30, 30 });
	pachinko6->randomPachinkoAnim.PushBack({ 90, 0, 30, 30 });
	pachinko6->randomPachinkoAnim.PushBack({ 120, 0, 30, 30 });
	pachinko6->randomPachinkoAnim.PushBack({ 150, 0, 30, 30 });
	pachinko6->randomPachinkoAnim.PushBack({ 180, 0, 30, 30 });
	pachinko6->randomPachinkoAnim.PushBack({ 210, 0, 30, 30 });
	pachinko6->randomPachinkoAnim.PushBack({ 240, 0, 30, 30 });
	pachinko6->randomPachinkoAnim.PushBack({ 270, 0, 30, 30 });
	pachinko6->randomPachinkoAnim.PushBack({ 300, 0, 30, 30 });
	pachinko6->randomPachinkoAnim.PushBack({ 330, 0, 30, 30 });
	pachinko6->randomPachinkoAnim.PushBack({ 360, 0, 30, 30 });
	pachinko6->randomPachinkoAnim.PushBack({ 390, 0, 30, 30 });
	pachinko6->randomPachinkoAnim.PushBack({ 420, 0, 30, 30 });
	pachinko6->randomPachinkoAnim.PushBack({ 450, 0, 30, 30 });
	pachinko6->randomPachinkoAnim.PushBack({ 480, 0, 30, 30 });
	pachinko6->randomPachinkoAnim.PushBack({ 510, 0, 30, 30 });
	pachinko6->randomPachinkoAnim.PushBack({ 540, 0, 30, 30 });
	pachinko6->randomPachinkoAnim.loop = false;
	pachinko6->randomPachinkoAnim.mustFlip = false;
	pachinko6->randomPachinkoAnim.speed = 0.4;
	//Pachinko collision animation
	pachinko6->collisionPachinkoAnim.PushBack({ 0, 0, 30, 30 });
	pachinko6->collisionPachinkoAnim.PushBack({ 30, 0, 30, 30 });
	pachinko6->collisionPachinkoAnim.PushBack({ 60, 0, 30, 30 });
	pachinko6->collisionPachinkoAnim.PushBack({ 90, 0, 30, 30 });
	pachinko6->collisionPachinkoAnim.PushBack({ 120, 0, 30, 30 });
	pachinko6->collisionPachinkoAnim.PushBack({ 150, 0, 30, 30 });
	pachinko6->collisionPachinkoAnim.PushBack({ 180, 0, 30, 30 });
	pachinko6->collisionPachinkoAnim.PushBack({ 210, 0, 30, 30 });
	pachinko6->collisionPachinkoAnim.PushBack({ 240, 0, 30, 30 });
	pachinko6->collisionPachinkoAnim.PushBack({ 270, 0, 30, 30 });
	pachinko6->collisionPachinkoAnim.PushBack({ 300, 0, 30, 30 });
	pachinko6->collisionPachinkoAnim.PushBack({ 330, 0, 30, 30 });
	pachinko6->collisionPachinkoAnim.PushBack({ 360, 0, 30, 30 });
	pachinko6->collisionPachinkoAnim.PushBack({ 390, 0, 30, 30 });
	pachinko6->collisionPachinkoAnim.PushBack({ 420, 0, 30, 30 });
	pachinko6->collisionPachinkoAnim.PushBack({ 450, 0, 30, 30 });
	pachinko6->collisionPachinkoAnim.PushBack({ 480, 0, 30, 30 });
	pachinko6->collisionPachinkoAnim.PushBack({ 510, 0, 30, 30 });
	pachinko6->collisionPachinkoAnim.PushBack({ 540, 0, 30, 30 });
	pachinko6->collisionPachinkoAnim.loop = false;
	pachinko6->collisionPachinkoAnim.pingpong = true;
	pachinko6->collisionPachinkoAnim.mustFlip = false;
	pachinko6->collisionPachinkoAnim.speed = 0.4;
	//Save pachinkos to linked list
	pachinkos.add(pachinko6);

	//Create PACHINKO5 obstacle
	PachinkoCircle* pachinko7 = new PachinkoCircle;
	pachinko7->round = App->physics->CreateCircle(226, 820, 15, b2_staticBody);
	//Pachinko idle animation
	pachinko7->idlePachinkoAnim.PushBack({ 0, 0, 29, 30 });
	pachinko7->idlePachinkoAnim.loop = false;
	pachinko7->idlePachinkoAnim.mustFlip = false;
	pachinko7->idlePachinkoAnim.speed = 0.01;
	//Pachinko random animation
	pachinko7->randomPachinkoAnim.PushBack({ 0, 0, 30, 30 });
	pachinko7->randomPachinkoAnim.PushBack({ 30, 0, 30, 30 });
	pachinko7->randomPachinkoAnim.PushBack({ 60, 0, 30, 30 });
	pachinko7->randomPachinkoAnim.PushBack({ 90, 0, 30, 30 });
	pachinko7->randomPachinkoAnim.PushBack({ 120, 0, 30, 30 });
	pachinko7->randomPachinkoAnim.PushBack({ 150, 0, 30, 30 });
	pachinko7->randomPachinkoAnim.PushBack({ 180, 0, 30, 30 });
	pachinko7->randomPachinkoAnim.PushBack({ 210, 0, 30, 30 });
	pachinko7->randomPachinkoAnim.PushBack({ 240, 0, 30, 30 });
	pachinko7->randomPachinkoAnim.PushBack({ 270, 0, 30, 30 });
	pachinko7->randomPachinkoAnim.PushBack({ 300, 0, 30, 30 });
	pachinko7->randomPachinkoAnim.PushBack({ 330, 0, 30, 30 });
	pachinko7->randomPachinkoAnim.PushBack({ 360, 0, 30, 30 });
	pachinko7->randomPachinkoAnim.PushBack({ 390, 0, 30, 30 });
	pachinko7->randomPachinkoAnim.PushBack({ 420, 0, 30, 30 });
	pachinko7->randomPachinkoAnim.PushBack({ 450, 0, 30, 30 });
	pachinko7->randomPachinkoAnim.PushBack({ 480, 0, 30, 30 });
	pachinko7->randomPachinkoAnim.PushBack({ 510, 0, 30, 30 });
	pachinko7->randomPachinkoAnim.PushBack({ 540, 0, 30, 30 });
	pachinko7->randomPachinkoAnim.loop = false;
	pachinko7->randomPachinkoAnim.mustFlip = false;
	pachinko7->randomPachinkoAnim.speed = 0.4;
	//Pachinko collision animation
	pachinko7->collisionPachinkoAnim.PushBack({ 0, 0, 30, 30 });
	pachinko7->collisionPachinkoAnim.PushBack({ 30, 0, 30, 30 });
	pachinko7->collisionPachinkoAnim.PushBack({ 60, 0, 30, 30 });
	pachinko7->collisionPachinkoAnim.PushBack({ 90, 0, 30, 30 });
	pachinko7->collisionPachinkoAnim.PushBack({ 120, 0, 30, 30 });
	pachinko7->collisionPachinkoAnim.PushBack({ 150, 0, 30, 30 });
	pachinko7->collisionPachinkoAnim.PushBack({ 180, 0, 30, 30 });
	pachinko7->collisionPachinkoAnim.PushBack({ 210, 0, 30, 30 });
	pachinko7->collisionPachinkoAnim.PushBack({ 240, 0, 30, 30 });
	pachinko7->collisionPachinkoAnim.PushBack({ 270, 0, 30, 30 });
	pachinko7->collisionPachinkoAnim.PushBack({ 300, 0, 30, 30 });
	pachinko7->collisionPachinkoAnim.PushBack({ 330, 0, 30, 30 });
	pachinko7->collisionPachinkoAnim.PushBack({ 360, 0, 30, 30 });
	pachinko7->collisionPachinkoAnim.PushBack({ 390, 0, 30, 30 });
	pachinko7->collisionPachinkoAnim.PushBack({ 420, 0, 30, 30 });
	pachinko7->collisionPachinkoAnim.PushBack({ 450, 0, 30, 30 });
	pachinko7->collisionPachinkoAnim.PushBack({ 480, 0, 30, 30 });
	pachinko7->collisionPachinkoAnim.PushBack({ 510, 0, 30, 30 });
	pachinko7->collisionPachinkoAnim.PushBack({ 540, 0, 30, 30 });
	pachinko7->collisionPachinkoAnim.loop = false;
	pachinko7->collisionPachinkoAnim.pingpong = true;
	pachinko7->collisionPachinkoAnim.mustFlip = false;
	pachinko7->collisionPachinkoAnim.speed = 0.4;
	//Save pachinkos to linked list
	pachinkos.add(pachinko7);

	bool ret = true;
	App->renderer->camera.x = App->renderer->camera.y = 0;

	background = App->textures->Load("pinball/background.png");
	ballTexture = App->textures->Load("pinball/ball.png");
	sunTexture = App->textures->Load("pinball/sun.png");
	pachinkoTexture = App->textures->Load("pinball/pachinko.png");
	box = App->textures->Load("pinball/crate.png");
	intro = App->audio->LoadFx("pinball/intro.wav");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	StartScreen = App->textures->Load("pinball/backgroundStart.png");

	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);

	App->audio->PlayFx(intro, 1);

	return ret;
}

// Load assets
bool ModuleSceneGame::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneGame::Update()
{

	switch (GameState)
	{
	case gameState::START:
		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) { GameState = gameState::GAME; }
		App->renderer->Blit(StartScreen, 0, 0, NULL, 0.0f, 0);

		break;
	case gameState::GAME:

		//ray debug
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			ray_on = !ray_on;
			ray.x = App->input->GetMouseX();
			ray.y = App->input->GetMouseY();
		}

		//kicker input
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN) App->physics->kickerJoint->SetMotorSpeed(100.0f);
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_UP) App->physics->kickerJoint->SetMotorSpeed(-100.0f);

		//flippers' input
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			p2List_item<Flipper*>* f = flippers.getFirst();
			while (f != NULL)
			{
				if (f->data->rightSide == false)
				{
					f->data->Rect->body->ApplyForce({ -5,0 }, { 0,0 }, true);
				}
				f = f->next;
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			p2List_item<Flipper*>* f = flippers.getFirst();
			while (f != NULL)
			{
				if (f->data->rightSide == true)
				{
					f->data->Rect->body->ApplyForce({ 5,0 }, { 0,0 }, true);
				}
				f = f->next;
			}
		}

		// Prepare for raycast ------------------------------------------------------
		iPoint mouse;
		mouse.x = App->input->GetMouseX();
		mouse.y = App->input->GetMouseY();
		int ray_hit = ray.DistanceTo(mouse);

		fVector normal(0.0f, 0.0f);

		// All draw functions ------------------------------------------------------
		App->renderer->Blit(background, 0, 0, NULL, 0.0f, 0);

		//BALL
		if (ballState != BALL_DEATH)
		{
			ballState = BALL_SPAWN;
			if (ball->spawnBallAnim.HasFinished())
			{
				ballState = BALL_IDLE;
			}
		}

		if (ballState != BALL_DEATH)
		{
			ballState = BALL_SPAWN;
			if (ball->spawnBallAnim.HasFinished())
			{
				ballState = BALL_IDLE;
			}
		}

		switch (ballState)
		{
		case BALL_IDLE:
			currentAnim = &ball->idleBallAnim;
			break;

		case BALL_DEATH:
			currentAnim = &ball->deathBallAnim;
			break;

		case BALL_SPAWN:
			currentAnim = &ball->spawnBallAnim;
			break;
		}

		int x, y;
		ball->round->GetPosition(x, y);
		App->renderer->Blit(ballTexture, x - 7, y - 7, &(currentAnim->GetCurrentFrame()), 1.0f, true);
		ball->idleBallAnim.Update();
		ball->deathBallAnim.Update();
		ball->spawnBallAnim.Update();


		//SUN
		switch (sunState)
		{
		case SUN_IDLE:
			currentAnim = &sun->idleSunAnim;
			break;
		case SUN_COLLISION:
			currentAnim = &sun->collisionSunAnim;
			break;
		}

		if ((sunState == SUN_COLLISION) && (sun->collisionSunAnim.HasFinished() == true))
		{
			sun->collisionSunAnim.Reset();
			sun->idleSunAnim.Reset();
			sunState = SUN_IDLE;
		}

		if (currentAnim == &sun->idleSunAnim)
		{
			sun->idleSunAnim.Update();
		}
		if (currentAnim == &sun->collisionSunAnim)
		{
			sun->collisionSunAnim.Update();
		}

		App->renderer->Blit(sunTexture, 226, 230, &(currentAnim->GetCurrentFrame()), 1.0f);

		//PACHINKO
		p2List_item<PachinkoCircle*>* p = pachinkos.getFirst();
		while (p != NULL)
		{
			int x = 166;
			int y = 668;

			int random = 0;
			srand(time(NULL));
			random = rand() % 40;

			if (random >= 35)
			{
				pachinkoState = PACHINKO_RANDOM;
			}

			switch (pachinkoState)
			{
			case PACHINKO_IDLE:
				currentAnim = &p->data->randomPachinkoAnim;
				break;

			case PACHINKO_RANDOM:
				currentAnim = &p->data->randomPachinkoAnim;
				break;

			case PACHINKO_COLLISION:
				currentAnim = &p->data->collisionPachinkoAnim;
				break;
			}

			if (pachinkoState == PACHINKO_IDLE) { p->data->idlePachinkoAnim.Update(); }
			if (pachinkoState == PACHINKO_RANDOM) { p->data->randomPachinkoAnim.Update(); }
			if (pachinkoState == PACHINKO_COLLISION) { p->data->collisionPachinkoAnim.Update(); }

			if (pachinkoState == PACHINKO_IDLE)
			{
				p->data->randomPachinkoAnim.Reset();
				p->data->collisionPachinkoAnim.Reset();
				pachinkoState = PACHINKO_IDLE;
			}
			if (pachinkoState == PACHINKO_RANDOM && p->data->randomPachinkoAnim.HasFinished() == true)
			{
				pachinkoState = PACHINKO_IDLE;
			}

			for (int i = 0; i < 7; i++)
			{
				if (i == 0) { App->renderer->Blit(pachinkoTexture, x, y, &(currentAnim->GetCurrentFrame()), 1.0f); }
				if (i == 1) { App->renderer->Blit(pachinkoTexture, x, y + 92, &(currentAnim->GetCurrentFrame()), 1.0f); }
				if (i == 2) { App->renderer->Blit(pachinkoTexture, x, y + 184, &(currentAnim->GetCurrentFrame()), 1.0f); }
				if (i == 3) { App->renderer->Blit(pachinkoTexture, x + 46, y + 46, &(currentAnim->GetCurrentFrame()), 1.0f); }
				if (i == 4) { App->renderer->Blit(pachinkoTexture, x - 46, y + 46, &(currentAnim->GetCurrentFrame()), 1.0f); }
				if (i == 5) { App->renderer->Blit(pachinkoTexture, x - 46, y + 138, &(currentAnim->GetCurrentFrame()), 1.0f); }
				if (i == 6) { App->renderer->Blit(pachinkoTexture, x + 46, y + 138, &(currentAnim->GetCurrentFrame()), 1.0f); }
			}

			p = p->next;


		}



		// ray ---------------
		if (ray_on == true)
		{
			fVector destination(mouse.x - ray.x, mouse.y - ray.y);
			destination.Normalize();
			destination *= ray_hit;

			App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

			if (normal.x != 0.0f)
				App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
		}
		break;
	}
	


	return UPDATE_CONTINUE;
}

void ModuleSceneGame::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	if (bodyA->body->GetType() == 2 && bodyB->body->GetType() == 0 || bodyA->body->GetType() == 2 && bodyB->body->GetType() == 2)
	{
		App->audio->PlayFx(bonus_fx);
		sunState = SUN_COLLISION;
	}

	/*if (bodyA->body->GetFixtureList()->)
	{
		sunState = SUN_COLLISION;
	}*/

	/*
	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}*/
}