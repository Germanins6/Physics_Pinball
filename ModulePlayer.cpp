#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleFonts.h"
#include "ModulePhysics.h"

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
	ball = App->physics->CreateCircle(570, 913, 14);
	
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
	if(!App->physics->debug)DebugCreateCircles();				//REMEMBER DELETE THIS FUNCTION OR SET IN DEBUG MODE NOW IN TEST
	if (reset) ResetBall();
	ResetGame();

	return UPDATE_CONTINUE;
}

void ModulePlayer::BlitPoints() {
	sprintf_s(points_text, 10, "%7d", CurrentScore);
	App->fonts->BlitText(200, 30, fontpoints, points_text);

	sprintf_s(points_total, 10, "%7d", TotalScore);
	App->fonts->BlitText(402, 22, fontpoints, points_total);

	sprintf_s(points_max, 10, "%7d", MaxScore);
	App->fonts->BlitText(402, 80, fontpoints, points_max);

}

void ModulePlayer::DrawBall() {
	if (ball != NULL) {
		int x, y;
		ball->GetPosition(x, y);
		App->renderer->Blit(textball, x, y, NULL);
	}
}

void ModulePlayer::ResetBall() {
	ball->body->SetTransform({ PIXEL_TO_METERS(570), PIXEL_TO_METERS(913) }, 0.0f);
	ball->body->SetLinearVelocity({ 0,0 });
	reset = false;
}

void ModulePlayer::DebugCreateCircles() {
	//Create test circles
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		App->scene_intro->circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 14));
		App->renderer->Blit(App->scene_intro->circle, App->input->GetMouseX(), App->input->GetMouseY());
	}
}

void ModulePlayer::ResetGame() {
	if (lifes <= 0) {
		if (TotalScore > MaxScore) MaxScore = TotalScore;
		TotalScore = 0;
		CurrentScore = 0;
		lifes = 3;
		ResetBall();
	}
}