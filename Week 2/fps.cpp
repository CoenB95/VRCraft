#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // !_USE_MATH_DEFINES

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif // !STB_IMAGE_IMPLEMENTATION

#include <GL/freeglut.h>
#include <cstdio>
#include <cmath>
#include <iostream>
#include <vector>

#include "block.h"
#include "block_stone.h"
#include "camera.h"
#include "chunk.h"
#include "mob.h"
#include "model.h"
#include "raycast.h"
#include "stb_image.h"

float lastFrameTime = 0;
float lastUpdate = 0;

int width, height;
GLuint terrainTextureId;
Chunk chunk(50, 20, 50);

Camera* camera = new Camera();
Mob* player = new Steve(chunk);
PickResult pickedBlock(nullptr, -1);
SelectionBlock selectionBlock(0.0f);
ObjModel* model = nullptr;

bool keys[255];


void drawCube()
{
	glBegin(GL_QUADS);
	glColor3f(1, 0, 0);
	glVertex3f(-1, -1, -1);
	glVertex3f(1, -1, -1);
	glVertex3f(1, 1, -1);
	glVertex3f(-1, 1, -1);

	glColor3f(1, 1, 0);
	glVertex3f(-1, -1, 1);
	glVertex3f(1, -1, 1);
	glVertex3f(1, 1, 1);
	glVertex3f(-1, 1, 1);

	glColor3f(0, 0, 1);
	glVertex3f(-1, -1, -1);
	glVertex3f(-1, 1, -1);
	glVertex3f(-1, 1, 1);
	glVertex3f(-1, -1, 1);

	glColor3f(1, -1, 1);
	glVertex3f(1, -1, -1);
	glVertex3f(1, 1, -1);
	glVertex3f(1, 1, 1);
	glVertex3f(1, -1, 1);

	glColor3f(0, 1, 0);
	glVertex3f(-1, -1, -1);
	glVertex3f(1, -1, -1);
	glVertex3f(1, -1, 1);
	glVertex3f(-1, -1, 1);

	glColor3f(1, 1, 0);
	glVertex3f(-1, 1, -1);
	glVertex3f(1, 1, -1);
	glVertex3f(1, 1, 1);
	glVertex3f(-1, 1, 1);
	glEnd();
}

void display()
{
	glClearColor(0.6f, 0.6f, 1, 1);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (float)width/height, 0.1, 30);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera->applyTransform();

	chunk.update();

	RayCast rayCast(player, chunk);
	PickResult b = rayCast.pickBlock();

	//if (pickedBlock.block != nullptr)
		//pickedBlock.block->mark = false;
	//if (b.block != nullptr && !b.block->isTransparent)
		//b.block->mark = true;

	if (lastUpdate >= 5.0f)
		lastUpdate = 0.0f;
	selectionBlock = SelectionBlock(lastUpdate / 5.0f);

	pickedBlock = b;
	if (pickedBlock.block != nullptr)
		selectionBlock.pos.set(pickedBlock.block->pos);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, terrainTextureId);
	chunk.draw();

	if (pickedBlock.block != nullptr)
	{
		selectionBlock.draw();
	}

	glPushMatrix();
	glTranslatef(player->position.x, player->position.y, -player->position.z );
	glRotatef(-player->rotateY + 90, 0, 1, 0);
	glScalef(0.2f, 0.2f, 0.2f);
	model->draw();
	glPopMatrix();

	glutSwapBuffers();
}

void idle()
{
	float frameTime = glutGet(GLUT_ELAPSED_TIME)/1000.0f;
	float deltaTime = frameTime - lastFrameTime;
	lastFrameTime = frameTime;
	lastUpdate += deltaTime;

	const float speed = 4.0f;
	if (keys['a']) player->move(270, deltaTime*speed, deltaTime);
	if (keys['d']) player->move(90, deltaTime*speed, deltaTime);
	if (keys['w']) player->move(0, deltaTime*speed, deltaTime);
	if (keys['s']) player->move(180, deltaTime*speed, deltaTime);
	if (keys[' '] && player->isFloored()) player->verticalSpeed = 8.0f;

	player->update(deltaTime);
	camera->position = player->eyePosition;

	glutPostRedisplay();
}

bool justMovedMouse = false;
void mousePassiveMotion(int x, int y)
{
	int dx = x - width / 2;
	int dy = y - height / 2;
	if ((dx != 0 || dy != 0) && abs(dx) < 400 && abs(dy) < 400 && !justMovedMouse)
	{
		camera->rotateY += dx * 0.3f;
		camera->rotateX += dy * 0.3f;
		if (camera->rotateX < -90)
			camera->rotateX = -90;
		else if (camera->rotateX > 90)
			camera->rotateX = 90;
	}
	if (!justMovedMouse)
	{
		glutWarpPointer(width / 2, height / 2);
		justMovedMouse = true;
	}
	else
		justMovedMouse = false;
}

void onMousePressed(int button, int state, int x, int y)
{
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		if (pickedBlock.block != nullptr)
		{
			Block::BlockContext context = chunk.getAdjacentBlocks(pickedBlock.block);
			Block* airBlock = context[pickedBlock.side];
			if (airBlock != nullptr)
			{
				Block* b = new CobblestoneBlock();
				b->pos.set(airBlock->pos);
				chunk.notifyBlockChanged(b);
			}
		}
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (pickedBlock.block != nullptr)
		{
			Block* b = new StoneBlock();
			b->isTransparent = true;
			b->pos.set(pickedBlock.block->pos);
			chunk.notifyBlockChanged(b);
		}
	}
}

void keyboard(unsigned char key, int, int)
{
	if (key == 27)
		exit(0);

	keys[key] = true;
}

void keyboardSpecial(int keyCode, int, int)
{
	if (keyCode == GLUT_KEY_F5)
		camera->toggleType();
}

void keyboardUp(unsigned char key, int,int)
{
	keys[key] = false;
}

int main(int argc, char* argv[])
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

	model = new ObjModel("models/steve/steve.obj");

	cout << "Loading textures... " << endl;

	int imageWidth, imageHeight, imageComponents;
	stbi_uc* image = stbi_load("terrain.png", &imageWidth, &imageHeight, &imageComponents, 0);

	if (image == nullptr)
	{
		cout << "Could not load textures" << endl << "  Reason: " << stbi_failure_reason() << endl;
	}
	else
	{
		cout << "Image size: " << imageWidth << "x" << imageHeight << endl;

		glGenTextures(1, &terrainTextureId);
		glBindTexture(GL_TEXTURE_2D, terrainTextureId);

		glTexImage2D(GL_TEXTURE_2D,
			0,					//level
			GL_RGBA,			//internal format
			imageWidth,			//width
			imageHeight,		//height
			0,					//border
			GL_RGBA,			//data format
			GL_UNSIGNED_BYTE,	//data type
			image);				//data

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glEnable(GL_TEXTURE_2D);

		stbi_image_free(image);
	}

	cout << "Loading textures done" << endl;

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
		player->position.x = spawnBlock->pos.x;
		player->position.y = spawnBlock->pos.y + 0.5f;
		player->position.z = spawnBlock->pos.z;
		break;
	}

	glutMainLoop();


	return 0;
}