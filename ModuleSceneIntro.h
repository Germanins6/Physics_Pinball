#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "ModulePlayer.h"
#include "Animation.h"

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

	void SetBackgroundColliders();
	void SetPaddles();
	void PaddleInputs();
	void DrawLifes();
	void LoadTextures();
	void CheckInteractions();
	void Sensors();

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;
	
	PhysBody* sensor;
	bool sensed;

	SDL_Texture* circle;

	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;
	bool ballthrow;
private:
	SDL_Texture* background;
	
	PhysBody* thrower;
	PhysBody* thrower_sta;
	SDL_Texture* textthrower;
	

public:

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

	/*Middle Circles*/
	PhysBody* CircleOne;
	PhysBody* CircleTwo;
	PhysBody* CircleThree;

	/*Middle Circle Sensors*/
	PhysBody* SensCircleOne;
	PhysBody* SensCircleTwo;
	PhysBody* SensCircleThree;

	/*Top Green Sensors*/
	PhysBody* GreenSensorOne;
	PhysBody* GreenSensorTwo;
	PhysBody* GreenSensorThree;
	PhysBody* GreenSensorFour;
	bool Sens_GreenOne = false;
	bool Sens_GreenTwo = false;
	bool Sens_GreenThree = false;
	bool Sens_GreenFour = false;


};