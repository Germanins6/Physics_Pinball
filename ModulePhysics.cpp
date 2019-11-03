#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "ModuleSceneIntro.h"
#include "ModuleAudio.h"

#include "p2Point.h"
#include "math.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	mouse_joint = NULL;
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	// needed to create joints like mouse joint
	b2BodyDef bd;
	ground = world->CreateBody(&bd);
	

	return true;
}

update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	for(b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if(c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			if(pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}

	return UPDATE_CONTINUE;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius, bool dynamic, float restitution)
{
	b2BodyDef body;
	if (dynamic == false)
		body.type = b2_staticBody;
	else if (dynamic == true)
		body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.restitution = restitution;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height, int angle, float restitution)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.angle = PIXEL_TO_METERS(angle);
	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.restitution = restitution;
	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

PhysBody* ModulePhysics::CreateSensor(int x, int y, int width, int height, bool circle, int radius, int angle)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.angle = PIXEL_TO_METERS(angle);
	b2Body* b = world->CreateBody(&body);

	if (!circle) {
		b2PolygonShape box;
		box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);
		b2FixtureDef fixture;
		fixture.shape = &box;
		fixture.density = 1.0f;
		fixture.isSensor = true;
		b->CreateFixture(&fixture);

	}
	else if (circle) {
		b2CircleShape shape;
		shape.m_radius = PIXEL_TO_METERS(radius);
		b2FixtureDef fixture;
		fixture.shape = &shape;
		fixture.density = 1.0f;
		fixture.isSensor = true;
		
		b->CreateFixture(&fixture);

	}

	
	
	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;

	return pbody;
}

PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size, bool dynamic)
{
	b2BodyDef body;
	if (dynamic == false)
		body.type = b2_staticBody;
	else if (dynamic == true)
		body.type = b2_dynamicBody;

	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for(uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

update_status ModulePhysics::PostUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(debug)
		return UPDATE_CONTINUE;

	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch(f->GetType())
			{
				// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for(int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for(int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if(i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
			}

			// TODO 1: If mouse button 1 is pressed ...
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
			{
				b2Vec2 mousepos{ PIXEL_TO_METERS(App->input->GetMouseX()),PIXEL_TO_METERS(App->input->GetMouseY()) };
					if (f->TestPoint(mousepos))
					{
						b2MouseJointDef def;
						def.bodyA = ground;
						def.bodyB = b;
						def.target = mousepos;
						def.dampingRatio = 0.5f;
						def.frequencyHz = 2.0f;
						def.maxForce = 100.0f * b->GetMass();
						mouse_joint = (b2MouseJoint*)world->CreateJoint(&def);
					}


			}
		}
	}


	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && mouse_joint != nullptr)
	{
		mouse_joint->SetTarget({ PIXEL_TO_METERS(App->input->GetMouseX()),PIXEL_TO_METERS(App->input->GetMouseY()) });
		b2Vec2 A;
		b2Vec2 B;
		A.x = METERS_TO_PIXELS(mouse_joint->GetAnchorA().x);
		A.y = METERS_TO_PIXELS(mouse_joint->GetAnchorA().y);
		B.x = METERS_TO_PIXELS(mouse_joint->GetAnchorB().x);
		B.y = METERS_TO_PIXELS(mouse_joint->GetAnchorB().y);
		App->renderer->DrawLine(A.x,A.y,B.x,B.y,255,0,0);
	}
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && mouse_joint != nullptr)
	{
		world->DestroyJoint(mouse_joint);
		mouse_joint = nullptr;
	}


	return UPDATE_CONTINUE;
}

bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

void PhysBody::GetPosition(int& x, int &y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = x2 - x1;
			float fy = y2 - y1;
			float dist = sqrtf((fx*fx) + (fy*fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return output.fraction * dist;
		}
		fixture = fixture->GetNext();
	}

	return ret;
}

void ModulePhysics::BeginContact(b2Contact* contact)
{
	float32 angle;
	angle = App->player->ball->body->GetAngle();
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if(physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if(physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);

	
	//Sensors

	/*Middle Circle Sensors*/
	if (physA == App->scene_intro->SensCircleOne) {
		App->player->CurrentScore += 10;
		App->scene_intro->EnableCircleOne = true;
	}

	if (physA == App->scene_intro->SensCircleTwo) {
		App->player->CurrentScore += 10;
		App->scene_intro->EnableCircleTwo = true;
	}

	if (physA == App->scene_intro->SensCircleThree) {
		App->player->CurrentScore += 10;
		App->scene_intro->EnableCircleThree = true;
	}

	/*fsSticks*/
	if (physA == App->scene_intro->GreenSensorOne) {
		App->audio->PlayFx(App->scene_intro->greensensor);
		App->scene_intro->Sens_GreenOne = true;
	}

	if (physA == App->scene_intro->GreenSensorTwo) {
		App->audio->PlayFx(App->scene_intro->greensensor);
		App->scene_intro->Sens_GreenTwo = true;
	}

	if (physA == App->scene_intro->GreenSensorThree) {
		App->audio->PlayFx(App->scene_intro->greensensor);
		App->scene_intro->Sens_GreenThree = true;
	}

	if (physA == App->scene_intro->GreenSensorFour) {
		App->audio->PlayFx(App->scene_intro->greensensor);
		App->scene_intro->Sens_GreenFour = true;
	}
	
	/*Reboters*/
	if (physA == App->scene_intro->SensReboterOne) {
		App->player->ball->body->SetLinearVelocity({ 15,3});
		App->scene_intro->Sens_ReboterOne = TRUE;
	}

	if (physA == App->scene_intro->SensReboterTwo) {
		App->player->ball->body->SetLinearVelocity({ 15,3 });
		App->scene_intro->Sens_ReboterTwo = TRUE;
	}

	if (physA == App->scene_intro->SensReboterThree) {
		App->player->ball->body->SetLinearVelocity({ 15,3 });
		App->scene_intro->Sens_ReboterThree = TRUE;
	}

	if (physA == App->scene_intro->SensReboterFour) {
		App->player->ball->body->SetLinearVelocity({ 15,3 });
		App->scene_intro->Sens_ReboterFour = TRUE;
	}

	/*Lost Ball Sensor*/
	if (physA == App->scene_intro->LostBallSensor) {

		//Update Score
		App->player->TotalScore += App->player->CurrentScore;		//Stores total score in current game
		App->player->CurrentScore = 0;								//Sets score 0 

		//Less balls
		App->player->lifes -= 1;
		App->audio->PlayFx(App->scene_intro->lostBall, 0);

		//Reset Arrows
		App->scene_intro->L_Arrow_enabled = false;
		App->scene_intro->R_Arrow_enabled = false;
		
		//Reset Green Sensors
		App->scene_intro->Sens_GreenOne = false;
		App->scene_intro->Sens_GreenTwo = false;
		App->scene_intro->Sens_GreenThree = false;
		App->scene_intro->Sens_GreenFour = false;

		//Calls reset function in player
		App->player->reset = true;
	}

	/*Arrow Sensors*/
	if (physA == App->scene_intro->Arrow_Left) {
		App->scene_intro->L_Arrow_enabled = true;
	}

	if (physA == App->scene_intro->Arrow_Right) {
		App->scene_intro->R_Arrow_enabled = true;
	}
}