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
#include "camera.h"
#include "chunk.h"
#include "mob.h"
#include "stb_image.h"

float lastFrameTime = 0;
float lastUpdate = 0;

int width, height;

Chunk chunk(50, 20, 50);

Mob* player = new Steve(chunk);
Block* pb = nullptr;

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

Block* lookAtTop(GLfloat diffY)
{
	if (player->getEyes().rotX <= 0)
		return nullptr;

	float r1 = (player->getMobHeight() - diffY) / tanf(player->getEyes().rotX / 180 * M_PI);
	float rayXSteve = cosf((-player->getEyes().rotY + 90) / 180 * M_PI) * r1 + player->getEyes().posX;
	float rayZSteve = sinf((-player->getEyes().rotY + 90) / 180 * M_PI) * r1 + player->getEyes().posZ;

	return chunk.getBlock(roundf(rayXSteve), player->getEyes().posY - player->getMobHeight() + diffY, roundf(rayZSteve));
}

Block* lookAtFrontSide(GLfloat diffZ)
{
	float blockZ = player->getEyes().posZ - roundf(player->getEyes().posZ);
	float realZ = (diffZ - 0.49f - blockZ) / sinf((-player->getEyes().rotY + 90) / 180 * M_PI);
	float diffY = -tanf(player->getEyes().rotX / 180 * M_PI) * realZ;
	float rayXSteve = cosf((-player->getEyes().rotY + 90) / 180 * M_PI) * realZ + player->getEyes().posX;
	float rayZSteve = sinf((-player->getEyes().rotY + 90) / 180 * M_PI) * realZ + player->getEyes().posZ;

	return chunk.getBlock(roundf(rayXSteve), roundf(player->getEyes().posY + diffY), roundf(rayZSteve));
}

Block* lookAtLeftSide(GLfloat diffX)
{
	float blockX = player->getEyes().posX - roundf(player->getEyes().posX);
	float realX = (diffX - 0.49f - blockX) / cosf((-player->getEyes().rotY + 90) / 180 * M_PI);
	float diffY = -tanf(player->getEyes().rotX / 180 * M_PI) * realX;
	float rayXSteve = cosf((-player->getEyes().rotY + 90) / 180 * M_PI) * realX + player->getEyes().posX;
	float rayZSteve = sinf((-player->getEyes().rotY + 90) / 180 * M_PI) * realX + player->getEyes().posZ;

	return chunk.getBlock(roundf(rayXSteve), roundf(player->getEyes().posY + diffY), roundf(rayZSteve));
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
	player->getEyes().applyTransform();

	chunk.update();

	/*Block* b = lookAtFrontSide(1);
	if (b == nullptr || b->isTransparent)
		b = lookAtFrontSide(2);*/

	int iT = 0;
	Block* b = nullptr;
	while (iT > -8 && (b == nullptr || b->isTransparent))
	{
		b = lookAtTop(iT);
		iT--;
	}

	int iF = 1;
	Block* bF = nullptr;
	while (iF <= 8 && (bF == nullptr || bF->isTransparent))
	{
		bF = lookAtFrontSide(iF);
		if (bF != nullptr && !bF->isTransparent)
		{
			if (b != nullptr && bF->z > b->z)
				break;
			b = bF;
		}
		iF++;
	}

	int iL = 1;
	Block* bL = nullptr;
	while (iL <= 8 && (bL == nullptr || bL->isTransparent))
	{
		bL = lookAtLeftSide(iL);
		if (bL != nullptr && !bL->isTransparent)
		{
			if (b != nullptr && bL->x > b->x)
				break;
			b = bL;
		}
		iL++;
	}

	cout << "Checked " << (-iT) << " tops, " << iL-1 << " lefts and " << iF-1 << " fronts" << endl;

	if (pb != nullptr)
		pb->mark = false;
	if (b != nullptr && !b->isTransparent)
		b->mark = true;
	pb = b;

	chunk.draw();

	glutSwapBuffers();
}

void idle()
{
	float frameTime = glutGet(GLUT_ELAPSED_TIME)/1000.0f;
	float deltaTime = frameTime - lastFrameTime;
	lastFrameTime = frameTime;
	lastUpdate += deltaTime;

	const float speed = 3;
	if (keys['a']) player->move(270, deltaTime*speed, deltaTime);
	if (keys['d']) player->move(90, deltaTime*speed, deltaTime);
	if (keys['w']) player->move(0, deltaTime*speed, deltaTime);
	if (keys['s']) player->move(180, deltaTime*speed, deltaTime);
	if (keys[' '] && player->isFloored()) player->getEyes().speedY = 8.0f;

	player->update(deltaTime);

	glutPostRedisplay();
}

bool justMovedMouse = false;
void mousePassiveMotion(int x, int y)
{
	int dx = x - width / 2;
	int dy = y - height / 2;
	if ((dx != 0 || dy != 0) && abs(dx) < 400 && abs(dy) < 400 && !justMovedMouse)
	{
		player->getEyes().rotY += dx * 0.3f;
		player->getEyes().rotX += dy * 0.3f;
		if (player->getEyes().rotX < -90)
			player->getEyes().rotX = -90;
		else if (player->getEyes().rotX > 90)
			player->getEyes().rotX = 90;
	}
	if (!justMovedMouse)
	{
		glutWarpPointer(width / 2, height / 2);
		justMovedMouse = true;
	}
	else
		justMovedMouse = false;
}

void keyboard(unsigned char key, int, int)
{
	if (key == 27)
		exit(0);
	keys[key] = true;
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

	glutIdleFunc(idle);
	glutDisplayFunc(display);
	glutReshapeFunc([](int w, int h) { width = w; height = h; glViewport(0, 0, w, h); });
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutPassiveMotionFunc(mousePassiveMotion);

	glutWarpPointer(width / 2, height / 2);

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

		GLuint textureId;

		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);

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
	Camera& camera = player->getEyes();
	camera.posX = 0.0f;
	camera.posY = 0.5f;
	camera.posZ = 0.0f;
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
		camera.posX = spawnBlock->x * chunk.blockSize;
		camera.posY = spawnBlock->y * chunk.blockSize + 5 * chunk.blockSize;
		camera.posZ = spawnBlock->z * chunk.blockSize;
		break;
	}

	glutMainLoop();


	return 0;
}