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

	void ResetBall();
	void BlitPoints();
	void DrawBall();
	void DebugCreateCircles();
	void ResetGame();

public:
	PhysBody* ball;
	SDL_Texture* textball;
	bool thrower;
	float vely;
	int lifes = 3;
	int CurrentScore = 0;
	int	TotalScore = 0;
	int MaxScore = 0;
	int fontpoints = -1;
	char points_text[10];
	bool reset = false;


};