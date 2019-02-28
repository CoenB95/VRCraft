#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // !_USE_MATH_DEFINES

#include <GL/glew.h>
#include <cstdio>
#include <cmath>
#include <iostream>
#include <list>
#include <vector>
#include <VrLib/Application.h>
#include <VrLib/Device.h>
#include <VrLib/gl/shader.h>
#include <VrLib/Log.h>
#include <VrLib/Texture.h>

#include "vr_craft.h"

#include "block.h"
#include "blocks.h"
#include "followcomponent.h"
#include "mob.h"
#include "model.h"
#include "raycast.h"
#include "world.h"

using vrlib::Log;
using vrlib::logger;

float lastFrameTime = 0;
float lastUpdate = 0;

vector<GameObject*> gameObjects3D;

int width, height;
GLuint terrainTextureId;
World* world;

Mob* player;
CobblestoneBlock thrownBlock;

PickResult pickedBlock(nullptr, -1);
//SelectionBlock selectionBlock(0.0f);
ObjModel* model = nullptr;

bool keys[255];

VrCraft::VrCraft() {
	clearColor = glm::vec4(0.0f, 0.5f, 0.9f, 1.0f);
}

void VrCraft::init() {
	Shaders::setupDefaultShaders();

	world = new World(vec3(4, 2, 4), vec3(16, 16, 16), vec3(1, 1, 1));
	world->loadTextures();
	world->build();

	player = new Steve(*world);

	logger << "Initialized" << Log::newline;

	gameObjects3D.push_back(world);
	gameObjects3D.push_back(player);
}

//void display()
void VrCraft::draw(const glm::mat4 &projectionMatrix, const glm::mat4 &modelViewMatrix)
{
	//glEnable(GL_CULL_FACE);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	
	for (GameObject* object : gameObjects3D)
		object->draw(projectionMatrix, modelViewMatrix);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	/*RayCast rayCast(player, chunk);
	PickResult b = rayCast.pickBlock();

	if (lastUpdate >= 5.0f)
		lastUpdate = 0.0f;
	selectionBlock = SelectionBlock(lastUpdate / 5.0f);

	pickedBlock = b;
	if (pickedBlock.block != nullptr)
		selectionBlock.position = pickedBlock.block->position;

	if (pickedBlock.block != nullptr)
	{
		selectionBlock.draw();
	}*/

	/*if (camera->type != Camera::CAMERA_TYPE_FIRST_PERSON)
	{
		glPushMatrix();
		glTranslatef(player->position.x, player->position.y, -player->position.z);
		glRotatef(-player->rotateY + 90, 0, 1, 0);
		glScalef(0.2f, 0.2f, 0.2f);
		model->draw();
		glPopMatrix();
	}*/

	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, (float)width/100.0f, (float)height/100.0f, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	thrownBlock.rotateX = 15.0f;
	thrownBlock.position = Vec3f(0.5f, 0.5f, 0.0f);
	thrownBlock.draw();

	glutSwapBuffers();*/
}

//void idle()
void VrCraft::preFrame(double frameTime, double totalTime)
{
	//float frameTime = glutGet(GLUT_ELAPSED_TIME)/1000.0f;
	//float deltaTime = frameTime - lastFrameTime;
	//lastFrameTime = frameTime;
	//lastUpdate += deltaTime;
	float deltaTime = (float)(frameTime / 1000.0);
	lastUpdate += deltaTime;

	const float speed = 4.0f;
	if (keys['a']) player->move(270, deltaTime*speed, deltaTime);
	if (keys['d']) player->move(90, deltaTime*speed, deltaTime);
	if (keys['w']) player->move(0, deltaTime*speed, deltaTime);
	if (keys['s']) player->move(180, deltaTime*speed, deltaTime);
	if (keys[' ']) player->jump();

	/*Stack* mergedStack = chunk.mergeStacks();
	if (mergedStack != nullptr)
		gameObjects3D.erase(find(gameObjects3D.begin(), gameObjects3D.end(), mergedStack));*/

	for (GameObject* object : gameObjects3D)
		object->update(deltaTime);

	thrownBlock.update(deltaTime);

	/*Stack* nearby = nullptr;
	nearby = chunk.getNearbyStack(player->position);
	if (nearby != nullptr)
	{
		cout << "Picked up " << nearby->getStackSize() << " item(s) of type '" << nearby->getType()->toString() << "'" << endl;
		chunk.destroyStack(nearby);
		gameObjects3D.erase(find(gameObjects3D.begin(), gameObjects3D.end(), nearby));
	}*/

	//glutPostRedisplay();
}

bool justMovedMouse = false;
void mousePassiveMotion(int x, int y)
{
	/*int dx = x - width / 2;
	int dy = y - height / 2;
	if ((dx != 0 || dy != 0) && abs(dx) < 400 && abs(dy) < 400 && !justMovedMouse)
	{
		player->rotateY += dx * 0.3f;
		player->rotateX += dy * 0.3f;
		if (player->rotateX < -89.9f)
			player->rotateX = -89.9f;
		else if (player->rotateX > 89.9f)
			player->rotateX = 89.9f;
	}
	if (!justMovedMouse)
	{
		glutWarpPointer(width / 2, height / 2);
		justMovedMouse = true;
	}
	else
		justMovedMouse = false;*/
}

void onMousePressed(int button, int state, int x, int y)
{
	/*if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		if (pickedBlock.block != nullptr)
		{
			Block::BlockContext context = chunk.getAdjacentBlocks(pickedBlock.block);
			Block* airBlock = context[pickedBlock.side];
			if (airBlock != nullptr)
			{
				Block* b = new CobblestoneBlock();
				b->position = airBlock->position;
				chunk.notifyBlockChanged(b);
			}
		}
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (pickedBlock.block != nullptr)
		{	
			Stack* dropped = nullptr;
			dropped = chunk.destroyBlock(pickedBlock.block);
			if (dropped != nullptr)
				gameObjects3D.push_back(dropped);
		}
	}*/
}

void keyboard(unsigned char key, int, int)
{
	if (key == 27)
		exit(0);

	keys[key] = true;
}

void keyboardSpecial(int keyCode, int, int)
{
	//if (keyCode == GLUT_KEY_F5)
	//	camera->toggleType();
}

void keyboardUp(unsigned char key, int,int)
{
	keys[key] = false;
}

/*int main(int argc, char* argv[])
{
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInit(&argc, argv);
	glutCreateWindow("Hello World");

	memset(keys, 0, sizeof(keys));
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glutIdleFunc(idle);
	glutDisplayFunc(display);
	glutReshapeFunc([](int w, int h) { width = w; height = h; glViewport(0, 0, w, h); });
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutMouseFunc(onMousePressed);
	glutSpecialFunc(keyboardSpecial);
	glutPassiveMotionFunc(mousePassiveMotion);

	glutWarpPointer(width / 2, height / 2);

	camera = new Camera();
	camera->addComponent(FollowComponent::rotatingAndTranslating(&player->eyes, 0.4f));

	model = new ObjModel("models/steve/steve.obj");

	thrownBlock.setScale(Block::SCALE_ITEM);
	thrownBlock.addComponent(new SpinComponent(50.0f));

	gameObjects3D.push_back(&chunk);
	gameObjects3D.push_back(player);
	//gameObjects3D.push_back(&thrownBlock);
	gameObjects3D.push_back(camera);

	ChunkDrawComponent::loadTextures();

	cout << "Spawning Steve..." << endl;
	Block* spawnBlock;
	Block* firstBlock = chunk.getBlock(0);
	Block* secondBlock;
	int i = 0;

	while (firstBlock != nullptr)
	{
		i++;
		firstBlock = chunk.getBlock(i / chunk.height, i, i / (chunk.height * chunk.width));
		cout << "Try at position " << firstBlock->toString() << endl;

		if (firstBlock != nullptr && !firstBlock->isTransparent)
		{
			cout << "  No air." << endl;
			continue;
		}

		Block::BlockContext context = chunk.getAdjacentBlocks(firstBlock);
		spawnBlock = context.bottom;
		if (spawnBlock == nullptr || spawnBlock->isTransparent)
		{
			cout << "  No solid spawnblock." << endl;
			break;
		}

		secondBlock = chunk.getAdjacentBlocks(firstBlock).top;
		if (firstBlock != nullptr && !firstBlock->isTransparent)
		{
			cout << "  Not enough air." << endl;
			continue;
		}

		cout << "  Found space!" << endl;
		player->position.x = spawnBlock->position.x;
		player->position.y = spawnBlock->position.y + 1.0f;
		player->position.z = spawnBlock->position.z;
		break;
	}

	thrownBlock.position = player->position;

	glutMainLoop();


	return 0;
}*/