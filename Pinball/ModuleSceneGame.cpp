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
	background = backgroundGame = ballTexture = columnsTexture = uiTexture = pachinkoTexture = sunTexture = box = StartScreen = flipperKicker =NULL;
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
	backgrounds.getFirst()->data->listener = this;
	backgrounds.getFirst()->data->type = PhysBody::Type::wallCollider;

	// Flippers --------------------------------------------------------------
	//Left Flipper
	Flipper* f1 = new Flipper;
	f1->Circle = App->physics->CreateCircle(285, 965, 4, b2_staticBody);
	f1->Rect = App->physics->CreateRectangle(265 + rectSect.w / 2, 945 + rectSect.h / 2, rectSect.w, rectSect.h - 15, b2_dynamicBody);
	f1->rightSide = false;
	App->physics->CreateRevoluteJoint(f1->Rect, veca, f1->Circle, vecb, 20.0f);
	flippers.add(f1);
	f1->Rect->listener = this;
	f1->Rect->type = PhysBody::Type::flipperCollider;

	//Right Flipper
	veca = { 0.80,0 };

	Flipper* f2 = new Flipper;
	f2->Circle = App->physics->CreateCircle(545, 965, 4, b2_staticBody);
	f2->Rect = App->physics->CreateRectangle(525 - rectSect.w / 2, 945 + rectSect.h / 2, rectSect.w, rectSect.h - 15, b2_dynamicBody);
	f2->rightSide = true;
	App->physics->CreateRevoluteJoint(f2->Rect, veca, f2->Circle, vecb, 20.0f);
	flippers.add(f2);
	f2->Rect->listener = this;
	f2->Rect->type = PhysBody::Type::flipperCollider;

	// kicker
	kicker = new Kicker;

	//rottenshit
	veca = { 1.5,0 };

	a = new circlerot;
	a2 = new circlerot;
	a->circlerot = App->physics->CreateCircle(500, 500, 10, b2_kinematicBody);
	a2->circlerot = App->physics->CreateCircle(500, 700, 10, b2_kinematicBody);
	a->centercircle = App->physics->CreateCircle(600, 600, 5, b2_staticBody);
	a2->centercircle = a->centercircle;
	App->physics->CreateRevoluteJoint(a->circlerot, veca,a->centercircle,vecb, 180,true, false);
	App->physics->CreateRevoluteJoint(a2->circlerot, veca, a2->centercircle, vecb, 180, true, false);
	a->circlerot->body->SetGravityScale(0);
	a2->circlerot->body->SetGravityScale(0);
	//a->circlerot->body->SetAngularDamping(0);
	//rottenshit
	a->circlerot->body->SetAngularVelocity(60 * DEGTORAD);
	a2->circlerot->body->SetAngularVelocity(60 * DEGTORAD);
	//a->circlerot->body->

	//a->circlerot->body->IsFixedRotation();
	//a2->circlerot->body->IsFixedRotation();
	//rottenshit

	//Create Kicker
	/*kicker = new Kicker;
	kicker->mobile = App->physics->CreateRectangle(742, 950, 40, 20, b2_kinematicBody);
	kicker->pivot = App->physics->CreateRectangle(742, 970, 20, 20, b2_staticBody);
	kicker->kickerJoint = App->physics->CreatePrismaticJoint(kicker->mobile, { 0, 3 }, kicker->pivot, { 0, 1 }, { 0, 1 }, 20.0f, true, true);*/

	//Defining textures
	background = App->textures->Load("pinball/background.png");
	uiTexture = App->textures->Load("pinball/UI.png");
	ballTexture = App->textures->Load("pinball/ball.png");
	columnsTexture = App->textures->Load("pinball/columns.png");
	sheenTexture = App->textures->Load("pinball/sheen.png");
	sunTexture = App->textures->Load("pinball/sun.png");
	pachinkoTexture = App->textures->Load("pinball/pachinko.png");
	box = App->textures->Load("pinball/crate.png");
	StartScreen = App->textures->Load("pinball/backgroundStart.png");
	backgroundGame = App->textures->Load("pinball/backgroundGame.png");
	flipperKicker = App->textures->Load("pinball/flipperKicker.png");
	
	//Loading music & fx
	intro = App->audio->LoadFx("pinball/intro.wav");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	ball1_fx = App->audio->LoadFx("pinball/ball1.wav");
	ball2_fx = App->audio->LoadFx("pinball/ball2.wav");
	ball3_fx = App->audio->LoadFx("pinball/ball3.wav");
	ballDeath_fx = App->audio->LoadFx("pinball/ballDeath.wav");
	sun_fx = App->audio->LoadFx("pinball/sun.wav");
	sheen1_fx = App->audio->LoadFx("pinball/sheen1.wav");
	sheen2_fx = App->audio->LoadFx("pinball/sheen2.wav");
	flippers_fx = App->audio->LoadFx("pinball/flippers.wav");

	//UI
	//UI Animations
	ui = new UI;
	ui->uiAnim1.PushBack({ 0, 0, 782, 73 });
	ui->uiAnim1.loop = false;
	ui->uiAnim1.mustFlip = false;
	ui->uiAnim1.speed = 0.14;

	//Create BALL
	ball = new DCircle;
	ball->round = App->physics->CreateCircle(742, 835, 12, b2_dynamicBody);
	ball->round->type = PhysBody::Type::ballCollider;
	ball->round->listener = this;
	ball->lifes = 5;
	ball->points = 0;

	//create ScoreBalls
	scoreBall1 = new ScoreBall;
	scoreBall1->scorebody = App->physics->CreateCircle(400, 400, 30, b2_staticBody);
	scoreBall1->scorebody->type = PhysBody::Type::scoreCollider1;

	scoreBall2 = new ScoreBall;
	scoreBall2->scorebody = App->physics->CreateCircle(400, 440, 30, b2_staticBody);
	scoreBall2->scorebody->type = PhysBody::Type::scoreCollider1;

	scoreBall3 = new ScoreBall;
	scoreBall3->scorebody = App->physics->CreateCircle(400, 480, 30, b2_staticBody);
	scoreBall3->scorebody->type = PhysBody::Type::scoreCollider3;

	//create sensor
	recSensor = new Sensor;
	recSensor->sensorBody = App->physics->CreateRectangle(0, 1020, 1600, 50, b2_staticBody);
	recSensor->sensorBody->type = PhysBody::Type::sensorCollider;

	//Ball Animations
	// 
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

	//Create COLUMNS obstacle
	columns = new Columns;
	//Idle columns animations
	columns->idleColumnAnim.PushBack({ 0, 0, 282, 282 });
	columns->idleColumnAnim.PushBack({ 0, 0, 282, 282 });
	columns->idleColumnAnim.PushBack({ 0, 0, 282, 282 });
	columns->idleColumnAnim.PushBack({ 0, 0, 282, 282 });
	columns->idleColumnAnim.PushBack({ 282, 0, 282, 282 });
	columns->idleColumnAnim.PushBack({ 564, 0, 282, 282 });
	columns->idleColumnAnim.PushBack({ 0, 282, 282, 282 });
	columns->idleColumnAnim.PushBack({ 282, 282, 282, 282 });
	columns->idleColumnAnim.PushBack({ 564, 282, 282, 282 });
	columns->idleColumnAnim.PushBack({ 0, 564, 282, 282 });
	columns->idleColumnAnim.PushBack({ 282, 564, 282, 282 });
	columns->idleColumnAnim.PushBack({ 564, 564, 282, 282 });
	columns->idleColumnAnim.PushBack({ 0, 846, 282, 282 });
	columns->idleColumnAnim.PushBack({ 0, 846, 282, 282 });
	columns->idleColumnAnim.PushBack({ 0, 846, 282, 282 });
	columns->idleColumnAnim.PushBack({ 0, 846, 282, 282 });
	columns->idleColumnAnim.PushBack({ 0, 846, 282, 282 });
	columns->idleColumnAnim.PushBack({ 0, 846, 282, 282 });
	columns->idleColumnAnim.PushBack({ 0, 846, 282, 282 });
	columns->idleColumnAnim.PushBack({ 564, 564, 282, 282 });
	columns->idleColumnAnim.PushBack({ 282, 564, 282, 282 });
	columns->idleColumnAnim.PushBack({ 0, 564, 282, 282 });
	columns->idleColumnAnim.PushBack({ 564, 282, 282, 282 });
	columns->idleColumnAnim.PushBack({ 282, 282, 282, 282 });
	columns->idleColumnAnim.PushBack({ 0, 282, 282, 282 });
	columns->idleColumnAnim.PushBack({ 564, 0, 282, 282 });
	columns->idleColumnAnim.PushBack({ 282, 0, 282, 282 });
	columns->idleColumnAnim.PushBack({ 0, 0, 282, 282 });
	columns->idleColumnAnim.PushBack({ 0, 0, 282, 282 });
	columns->idleColumnAnim.PushBack({ 0, 0, 282, 282 });
	columns->idleColumnAnim.PushBack({ 0, 0, 282, 282 });
	columns->idleColumnAnim.loop = true;
	columns->idleColumnAnim.mustFlip = false;
	columns->idleColumnAnim.speed = 0.1f;

	//Create SUN obstacle
	sun = new SunCircle;
	sun->round = App->physics->CreateCircle(294, 325, 56, b2_staticBody);
	sun->round->listener = this;
	sun->round->type = PhysBody::Type::sunCollider;
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
	sun->collisionSunAnim.speed = 0.23f;
	sunState = SUN_IDLE;

	//SHEEN
	//Create SHEEN obstacle
	sheen = new Sheen;
	sheen->sheen = App->physics->CreateChain(0, 0, rhombus1, 8, b2_staticBody);
	sheen->sheen->type = PhysBody::Type::sheenCollider;
	//Sheen idle animation
	sheen->idleSheenAnim.PushBack({ 504, 0, 72, 72 });
	sheen->idleSheenAnim.loop = false;
	sheen->idleSheenAnim.mustFlip = false;
	sheen->idleSheenAnim.speed = 0.2f;
	//Sheen spawn animation
	sheen->spawnSheenAnim.PushBack({ 0, 0, 72, 72 });
	sheen->spawnSheenAnim.PushBack({ 72, 0, 72, 72 });
	sheen->spawnSheenAnim.PushBack({ 144, 0, 72, 72 });
	sheen->spawnSheenAnim.PushBack({ 216, 0, 72, 72 });
	sheen->spawnSheenAnim.PushBack({ 288, 0, 72, 72 });
	sheen->spawnSheenAnim.PushBack({ 360, 0, 72, 72 });
	sheen->spawnSheenAnim.PushBack({ 432, 0, 72, 72 });
	sheen->spawnSheenAnim.PushBack({ 504, 0, 72, 72 });
	sheen->spawnSheenAnim.loop = false;
	sheen->spawnSheenAnim.mustFlip = false;
	sheen->spawnSheenAnim.speed = 0.2f;
	//Sheen despawn animation
	sheen->despawnSheenAnim.PushBack({ 504, 0, 72, 72 });
	sheen->despawnSheenAnim.PushBack({ 432, 0, 72, 72 });
	sheen->despawnSheenAnim.PushBack({ 360, 0, 72, 72 });
	sheen->despawnSheenAnim.PushBack({ 288, 0, 72, 72 });
	sheen->despawnSheenAnim.PushBack({ 216, 0, 72, 72 });
	sheen->despawnSheenAnim.PushBack({ 144, 0, 72, 72 });
	sheen->despawnSheenAnim.PushBack({ 72, 0, 72, 72 });
	sheen->despawnSheenAnim.PushBack({ 0, 0, 72, 72 });
	sheen->despawnSheenAnim.PushBack({ 0, 0, 0, 0 });
	sheen->despawnSheenAnim.loop = false;
	sheen->despawnSheenAnim.mustFlip = false;
	sheen->despawnSheenAnim.speed = 0.2f;
	//Sheen collision animation
	sheen->collisionSheenAnim.PushBack({ 0, 72, 72, 72 });
	sheen->collisionSheenAnim.PushBack({ 72, 72, 72, 72 });
	sheen->collisionSheenAnim.PushBack({ 144, 72, 72, 72 });
	sheen->collisionSheenAnim.PushBack({ 216, 72, 72, 72 });
	sheen->collisionSheenAnim.PushBack({ 288, 72, 72, 72 });
	sheen->collisionSheenAnim.PushBack({ 360, 72, 72, 72 });
	sheen->collisionSheenAnim.PushBack({ 432, 72, 72, 72 });
	sheen->collisionSheenAnim.PushBack({ 504, 72, 72, 72 });
	sheen->collisionSheenAnim.loop = false;
	sheen->collisionSheenAnim.mustFlip = false;
	sheen->collisionSheenAnim.speed = 0.2f;
	
	sheenState = SHEEN_SPAWN;

	//Create PACHINKO1 obstacle
	PachinkoCircle* pachinko1 = new PachinkoCircle;
	pachinko1->round = App->physics->CreateCircle(180, 682, 15, b2_staticBody);
	//Pachinko idle animation
	pachinko1->idlePachinkoAnim.PushBack({ 0, 0, 29, 30 });
	pachinko1->idlePachinkoAnim.loop = false;
	pachinko1->idlePachinkoAnim.mustFlip = false;
	pachinko1->idlePachinkoAnim.speed = 0.01;
	pachinko1->round->listener = this;
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
	pachinko2->round->listener = this;
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
	pachinko3->round->listener = this;
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
	pachinko4->round = App->physics->CreateCircle(217, 728, 15, b2_staticBody);
	pachinko4->round->listener = this;
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
	pachinko5->round = App->physics->CreateCircle(144, 728, 15, b2_staticBody);
	pachinko5->round->listener = this;
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
	pachinko6->round = App->physics->CreateCircle(144, 820, 15, b2_staticBody);
	pachinko6->round->listener = this;
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
	pachinko7->round = App->physics->CreateCircle(217, 820, 15, b2_staticBody);
	pachinko7->round->listener = this;
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

	//kicker anim
	kicker->kickerAnim.PushBack({ 92, 0, 40, 20 });
	kicker->kickerAnim.PushBack({ 92, 0, 40, 20 });
	kicker->kickerAnim.loop = false;
	kicker->kickerAnim.mustFlip = false;
	kicker->kickerAnim.speed = 0.1;

	p2List_item<PachinkoCircle*>* p = pachinkos.getFirst();
	while (p != NULL)
	{
		p->data->round->type = PhysBody::Type::pachinkoCollider;
		p = p->next;
	}

	bool ret = true;
	App->renderer->camera.x = App->renderer->camera.y = 0;

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
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN) App->physics->kickerJoint->SetMotorSpeed(100.0f);
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP) App->physics->kickerJoint->SetMotorSpeed(-100.0f);

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
		App->renderer->Blit(backgroundGame, 0, 0, NULL, 0.0f, 0);
		App->renderer->Blit(background, 0, 0, NULL, 0.0f, 0);

		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 12));
			circles.getLast()->data->listener = this;
		}

		//UI
		switch (uiState)
		{
		case UI1:
			currentAnim = &ui->uiAnim1;
			break;
		case UI2:
			currentAnim = &ui->uiAnim1;
			break;
		case UI3:
			currentAnim = &ui->uiAnim1;
			break;
		case UI4:
			currentAnim = &ui->uiAnim1;
			break;
		case UI5:
			currentAnim = &ui->uiAnim1;
			break;
		case UI6:
			currentAnim = &ui->uiAnim1;
			break;
		case UI7:
			currentAnim = &ui->uiAnim1;
			break;
		}

		App->renderer->Blit(uiTexture, 0, 0, &(currentAnim->GetCurrentFrame()), 0.0f, 0);

		//BALL STATES
		switch (ballState)
		{
		case BALL_IDLE:
			currentAnim = &ball->idleBallAnim;
			break;

		case BALL_DEATH:
			currentAnim = &ball->deathBallAnim;
			if (ball->deathBallAnim.HasFinished() == true)
			{
				ballState = BALL_SPAWN;
				ball->deathBallAnim.Reset();
				ball->round->body->SetTransform({ PIXEL_TO_METERS(742), PIXEL_TO_METERS(835) }, 0.0f);
				ball->lifes--;
			}
			break;

		case BALL_SPAWN:
			currentAnim = &ball->spawnBallAnim;
			if (ball->spawnBallAnim.HasFinished() == true)
			{
				ballState = BALL_IDLE;
				ball->spawnBallAnim.Reset();
			}
			break;
		}

		int x, y;
		ball->round->GetPosition(x, y);
		App->renderer->Blit(ballTexture, x - 7, y - 7, &(currentAnim->GetCurrentFrame()), 0.0f, 0.0f);
		ball->idleBallAnim.Update();
		ball->deathBallAnim.Update();
		ball->spawnBallAnim.Update();

		//COLUMNS
		currentAnim = &columns->idleColumnAnim;
		App->renderer->Blit(columnsTexture, 380, 254, &(currentAnim->GetCurrentFrame()), 0.0f, 0, 0, 0);
		columns->idleColumnAnim.Update();

		//SHEEN
		switch (sheenState)
		{
		case SHEEN_IDLE:
			sheen->spawnSheenAnim.Reset();
			currentAnim = &sheen->idleSheenAnim;
			break;
		case SHEEN_COLLISION:
			sheen->spawnSheenAnim.Reset();
			currentAnim = &sheen->collisionSheenAnim;
			break;
		case SHEEN_SPAWN:
			sheen->despawnSheenAnim.Reset();

			currentAnim = &sheen->spawnSheenAnim;
			break;
		case SHEEN_DESPAWN:
			sheen->spawnSheenAnim.Reset();
			currentAnim = &sheen->despawnSheenAnim;
			break;
		}

		if ((sheenState == SHEEN_COLLISION) && (sheen->collisionSheenAnim.HasFinished() == true))
		{
			sheen->collisionSheenAnim.Reset();
			sheen->idleSheenAnim.Reset();
			sheenState = SHEEN_IDLE;
		}


		counter++;

		if (counter < 180)
		{
			if (sheenState == SHEEN_DESPAWN && sheen->despawnSheenAnim.HasFinished() == true)
			{
				sheenState = SHEEN_SPAWN;
			}
			/*else if (sheenState == SHEEN_SPAWN && sheen->spawnSheenAnim.HasFinished() == true)
			{
				sheenState = SHEEN_DESPAWN;
			}*/
			sheen->sheen->body->GetWorld()->DestroyBody(sheen->sheen->body);
			sheen->sheen = App->physics->CreateChain(0, 0, rhombus1, 8, b2_staticBody);
			sheen->sheen->type = PhysBody::Type::sheenCollider;
			App->renderer->Blit(sheenTexture, 185, 533, &(currentAnim->GetCurrentFrame()), 1.0f);
		}
		if (counter < 360 && counter > 180)
		{
			if (sheen->despawnSheenAnim.HasFinished() == true)
			{
				sheenState = SHEEN_SPAWN;
			}
			/*if (sheen->spawnSheenAnim.HasFinished() == true)
			{
				sheenState = SHEEN_DESPAWN;
			}*/
			sheen->sheen->body->GetWorld()->DestroyBody(sheen->sheen->body);
			sheen->sheen = App->physics->CreateChain(0, 0, rhombus2, 8, b2_staticBody);
			sheen->sheen->type = PhysBody::Type::sheenCollider;
			App->renderer->Blit(sheenTexture, 297, 478, &(currentAnim->GetCurrentFrame()), 1.0f);
		}
		if (counter < 540 && counter > 360)
		{
			if (sheen->despawnSheenAnim.HasFinished() == true)
			{
				sheenState = SHEEN_SPAWN;
			}
			/*if (sheen->spawnSheenAnim.HasFinished() == true)
			{
				sheenState = SHEEN_DESPAWN;
			}*/
			sheen->sheen->body->GetWorld()->DestroyBody(sheen->sheen->body);
			sheen->sheen = App->physics->CreateChain(0, 0, rhombus3, 8, b2_staticBody);
			sheen->sheen->type = PhysBody::Type::sheenCollider;
			App->renderer->Blit(sheenTexture, 322, 599, &(currentAnim->GetCurrentFrame()), 1.0f);
		}
		if (counter > 540) counter = 0;

		sheen->despawnSheenAnim.Update();
		sheen->spawnSheenAnim.Update();

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

		App->renderer->Blit(sunTexture, 185, 215, &(currentAnim->GetCurrentFrame()), 1.0f);

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
				if (i == 3) { App->renderer->Blit(pachinkoTexture, x + 37, y + 46, &(currentAnim->GetCurrentFrame()), 1.0f); }
				if (i == 4) { App->renderer->Blit(pachinkoTexture, x - 38, y + 46, &(currentAnim->GetCurrentFrame()), 1.0f); }
				if (i == 5) { App->renderer->Blit(pachinkoTexture, x - 38, y + 138, &(currentAnim->GetCurrentFrame()), 1.0f); }
				if (i == 6) { App->renderer->Blit(pachinkoTexture, x + 37, y + 138, &(currentAnim->GetCurrentFrame()), 1.0f); }
			}

			p = p->next;
		}

		//kicker anim
		if (kicker->kickerAnim.HasFinished())
		{
			kicker->kickerAnim.Reset();
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

	if (bodyA != nullptr && bodyB != nullptr)
	{
		// ball-sun collision
		if (bodyA->type == PhysBody::Type::ballCollider && bodyB->type == PhysBody::Type::sunCollider)
		{
			App->audio->PlayFx(sun_fx);
			sunState = SUN_COLLISION;
		}

		// ball-flippers collision
		if (bodyA->type == PhysBody::Type::ballCollider && bodyB->type == PhysBody::Type::flipperCollider && App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			App->audio->PlayFx(flippers_fx);
		}

		//ball-sheen collision
		if (bodyA->type == PhysBody::Type::ballCollider && bodyB->type == PhysBody::Type::sheenCollider)
		{
			int random = 0;
			srand(time(NULL));
			random = rand() % 2;
			if (random == 0) App->audio->PlayFx(sheen1_fx);
			if (random == 1) App->audio->PlayFx(sheen2_fx);
		}

		// ball-wall collision
		if (bodyA->type == PhysBody::Type::ballCollider && bodyB->type == PhysBody::Type::wallCollider)
		{
			int random = 0;
			srand(time(NULL));
			random = rand() % 3;
			if (random == 0) App->audio->PlayFx(ball1_fx);
			if (random == 1) App->audio->PlayFx(ball2_fx);
			if (random == 2) App->audio->PlayFx(ball3_fx);
		}

		// ball-sensor collision
		if (bodyA->type == PhysBody::Type::ballCollider && bodyB->type == PhysBody::Type::sensorCollider)
		{
			App->audio->PlayFx(ballDeath_fx);
			ballState = BALL_DEATH;
		}

		// ball-scoreballs collision
		if (bodyA->type == PhysBody::Type::ballCollider && bodyB->type == PhysBody::Type::scoreCollider1)
		{
			ball->points += 1;
		}
		if (bodyA->type == PhysBody::Type::ballCollider && bodyB->type == PhysBody::Type::scoreCollider3)
		{
			ball->points += 3;
		}
	}
}