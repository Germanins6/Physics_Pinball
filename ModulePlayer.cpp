#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleFonts.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	textball = NULL;
	ball = nullptr;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	textball = App->textures->Load("sprites/ball2.png");
	fontpoints = App->fonts->Load("Sprites/preloaderfont.png", "1234567890%", 1);
	ball = App->physics->CreateCircle(570,913,15);


	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");
	App->fonts->UnLoad(fontpoints);
	App->textures->Unload(textball);
	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_REPEAT) {
		puntos += 212;
	}

	sprintf_s(points_text, 10, "%7d", puntos);
	App->fonts->BlitText(0, 0, fontpoints, points_text);

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		vely += -0.5f;
		thrower = true;

	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
	{
		if (ball != nullptr && App->scene_intro->ballthrow == true)
			ball->body->SetLinearVelocity(b2Vec2(0, vely));

		
		vely = 0;
		thrower = false;
	}


	return UPDATE_CONTINUE;
}



