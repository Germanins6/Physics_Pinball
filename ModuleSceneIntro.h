#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "ModulePlayer.h"
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

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;
	
	PhysBody* sensor;
	bool sensed;

	SDL_Texture* circle;
	SDL_Texture* right_t;
	SDL_Texture* left_t;
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

	
	PhysBody* left;
	PhysBody* right;
	PhysBody* point_right;
	PhysBody* point_left;
	b2RevoluteJointDef revoluteJointDef_right;
	b2RevoluteJointDef revoluteJointDef_left;


};
