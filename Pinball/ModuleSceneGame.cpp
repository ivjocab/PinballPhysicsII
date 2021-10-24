#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneGame.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleSceneGame::ModuleSceneGame(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	background = ball = sun = box = NULL;
	ray_on = false;
	sensed = false;

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
}

ModuleSceneGame::~ModuleSceneGame()
{}

// Load assets
bool ModuleSceneGame::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	background = App->textures->Load("pinball/background.png");
	ball = App->textures->Load("pinball/ball.png");
	sun = App->textures->Load("pinball/sun.png");
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

	if ((sunState == SUN_COLLISION) && (collisionSunAnim.HasFinished()))
	{
		collisionSunAnim.Reset();
		idleSunAnim.Reset();
		sunState = SUN_IDLE;
	}

	App->renderer->Blit(sun, 226, 230, &(currentAnim->GetCurrentFrame()), 1.0f);
	if (sunState == SUN_IDLE){ idleSunAnim.Update(); }
	else { collisionSunAnim.Update(); }


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