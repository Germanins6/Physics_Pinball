#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleFonts.h"

#include <string.h>
#include <stdio.h>

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	fontpoints = App->fonts->Load("Sprites/preloaderfont.png", "1234567890%", 1);

	LOG("Loading player");
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	App->fonts->UnLoad(fontpoints);
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{

	puntos += 1;
	sprintf_s(points_text, 10, "%7d",puntos);
	App->fonts->BlitText(0,0, fontpoints, points_text);

	return UPDATE_CONTINUE;
}



