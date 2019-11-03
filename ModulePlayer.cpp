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
	fontpoints = App->fonts->Load("sprites/preloaderfont.png", "1234567890%", 1);
	ResetBall();

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
	DrawBall();
	App->scene_intro->PlayerInputs();
	BlitPoints();

	return UPDATE_CONTINUE;
}

void ModulePlayer::BlitPoints() {
	sprintf_s(points_text, 10, "%7d", puntos);
	App->fonts->BlitText(200, 30, fontpoints, points_text);
}

void ModulePlayer::DrawBall() {
	if (ball != NULL) {
		int x, y;
		ball->GetPosition(x, y);
		App->renderer->Blit(textball, x, y, NULL);
	}
}

void ModulePlayer::ResetBall() {
	ball = App->physics->CreateCircle(570, 913, 15);
}