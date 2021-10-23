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
	idleBallAnim.PushBack({ -5, 7, 50, 50 });
	idleBallAnim.PushBack({ 43, 7, 50, 50 });
	idleBallAnim.PushBack({ 90, 7, 50, 50 });
	idleBallAnim.PushBack({ 138, 7, 50, 50 });
	idleBallAnim.PushBack({ 184, 7, 50, 50 });
	idleBallAnim.PushBack({ 230, 7, 50, 50 });
	idleBallAnim.PushBack({ 276, 7, 50, 50 });
	idleBallAnim.PushBack({ 322, 7, 50, 50 });
	idleBallAnim.PushBack({ 368, 7, 50, 50 });
	idleBallAnim.PushBack({ 414, 7, 50, 50 });
	idleBallAnim.PushBack({ 460, 7, 50, 50 });
	idleBallAnim.PushBack({ 506, 7, 50, 50 });
	idleBallAnim.PushBack({ 552, 7, 50, 50 });
	idleBallAnim.PushBack({ 598, 7, 50, 50 });
	idleBallAnim.PushBack({ 644, 7, 50, 50 });
	idleBallAnim.PushBack({ 690, 7, 50, 50 });
	idleBallAnim.loop = true;
	idleBallAnim.mustFlip = false;
	idleBallAnim.speed = 0.2f;

	//Death
	deathBallAnim.PushBack({ 0, 57, 50, 50 });
	deathBallAnim.PushBack({ 46, 57, 50, 50 });
	deathBallAnim.PushBack({ 92, 57, 50, 50 });
	deathBallAnim.PushBack({ 138, 57, 50, 50 });
	deathBallAnim.PushBack({ 184, 57, 50, 50 });
	deathBallAnim.PushBack({ 230, 57, 50, 50 });
	deathBallAnim.PushBack({ 276, 57, 50, 50 });
	deathBallAnim.PushBack({ 322, 57, 50, 50 });
	deathBallAnim.PushBack({ 368, 57, 50, 50 });
	deathBallAnim.PushBack({ 414, 57, 50, 50 });
	deathBallAnim.PushBack({ 460, 57, 50, 50 });
	deathBallAnim.PushBack({ 506, 57, 50, 50 });
	deathBallAnim.PushBack({ 552, 57, 50, 50 });
	deathBallAnim.PushBack({ 598, 57, 50, 50 });
	deathBallAnim.PushBack({ 644, 57, 50, 50 });
	deathBallAnim.PushBack({ 694, 57, 46, 50 });
	deathBallAnim.PushBack({ 0, 104, 50, 50 });
	deathBallAnim.PushBack({ 46, 104, 50, 50 });
	deathBallAnim.PushBack({ 92, 104, 50, 50 });
	deathBallAnim.PushBack({ 138, 104, 50, 50 });
	deathBallAnim.PushBack({ 184, 104, 50, 50 });
	deathBallAnim.PushBack({ 230, 104, 50, 50 });
	deathBallAnim.PushBack({ 276, 104, 50, 50 });
	deathBallAnim.PushBack({ 322, 104, 50, 50 });
	deathBallAnim.PushBack({ 0, 200, 50, 50 });
	deathBallAnim.loop = false;
	deathBallAnim.mustFlip = false;
	deathBallAnim.speed = 0.2f;

	//Spawn
	spawnBallAnim.PushBack({ 322, 104, 50, 50 });
	spawnBallAnim.PushBack({ 276, 104, 50, 50 });
	spawnBallAnim.PushBack({ 230, 104, 50, 50 });
	spawnBallAnim.PushBack({ 184, 104, 50, 50 });
	spawnBallAnim.PushBack({ 138, 104, 50, 50 });
	spawnBallAnim.PushBack({ 92, 104, 50, 50 });
	spawnBallAnim.PushBack({ 46, 104, 50, 50 });
	spawnBallAnim.PushBack({ 0, 104, 50, 50 });
	spawnBallAnim.PushBack({ 694, 57, 46, 50 });
	spawnBallAnim.PushBack({ 644, 57, 50, 50 });
	spawnBallAnim.PushBack({ 598, 57, 50, 50 });
	spawnBallAnim.PushBack({ 552, 57, 50, 50 });
	spawnBallAnim.PushBack({ 506, 57, 50, 50 });
	spawnBallAnim.PushBack({ 460, 57, 50, 50 });
	spawnBallAnim.PushBack({ 414, 57, 50, 50 });
	spawnBallAnim.PushBack({ 368, 57, 50, 50 });
	spawnBallAnim.PushBack({ 322, 57, 50, 50 });
	spawnBallAnim.PushBack({ 276, 57, 50, 50 });
	spawnBallAnim.PushBack({ 230, 57, 50, 50 });
	spawnBallAnim.PushBack({ 184, 57, 50, 50 });
	spawnBallAnim.PushBack({ 138, 57, 50, 50 });
	spawnBallAnim.PushBack({ 92, 57, 50, 50 });
	spawnBallAnim.PushBack({ 44, 57, 50, 50 });
	spawnBallAnim.PushBack({ -5, 57, 50, 50 });
	spawnBallAnim.loop = false;
	spawnBallAnim.mustFlip = false;
	spawnBallAnim.speed = 0.2f;

	//Sun animations
	//Idle
	idleSunAnim.PushBack({ 0, 0, 229, 229 });
	idleSunAnim.PushBack({ 230, 0, 229, 229 });
	idleSunAnim.PushBack({ 461, 0, 229, 229 });
	idleSunAnim.PushBack({ 0, 230, 229, 229 });
	idleSunAnim.PushBack({ 230, 230, 229, 229 });
	idleSunAnim.PushBack({ 461, 230, 229, 229 });
	idleSunAnim.PushBack({ 0, 0, 461, 229 });
	idleSunAnim.PushBack({ 230, 0, 461, 229 });
	idleSunAnim.PushBack({ 461, 0, 461, 229 });

	//Collision
	collisionSunAnim.PushBack({ 0, 0, 229, 229 });
	collisionSunAnim.PushBack({ 230, 0, 229, 229 });
	collisionSunAnim.PushBack({ 461, 0, 229, 229 });
	collisionSunAnim.PushBack({ 0, 230, 229, 229 });
	collisionSunAnim.PushBack({ 230, 230, 229, 229 });
	collisionSunAnim.PushBack({ 461, 230, 229, 229 });
	collisionSunAnim.PushBack({ 0, 0, 461, 229 });
	collisionSunAnim.PushBack({ 230, 0, 461, 229 });
	collisionSunAnim.PushBack({ 461, 0, 461, 229 });
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
		App->renderer->Blit(ball, x - 14, y - 8, &(currentAnim->GetCurrentFrame()), 1.0f, true);
		c = c->next;
		idleBallAnim.Update();
		deathBallAnim.Update();
		spawnBallAnim.Update();
	}

	//SUN
	sunState = SUN_IDLE;

	if (sunState != SUN_COLLISION)
	{
		sunState = SUN_IDLE;
	}

	switch (sunState)
	{
	case SUN_IDLE:
		currentAnim = &idleSunAnim;
		break;

	case SUN_COLLISION:
		currentAnim = &collisionSunAnim;
		break;
	}

	while (c != NULL)
	{
		App->renderer->Blit(sun, 0, 0, &(currentAnim->GetCurrentFrame()), 1.0f, true);
		idleSunAnim.Update();
		collisionSunAnim.Update();
	}

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