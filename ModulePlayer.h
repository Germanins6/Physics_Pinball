#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModulePhysics.h"

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

	void Restart(bool reset = false);
	void BlitPoints();

public:
	PhysBody* ball;
	SDL_Texture* textball;
	bool thrower;
	float vely;
	int lifes = 3;
	int puntos = 0;
	int fontpoints = -1;
	char points_text[10];


};