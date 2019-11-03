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
	int lifes = 3;
	bool reset = false;


	/*Fonts and points*/
	int fontpoints = -1;
	int CurrentScore = 0;
	char points_text[10];
	int	TotalScore = 0;
	char points_total[10];
	int MaxScore = 0;
	char points_max[10];



};