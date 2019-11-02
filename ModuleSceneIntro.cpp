#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"
ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = NULL;
	textthrower = background = NULL;
	ray_on = false;
	sensed = false;

}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	left_t = App->textures->Load("sprites/pala_izq.png");
	right_t = App->textures->Load("sprites/pala_der.png");
	
	//Background
	background = App->textures->Load("sprites/background.png");

	//Thrower
	thrower = App->physics->CreateRectangle(555, 951, 35, 100);
	textthrower = App->textures->Load("sprites/thrower.png");
	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);

	//Monkey planks
	int monkey_planks[54] = {
	150, 705,
	158, 708,
	164, 715,
	168, 727,
	174, 748,
	185, 778,
	195, 811,
	202, 832,
	203, 838,
	202, 844,
	195, 850,
	184, 851,
	177, 848,
	169, 844,
	162, 839,
	155, 834,
	150, 831,
	140, 823,
	134, 816,
	132, 811,
	132, 781,
	132, 757,
	132, 736,
	132, 721,
	134, 712,
	137, 708,
	142, 706
	};
	App->physics->CreateChain(0, 0, monkey_planks, 54, false);
	//Panda planks
	int panda_planks[54] = {
	395, 800,
	400, 786,
	408, 765,
	414, 745,
	418, 729,
	422, 718,
	427, 709,
	432, 705,
	441, 704,
	446, 706,
	452, 712,
	454, 721,
	454, 738,
	454, 759,
	454, 781,
	454, 798,
	454, 807,
	452, 815,
	439, 825,
	426, 834,
	409, 845,
	396, 847,
	388, 841,
	386, 836,
	387, 827,
	391, 814,
	393, 806
	};
	App->physics->CreateChain(0, 0, panda_planks, 54, false);

	//hockey_left
	int hoc_left[28] = {
	78, 678,
	83, 675,
	88, 678,
	89, 694,
	89, 816,
	90, 830,
	177, 897,
	175, 906,
	163, 920,
	153, 917,
	81, 866,
	78, 859,
	75, 842,
	76, 688
	};

	App->physics->CreateChain(0, 0, hoc_left, 28, false);
	//hockey_right
	int hoc_right[28] = {
	492, 677,
	502, 678,
	505, 687,
	503, 853,
	502, 861,
	495, 868,
	426, 919,
	416, 921,
	410, 919,
	399, 904,
	400, 896,
	488, 833,
	491, 809,
	491, 690
	};

	App->physics->CreateChain(0, 0, hoc_right, 28, false);

	//right side
	int right_side[46] = {
	-2, 124,
	34, 123,
	108, 124,
	111, 146,
	112, 175,
	46, 256,
	37, 262,
	37, 288,
	36, 310,
	63, 435,
	114, 483,
	114, 494,
	117, 503,
	89, 558,
	88, 622,
	68, 633,
	52, 638,
	36, 673,
	36, 882,
	218, 1004,
	218, 1050,
	0, 1050,
	0, 146
	};
	App->physics->CreateChain(0, 0, right_side, 46, false);

	//left_side
	int left_side[40] = {
	480, 208,
	488, 202,
	551, 276,
	552, 290,
	553, 1052,
	356, 1050,
	356, 1001,
	540, 880,
	541, 673,
	535, 669,
	522, 643,
	492, 625,
	490, 576,
	455, 501,
	459, 483,
	510, 436,
	536, 306,
	543, 304,
	541, 279,
	486, 218
	};
	App->physics->CreateChain(0, 0, left_side, 40, false);
	//rest
	int rest[28] = {
	591, 1047,
	592, 305,
	583, 264,
	567, 221,
	543, 186,
	511, 158,
	456, 132,
	388, 126,
	301, 124,
	3, 120,
	-1, 1,
	615, -2,
	620, 1048,
	606, 1050
	};
	App->physics->CreateChain(0, 0, rest, 28, false);
	// sticks
	int fs_stick[14] = {
	190, 213,
	195, 215,
	195, 298,
	189, 303,
	182, 298,
	182, 224,
	182, 215
	};
	App->physics->CreateChain(0, 0, fs_stick, 14, false);
	App->physics->CreateChain(48, 0, fs_stick, 14, false);
	App->physics->CreateChain(99, 0, fs_stick, 14, false);
	App->physics->CreateChain(147, 0, fs_stick, 14, false);
	App->physics->CreateChain(198, 0, fs_stick, 14, false);



	////RIGHT

	right = App->physics->CreateRectangle(149, 376, 26, 7);



	point_right = App->physics->CreateCircle(149, 376, 2);
	point_right->body->SetType(b2_staticBody);


	revoluteJointDef_right.bodyA = right->body;
	revoluteJointDef_right.bodyB = point_right->body;
	revoluteJointDef_right.referenceAngle = 0 * DEGTORAD;
	revoluteJointDef_right.enableLimit = true;
	revoluteJointDef_right.lowerAngle = -30 * DEGTORAD;
	revoluteJointDef_right.upperAngle = 30 * DEGTORAD;
	revoluteJointDef_right.localAnchorA.Set(PIXEL_TO_METERS(13), 0);
	revoluteJointDef_right.localAnchorB.Set(0, 0);
	b2RevoluteJoint* joint_right = (b2RevoluteJoint*)App->physics->world->CreateJoint(&revoluteJointDef_right);
		
		


	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	App->textures->Unload(background);
	App->textures->Unload(textthrower);
	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	App->renderer->Blit(background, 0, 0, NULL);

	//ball
	if (App->player->ball != nullptr)
	{
		int x, y;
		App->player->ball->GetPosition(x, y);
		App->renderer->Blit(App->player->textball, x, y);
	}
	//thrower
	if (App->player->thrower == true)
	{
		int x, y;
		App->scene_intro->thrower->GetPosition(x, y);
		App->renderer->Blit(App->scene_intro->textthrower, x, y + 2 - (int)App->player->vely * 1.5f);
	}
	else
	{
		int x, y;
		thrower->GetPosition(x, y);
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_IDLE)
			App->renderer->Blit(textthrower, x, y + 2);
	}


	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 14));
		App->renderer->Blit(circle, App->input->GetMouseX(), App->input->GetMouseY());
		//circles.getLast()->data->listener = this;
	}


	//RIGHT TRIGGER
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		b2Vec2 force = b2Vec2(0, -200);
		right->body->ApplyForceToCenter(force, 1);
		revoluteJointDef_right.lowerAngle = 30 * DEGTORAD;
	}


	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();







	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	if (bodyB == thrower)
		ballthrow = true;
	else
		ballthrow = false;

	/*
	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}*/
}
