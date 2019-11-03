#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "ModulePlayer.h"
#include "Animation.h"

#define MAX_FORCE 1200;

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	void DrawMap();
	void SetBackgroundColliders();
	void SetPaddles();	
	void SetThrower();
	void PlayerInputs();
	void DrawLifes();
	void LoadTextures();
	void CheckInteractions();
	void Sensors();
	void SetReboters();

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;
	
	bool sensed;

	SDL_Texture* circle;

	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;
	bool ballthrow;
	SDL_Texture* background;
	

public:


	//THROWER
	PhysBody* thrower;
	PhysBody* staticThrower;
	b2PrismaticJointDef prismaticJoint_launcher;
	SDL_Texture* textthrower;
	int ThrowForce = 0;


	//PADDLES
	PhysBody* left;
	PhysBody* right;
	PhysBody* point_right;
	PhysBody* point_left;
	b2RevoluteJointDef revoluteJointDef_right;
	b2RevoluteJointDef revoluteJointDef_left;
	SDL_Texture* paddlestex;
	SDL_Rect paddle_left;
	SDL_Rect paddle_right;

	//LIVES AND SCORE
	SDL_Texture* lifes;
	SDL_Rect score;
	SDL_Rect nolife;
	SDL_Rect onelife;
	SDL_Rect twolifes;
	SDL_Rect all_lifes;

	//INTERACTIVE ELEMENTS
	SDL_Texture* dynElements;
	SDL_Rect GreenStick;
	SDL_Rect RebotersOT;
	SDL_Rect RebotersTF;

	/*Middle Circles*/
	SDL_Rect CircleGlow;
	PhysBody* CircleOne;
	PhysBody* CircleTwo;
	PhysBody* CircleThree;

	/*Middle Circle Sensors*/
	PhysBody* SensCircleOne;
	PhysBody* SensCircleTwo;
	PhysBody* SensCircleThree;
	bool EnableCircleOne = false;
	bool EnableCircleTwo = false;
	bool EnableCircleThree = false;

	/*Top Green Sensors*/
	PhysBody* GreenSensorOne;
	PhysBody* GreenSensorTwo;
	PhysBody* GreenSensorThree;
	PhysBody* GreenSensorFour;
	bool Sens_GreenOne = false;
	bool Sens_GreenTwo = false;
	bool Sens_GreenThree = false;
	bool Sens_GreenFour = false;

	/*Lost ball Sensor*/
	PhysBody* LostBallSensor;
	
	/*Reboters*/
	PhysBody* ReboterOne;
	PhysBody* ReboterTwo;
	PhysBody* ReboterThree;
	PhysBody* ReboterFour;
	

	/*Reboters Sensors*/
	PhysBody* SensReboterOne;
	PhysBody* SensReboterTwo;
	PhysBody* SensReboterThree;
	PhysBody* SensReboterFour;
	bool Sens_ReboterOne = false;
	bool Sens_ReboterTwo = false;
	bool Sens_ReboterThree = false;
	bool Sens_ReboterFour = false;

	/*Arrow Sensors and Animation*/
	SDL_Rect DisableArrow;
	Animation Arrows;
	PhysBody* Arrow_Left;
	PhysBody* Arrow_Right;
	bool L_Arrow_enabled = false;
	bool R_Arrow_enabled = false;

};