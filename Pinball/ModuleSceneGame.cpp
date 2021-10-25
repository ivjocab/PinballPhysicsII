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
	background = ball = pachinko = sunTexture = box = NULL;
	ray_on = false;
	sensed = false;
}

ModuleSceneGame::~ModuleSceneGame()
{}

// Load assets
bool ModuleSceneGame::Start()
{
	LOG("Loading Game assets & variables needed");

	b2Vec2 veca = { -0.44, 0 };
	b2Vec2 vecb = { 0, 0 };

	//CREATING BODIES AND OBSTACLES
	//BACKGROUND chains
	backgrounds.add(App->physics->CreateChain(0, 0, bg, 106, b2_staticBody));

	//Create SUN obstacle
	Circle* sun = new Circle;
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

	// Flippers --------------------------------------------------------------
	Flipper* f1 = new Flipper;
	f1->Circle = App->physics->CreateCircle(300, 965, 4, b2_staticBody);
	f1->Rect = App->physics->CreateRectangle(310 + rectSect.w / 3, 950 + rectSect.h / 2, rectSect.w, rectSect.h - 10, b2_dynamicBody);
	f1->rightSide = false;
	App->physics->CreateRevoluteJoint(f1->Rect, veca, f1->Circle, vecb, 35.0f);
	flippers.add(f1);

	Flipper* f2 = new Flipper;
	f2->Circle = App->physics->CreateCircle(500, 965, 4, b2_staticBody);
	f2->Rect = App->physics->CreateRectangle(510 + rectSect.w / 3, 950 + rectSect.h / 2, rectSect.w, rectSect.h - 10, b2_dynamicBody);
	f2->rightSide = true;
	App->physics->CreateRevoluteJoint(f2->Rect, veca, f2->Circle, vecb, 35.0f);
	flippers.add(f2);

	veca = { 0.44,0 };

	//ANIMATIONS
	//Ball Animations
	//Idle
	idleBallAnim.PushBack({ 0, 0, 39, 38 });
	idleBallAnim.PushBack({ 38, 0, 39, 38 });
	idleBallAnim.PushBack({ 76, 0, 39, 38 });
	idleBallAnim.PushBack({ 114, 0, 39, 38 });
	idleBallAnim.PushBack({ 152, 0, 39, 38 });
	idleBallAnim.PushBack({ 190, 0, 39, 38 });
	idleBallAnim.PushBack({ 228, 0, 39, 38 });
	idleBallAnim.PushBack({ 266, 0, 39, 38 });
	idleBallAnim.PushBack({ 304, 0, 39, 38 });
	idleBallAnim.PushBack({ 342, 0, 39, 38 });
	idleBallAnim.PushBack({ 380, 0, 39, 38 });
	idleBallAnim.PushBack({ 418, 0, 39, 38 });
	idleBallAnim.PushBack({ 456, 0, 39, 38 });
	idleBallAnim.PushBack({ 494, 0, 39, 38 });
	idleBallAnim.PushBack({ 532, 0, 39, 38 });
	idleBallAnim.PushBack({ 570, 0, 39, 38 });
	idleBallAnim.loop = true;
	idleBallAnim.mustFlip = false;
	idleBallAnim.speed = 0.5f;

	//Death
	deathBallAnim.PushBack({ 0, 39, 39, 38 });
	deathBallAnim.PushBack({ 38, 39, 39, 38 });
	deathBallAnim.PushBack({ 76, 39, 39, 38 });
	deathBallAnim.PushBack({ 114, 39, 39, 38 });
	deathBallAnim.PushBack({ 152, 39, 39, 38 });
	deathBallAnim.PushBack({ 190, 39, 39, 38 });
	deathBallAnim.PushBack({ 228, 39, 39, 38 });
	deathBallAnim.PushBack({ 266, 39, 39, 38 });
	deathBallAnim.PushBack({ 304, 39, 39, 38 });
	deathBallAnim.PushBack({ 342, 39, 39, 38 });
	deathBallAnim.PushBack({ 380, 39, 39, 38 });
	deathBallAnim.PushBack({ 418, 39, 39, 38 });
	deathBallAnim.PushBack({ 456, 39, 39, 38 });
	deathBallAnim.PushBack({ 494, 39, 39, 38 });
	deathBallAnim.PushBack({ 532, 39, 39, 38 });
	deathBallAnim.PushBack({ 570, 39, 39, 38 });
	deathBallAnim.PushBack({ 0, 78, 39, 38 });
	deathBallAnim.PushBack({ 38, 78, 39, 38 });
	deathBallAnim.PushBack({ 76, 78, 39, 38 });
	deathBallAnim.PushBack({ 114, 78, 39, 38 });
	deathBallAnim.PushBack({ 152, 78, 39, 38 });
	deathBallAnim.PushBack({ 190, 78, 39, 38 });
	deathBallAnim.PushBack({ 228, 78, 39, 38 });
	deathBallAnim.PushBack({ 266, 78, 39, 38 });
	deathBallAnim.loop = false;
	deathBallAnim.mustFlip = false;
	deathBallAnim.speed = 0.2f;

	//Spawn
	spawnBallAnim.PushBack({ 266, 78, 39, 38 });
	spawnBallAnim.PushBack({ 228, 78, 39, 38 });
	spawnBallAnim.PushBack({ 190, 78, 39, 38 });
	spawnBallAnim.PushBack({ 152, 78, 39, 38 });
	spawnBallAnim.PushBack({ 114, 78, 39, 38 });
	spawnBallAnim.PushBack({ 76, 78, 39, 38 });
	spawnBallAnim.PushBack({ 38, 78, 39, 38 });
	spawnBallAnim.PushBack({ 0, 78, 39, 38 });
	spawnBallAnim.PushBack({ 570, 39, 39, 38 });
	spawnBallAnim.PushBack({ 532, 39, 39, 38 });
	spawnBallAnim.PushBack({ 494, 39, 39, 38 });
	spawnBallAnim.PushBack({ 456, 39, 39, 38 });
	spawnBallAnim.PushBack({ 418, 39, 39, 38 });
	spawnBallAnim.PushBack({ 380, 39, 39, 38 });
	spawnBallAnim.PushBack({ 342, 39, 39, 38 });
	spawnBallAnim.PushBack({ 304, 39, 39, 38 });
	spawnBallAnim.PushBack({ 266, 39, 39, 38 });
	spawnBallAnim.PushBack({ 228, 39, 39, 38 });
	spawnBallAnim.PushBack({ 190, 39, 39, 38 });
	spawnBallAnim.PushBack({ 152, 39, 39, 38 });
	spawnBallAnim.PushBack({ 114, 39, 39, 38 });
	spawnBallAnim.PushBack({ 76, 39, 39, 38 });
	spawnBallAnim.PushBack({ 38, 39, 39, 38 });
	spawnBallAnim.PushBack({ 0, 39, 39, 38 });
	spawnBallAnim.loop = false;
	spawnBallAnim.mustFlip = false;
	spawnBallAnim.speed = 0.2f;

	//Sun animations
	//Idle
	idleSunAnim.PushBack({ 0, 0, 220, 218 });
	idleSunAnim.PushBack({ 218, 0, 220, 218 });
	idleSunAnim.PushBack({ 436, 0, 220, 218 });
	idleSunAnim.PushBack({ 0, 220, 220, 218 });
	idleSunAnim.PushBack({ 218, 220, 220, 218 });
	idleSunAnim.PushBack({ 436, 220, 220, 218 });
	idleSunAnim.PushBack({ 0, 440, 220, 218 });
	idleSunAnim.PushBack({ 218, 440, 220, 218 });
	idleSunAnim.PushBack({ 436, 440, 220, 218 });
	idleSunAnim.PushBack({ 218, 440, 220, 218 });
	idleSunAnim.PushBack({ 0, 440, 220, 218 });
	idleSunAnim.PushBack({ 436, 220, 220, 218 });
	idleSunAnim.PushBack({ 218, 220, 220, 218 });
	idleSunAnim.PushBack({ 0, 220, 220, 218 });
	idleSunAnim.PushBack({ 436, 0, 220, 218 });
	idleSunAnim.PushBack({ 218, 0, 220, 218 });
	idleSunAnim.loop = true;
	idleSunAnim.mustFlip = false;
	idleSunAnim.speed = 0.12f;

	//Collision
	collisionSunAnim.PushBack({ 0, 660, 220, 218 });
	collisionSunAnim.PushBack({ 218, 660, 220, 218 });
	collisionSunAnim.PushBack({ 436, 660, 220, 218 });
	collisionSunAnim.PushBack({ 0, 880, 220, 218 });
	collisionSunAnim.PushBack({ 218, 880, 220, 218 });
	collisionSunAnim.PushBack({ 436, 880, 220, 218 });
	collisionSunAnim.PushBack({ 0, 1100, 220, 218 });
	collisionSunAnim.PushBack({ 218, 1100, 220, 218 });
	collisionSunAnim.PushBack({ 436, 1100, 220, 218 });
	collisionSunAnim.PushBack({ 218, 1100, 220, 218 });
	collisionSunAnim.PushBack({ 0, 1100, 220, 218 });
	collisionSunAnim.PushBack({ 436, 880, 220, 218 });
	collisionSunAnim.PushBack({ 218, 880, 220, 218 });
	collisionSunAnim.PushBack({ 0, 880, 220, 218 });
	collisionSunAnim.PushBack({ 436, 660, 220, 218 });
	collisionSunAnim.PushBack({ 218, 660, 220, 218 });
	collisionSunAnim.PushBack({ 0, 660, 220, 218 });
	collisionSunAnim.loop = false;
	collisionSunAnim.mustFlip = false;
	collisionSunAnim.speed = 0.28f;

	//Pachinko animations
	//Idle
	idlePachinkoAnim.PushBack({ 0, 0, 29, 30 });
	idlePachinkoAnim.loop = false;
	idlePachinkoAnim.mustFlip = false;
	idlePachinkoAnim.speed = 0.01;

	//Random
	randomPachinkoAnim.PushBack({ 0, 0, 30, 30 });
	randomPachinkoAnim.PushBack({ 30, 0, 30, 30 });
	randomPachinkoAnim.PushBack({ 60, 0, 30, 30 });
	randomPachinkoAnim.PushBack({ 90, 0, 30, 30 });
	randomPachinkoAnim.PushBack({ 120, 0, 30, 30 });
	randomPachinkoAnim.PushBack({ 150, 0, 30, 30 });
	randomPachinkoAnim.PushBack({ 180, 0, 30, 30 });
	randomPachinkoAnim.PushBack({ 210, 0, 30, 30 });
	randomPachinkoAnim.PushBack({ 240, 0, 30, 30 });
	randomPachinkoAnim.PushBack({ 270, 0, 30, 30 });
	randomPachinkoAnim.PushBack({ 300, 0, 30, 30 });
	randomPachinkoAnim.PushBack({ 330, 0, 30, 30 });
	randomPachinkoAnim.PushBack({ 360, 0, 30, 30 });
	randomPachinkoAnim.PushBack({ 390, 0, 30, 30 });
	randomPachinkoAnim.PushBack({ 420, 0, 30, 30 });
	randomPachinkoAnim.PushBack({ 450, 0, 30, 30 });
	randomPachinkoAnim.PushBack({ 480, 0, 30, 30 });
	randomPachinkoAnim.PushBack({ 510, 0, 30, 30 });
	randomPachinkoAnim.PushBack({ 540, 0, 30, 30 });
	randomPachinkoAnim.loop = false;
	randomPachinkoAnim.mustFlip = false;
	randomPachinkoAnim.speed = 0.4;
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	background = App->textures->Load("pinball/background.png");
	ball = App->textures->Load("pinball/ball.png");
	sunTexture = App->textures->Load("pinball/sun.png");
	pachinko = App->textures->Load("pinball/pachinko.png");
	box = App->textures->Load("pinball/crate.png");
	intro = App->audio->LoadFx("pinball/intro.wav");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");

	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);

	//App->audio->PlayFx(intro, 1);

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
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 12));
		circles.getLast()->data->listener = this;
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		p2List_item<Flipper*>* f = flippers.getFirst();
		while (f != NULL)
		{
			if (f->data->rightSide == false)
			{
				f->data->Rect->body->ApplyForce({ -3,0 }, { 0,0 }, true);
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
				f->data->Rect->body->ApplyForce({ 3,0 }, { 0,0 }, true);
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
	p2List_item<PhysBody*>* c = circles.getFirst();

	App->renderer->Blit(background, 0, 0, NULL, 0.0f, 0);

	//BALL



	if (ballState != BALL_DEATH)
	{
		ballState = BALL_SPAWN;
		if (spawnBallAnim.HasFinished())
		{
			ballState = BALL_IDLE;
		}
	}

	switch (ballState)
	{
	case BALL_IDLE:
		currentAnim = &idleBallAnim;
		break;

	case BALL_DEATH:
		currentAnim = &deathBallAnim;
		break;

	case BALL_SPAWN:
		currentAnim = &spawnBallAnim;
	}

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(ball, x - 7, y - 7, &(currentAnim->GetCurrentFrame()), 1.0f, true);
		c = c->next;
		idleBallAnim.Update();
		deathBallAnim.Update();
		spawnBallAnim.Update();
	}

	//SUN

	switch (sunState)
	{
	case SUN_IDLE:
		currentAnim = &idleSunAnim;
		break;

	case SUN_COLLISION:
		currentAnim = &collisionSunAnim;
		break;
	}

	if (sunState == SUN_IDLE) { idleSunAnim.Update(); }
	else { collisionSunAnim.Update(); }

	if ((sunState == SUN_COLLISION) && (collisionSunAnim.HasFinished()))
	{
		collisionSunAnim.Reset();
		idleSunAnim.Reset();
		sunState = SUN_IDLE;
	}

	App->renderer->Blit(sunTexture, 226, 230, &(currentAnim->GetCurrentFrame()), 1.0f);

	//BOX

	c = boxes.getFirst();

	while (c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		if (ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if (hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}

	//PACHINKO
	int random = 0;
	srand(time(NULL));
	random = rand() % 40;

	if (random >= 35)
	{
		pachinkoState = PACHINKO_RANDOM;
	}
	
	random = 0;

	switch (pachinkoState)
	{
	case PACHINKO_IDLE:
		currentAnim = &randomPachinkoAnim;
		break;

	case PACHINKO_RANDOM:
		currentAnim = &randomPachinkoAnim;
		break;

	case PACHINKO_COLLISION:
		currentAnim = &collisionPachinkoAnim;
		break;
	}

	if (pachinkoState == PACHINKO_IDLE) { idlePachinkoAnim.Update(); }
	if (pachinkoState == PACHINKO_RANDOM) { randomPachinkoAnim.Update(); }
	if (pachinkoState == PACHINKO_COLLISION) { collisionPachinkoAnim.Update(); }

	if (pachinkoState == PACHINKO_IDLE || randomPachinkoAnim.HasFinished() == true)
	{
		randomPachinkoAnim.Reset();
		collisionPachinkoAnim.Reset();
		pachinkoState = PACHINKO_IDLE;
	}

	App->renderer->Blit(pachinko, 166, 668, &(currentAnim->GetCurrentFrame()), 1.0f);




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