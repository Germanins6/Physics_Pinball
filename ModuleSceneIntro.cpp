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
	
	//Setting Pointers to SDL_Tex in nullptr
	circle = textthrower = background = lifes = paddlestex = dynElements = nullptr;
	ray_on = false;
	sensed = false;

}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{

	
	//Loading all asset textures
	LoadTextures();

	//Draw all vectors owned by the background
	SetBackgroundColliders();
	Sensors();
	 
	//Set R/L Paddles in Scene
	SetPaddles();

	//Set Thrower in Scene
	SetThrower();
	//SetReboters
	SetReboters();
		
	return true;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading all assets");
	App->textures->Unload(paddlestex);
	App->textures->Unload(lifes);
	App->textures->Unload(background);
	App->textures->Unload(textthrower);
	
	App->textures->Unload(dynElements);
	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	App->renderer->Blit(background, 0, 0, NULL);
	App->renderer->Blit(dynElements, 142, 433, &DisableArrow, 1.0f, -20);
	App->renderer->Blit(dynElements, 390, 433, &DisableArrow, 1.0f, 20);

	

	CheckInteractions();
	DrawLifes();

	LOG("X: %d and Y: %d", App->input->GetMouseX(), App->input->GetMouseY());





	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	if (bodyB == thrower)
		ballthrow = true;
	else
		ballthrow = false;

}

//Must be called in start
void ModuleSceneIntro::SetBackgroundColliders() {

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


	//Static Circles in middle
	CircleOne = App->physics->CreateCircle(211, 415, 34, false);
	CircleTwo = App->physics->CreateCircle(359, 415, 34, false);
	CircleThree = App->physics->CreateCircle(286, 510, 34, false);

}

void ModuleSceneIntro::SetPaddles() {
	
	//Right Paddle

	right = App->physics->CreateRectangle(300, 920, 94, 14);
	point_right = App->physics->CreateCircle(392, 920, 8);
	point_right->body->SetType(b2_staticBody);

	
	revoluteJointDef_right.bodyA = right->body;
	revoluteJointDef_right.bodyB = point_right->body;
	revoluteJointDef_right.referenceAngle = 0 * DEGTORAD;
	revoluteJointDef_right.enableLimit = true;
	revoluteJointDef_right.lowerAngle = -30 * DEGTORAD;
	revoluteJointDef_right.upperAngle = 30 * DEGTORAD;
	revoluteJointDef_right.localAnchorA.Set(PIXEL_TO_METERS(47), 0);
	revoluteJointDef_right.localAnchorB.Set(0, 0);
	b2RevoluteJoint* joint_right = (b2RevoluteJoint*)App->physics->world->CreateJoint(&revoluteJointDef_right);

	//Left Paddle

	left = App->physics->CreateRectangle(184, 920, 94, 14);
	point_left = App->physics->CreateCircle(184,920, 8);
	point_left->body->SetType(b2_staticBody);

	revoluteJointDef_left.bodyA = left->body;
	revoluteJointDef_left.bodyB = point_left->body;
	revoluteJointDef_left.referenceAngle = 0 * DEGTORAD;
	revoluteJointDef_left.enableLimit = true;
	revoluteJointDef_left.lowerAngle = -30 * DEGTORAD;
	revoluteJointDef_left.upperAngle = 30 * DEGTORAD;
	revoluteJointDef_left.localAnchorA.Set(PIXEL_TO_METERS(-47), 0);
	revoluteJointDef_left.localAnchorB.Set(0, 0);
	b2RevoluteJoint* joint_left = (b2RevoluteJoint*)App->physics->world->CreateJoint(&revoluteJointDef_left);

}

void ModuleSceneIntro::SetThrower() {

	//Thrower
	thrower = App->physics->CreateRectangle(572, 1009, 32, 40);
	staticThrower = App->physics->CreateRectangle(572, 1009, 32, 40);

	staticThrower->body->SetType(b2_staticBody);

	prismaticJoint_launcher.bodyB = thrower->body;
	prismaticJoint_launcher.bodyA = staticThrower->body;
	prismaticJoint_launcher.collideConnected = false;
	prismaticJoint_launcher.enableLimit = true;
	prismaticJoint_launcher.lowerTranslation = PIXEL_TO_METERS(25);
	prismaticJoint_launcher.upperTranslation = PIXEL_TO_METERS(60);
	prismaticJoint_launcher.localAnchorA.Set(0, 0);
	prismaticJoint_launcher.localAnchorB.Set(0, 0);
	prismaticJoint_launcher.localAxisA.Set(0, -1);

	b2PrismaticJoint* joint_launcher = (b2PrismaticJoint*)App->physics->world->CreateJoint(&prismaticJoint_launcher);
}
void ModuleSceneIntro::SetReboters() {
	//Reboter One
	ReboterOne = App->physics->CreateRectangle(90,453,54,14,39,0.99f);
	ReboterOne->body->SetType(b2_staticBody);
	//Reboter Two
	ReboterTwo = App->physics->CreateRectangle(483, 453, 54, 14, -39, 0.99f);
	ReboterTwo->body->SetType(b2_staticBody);
	//Reboter Three
	ReboterThree = App->physics->CreateRectangle(189, 773, 89, 17, 62, 0.99f);
	ReboterThree->body->SetType(b2_staticBody);
	//Reboter Four
	ReboterFour = App->physics->CreateRectangle(397, 773, 89, 17, -62, 0.99f);
	ReboterFour->body->SetType(b2_staticBody);

}

void ModuleSceneIntro::PlayerInputs() {
	
	//Right Paddle
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		b2Vec2 force = b2Vec2(0, -350);
		right->body->ApplyForceToCenter(force, 1);
		revoluteJointDef_right.lowerAngle = 30 * DEGTORAD;
	}

	if (right != NULL)
	{
		int x, y;
		right->GetPosition(x, y);
		App->renderer->Blit(paddlestex, x, y, &paddle_right, 1.0f, right->GetRotation()+5);
	}

	//Left Paddle
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		b2Vec2 force = b2Vec2(0, -350);
		left->body->ApplyForceToCenter(force, 1);
		revoluteJointDef_left.lowerAngle = 30 * DEGTORAD;
	}

	if (left != NULL)
	{
		int x, y;
		left->GetPosition(x, y);
		App->renderer->Blit(paddlestex, x, y, &paddle_left, 1.0f, left->GetRotation()-5);
	}

	//Thrower
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
		ThrowForce += 50;
		if (ThrowForce > 1200) ThrowForce = 1200;
		LOG("Force will be %i", ThrowForce);
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP) {
		thrower->body->ApplyForceToCenter(b2Vec2(0, -ThrowForce), 1);
	}

	if (thrower != NULL)
	{
		int x, y;
		thrower->GetPosition(x, y);
		App->renderer->Blit(textthrower, x-2, y, NULL, 1.0f);
	}

}


void ModuleSceneIntro::DrawLifes() {

	//Blit Score
	App->renderer->Blit(lifes, 10, 10, &score);

	//Blit Lifes
	if (App->player->lifes == 3) App->renderer->Blit(lifes, 10,74, &all_lifes);
	if (App->player->lifes == 2) App->renderer->Blit(lifes, 10, 74, &twolifes);
	if (App->player->lifes == 1) App->renderer->Blit(lifes, 10, 74, &onelife);
	if (App->player->lifes <= 0) App->renderer->Blit(lifes, 10, 74, &nolife);

}

//SHOULD BE CALLED ONLY IN START--->>>>> REMEMBER UNLOAD ALL THIS TEX!!!
void ModuleSceneIntro::LoadTextures() {

	LOG("Loading Intro assets");

	//Loading paddles tex
	paddlestex = App->textures->Load("sprites/paddles.png");
	{
		paddle_left.x = 7;
		paddle_left.y = 58;
		paddle_left.w = 100;
		paddle_left.h = 34;

		paddle_right.x = 7;
		paddle_right.y = 12;
		paddle_right.w = 100;
		paddle_right.h = 34;
	}

	//Background
	background = App->textures->Load("sprites/background.png");

	//Thrower
	textthrower = App->textures->Load("sprites/thrower.png");

	//Lifes and Score
	lifes = App->textures->Load("sprites/lifes.png");
	{
		nolife.x = 184;
		nolife.y = 137;
		nolife.w = 125;
		nolife.h = 50;

		onelife.x = 184;
		onelife.y = 88;
		onelife.w = 125;
		onelife.h = 50;

		twolifes.x = 21;
		twolifes.y = 137;
		twolifes.w = 125;
		twolifes.h = 50;

		all_lifes.x = 21;
		all_lifes.y = 88;
		all_lifes.w = 125;
		all_lifes.h = 50;

		score.x = 8;
		score.y = 8;
		score.w = 326;
		score.h = 68;
	}

	//Interactive Elements
	dynElements = App->textures->Load("sprites/interactive_elements.png");
	{
		/*Green Sticks Top*/
		GreenStick.x = 42;
		GreenStick.y = 173;
		GreenStick.w = 15;
		GreenStick.h = 41;


	}

	//Arrows
	DisableArrow.x = 25;
	DisableArrow.y = 689;
	DisableArrow.w = 55;
	DisableArrow.h = 130;

	{
		Arrows.PushBack({ 25,689,55,130 });
		Arrows.PushBack({ 83,689,55,130 });
		Arrows.PushBack({ 142,689,55,130 });
		Arrows.PushBack({ 199,689,55,130 });
		Arrows.PushBack({ 296,689,55,130 });
		Arrows.PushBack({ 354,689,55,130 });
		Arrows.PushBack({ 413,689,55,130 });
		Arrows.PushBack({ 481,689,55,130 });

		Arrows.speed = 0.1f;
	}
	



}

void ModuleSceneIntro::CheckInteractions() {
	/*Top Green Sensors*/
	if (Sens_GreenOne) App->renderer->Blit(dynElements, 203, 239, &GreenStick);
	if (Sens_GreenTwo) App->renderer->Blit(dynElements, 253, 239, &GreenStick);
	if (Sens_GreenThree) App->renderer->Blit(dynElements, 300, 239, &GreenStick);
	if (Sens_GreenFour) App->renderer->Blit(dynElements, 352, 239, &GreenStick);

	//Checks if 4 sensors are true
	if (Sens_GreenOne && Sens_GreenTwo && Sens_GreenThree && Sens_GreenFour) {
		App->player->puntos += 5000;
		Sens_GreenOne = Sens_GreenTwo = Sens_GreenThree = Sens_GreenFour = false;
	}

	if(L_Arrow_enabled) App->renderer->Blit(App->scene_intro->dynElements, 145, 433, &App->scene_intro->Arrows.GetCurrentFrame(), 1.0f, -20);
	if(R_Arrow_enabled) App->renderer->Blit(App->scene_intro->dynElements, 390, 433, &App->scene_intro->Arrows.GetCurrentFrame(), 1.0f, 20);



}

void ModuleSceneIntro::Sensors() {

	/*Middle Circles*/
	SensCircleOne = App->physics->CreateSensor(211, 415,NULL,NULL, true, 40);
	SensCircleTwo = App->physics->CreateSensor(359, 415, NULL, NULL, true, 40);
	SensCircleThree = App->physics->CreateSensor(286, 510, NULL,NULL ,true, 40);

	/*Top Green sensors*/
	GreenSensorOne = App->physics->CreateSensor(213, 260, 17, 42);
	GreenSensorTwo = App->physics->CreateSensor(263, 260, 17, 42);
	GreenSensorThree = App->physics->CreateSensor(310, 260, 17, 42);
	GreenSensorFour = App->physics->CreateSensor(362, 260, 17, 42);
	/*Reboters sensors*/

	SensReboterOne = App->physics->CreateSensor(90, 453, 65, 25,false,NULL, 39);
    SensReboterTwo=App->physics->CreateSensor(483, 453,65, 25, false, NULL, -39);
	SensReboterThree= App->physics->CreateSensor(189, 773, 100, 28,false,NULL, 62);
    SensReboterFour= App->physics->CreateSensor(397, 773,100, 28, false, NULL, -62);


	/*Lose Sensor*/
	LostBallSensor = App->physics->CreateSensor(284, 1025, 110 , 20);

	/*Arrow Sensors*/
	Arrow_Left = App->physics->CreateSensor(142, 433, 10, 10, false, 0 , -20);
	Arrow_Right = App->physics->CreateSensor(442, 433, 10, 10, false, 0 , 20);
}
