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
#include "chunk.h"
#include "stb_image.h"

float lastFrameTime = 0;
float lastUpdate = 0;

int width, height;

Chunk chunk(30, 30, 30);

struct Camera
{
	float posX = 0;
	float posY = -4;
	float posZ = 0;
	float rotX = 0;
	float rotY = 0;
} camera;

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
	glRotatef(camera.rotX, 1, 0, 0);
	glRotatef(camera.rotY, 0, 1, 0);
	glTranslatef(-camera.posX, -camera.posY, -camera.posZ);


	/*glColor3f(0.1f, 0.6f, 0.2f);
	glBegin(GL_QUADS);
		glVertex3f(-15, -1, -15);
		glVertex3f( 15, -1, -15);
		glVertex3f( 15, -1,  15);
		glVertex3f(-15, -1,  15);
	glEnd();*/

	/*for (int x = -10; x <= 10; x += 5)
	{
		for (int y = -10; y <= 10; y += 5)
		{
			glPushMatrix();
			glTranslatef((float)x, 0.0f, (float)y);
			drawCube();
			glPopMatrix();
		}
	}*/

	/*for (int i = 0; i < blocks.size(); i++)
	{
		glPushMatrix();
		blocks[i].draw();
		glPopMatrix();
	}*/

	chunk.update();
	chunk.draw();

	glutSwapBuffers();
}

bool transp(Block* b1, Block* b2)
{
	return (b1 == nullptr || b1->isTransparent) && (b2 == nullptr || b2->isTransparent);
}

void move(float angle, float fac)
{
	float chunkX = roundf(camera.posX / chunk.blockSize);
	float chunkZ = roundf(camera.posZ / chunk.blockSize);
	float blockX = camera.posX - chunkX * chunk.blockSize;
	float blockZ = camera.posZ - chunkZ * chunk.blockSize;
	float deltaX = (float)cos((-90 + camera.rotY + angle) / 360 * M_PI * 2) * fac;
	float deltaZ = (float)sin((-90 + camera.rotY + angle) / 360 * M_PI * 2) * fac;
	blockX += deltaX / chunk.blockSize;
	blockZ += deltaZ / chunk.blockSize;

	Block* curFloor = chunk.getBlock(roundf(camera.posX / chunk.blockSize),
		(camera.posY - 2 * chunk.blockSize) / chunk.blockSize,
		-roundf(camera.posZ / chunk.blockSize));

	Block::BlockContext feetContext = chunk.getAdjacentBlocks(chunk.getAdjacentBlocks(curFloor).top);
	Block::BlockContext headContext = chunk.getAdjacentBlocks(
		chunk.getAdjacentBlocks(chunk.getAdjacentBlocks(curFloor).top).top);

	Block* nextFloor = chunk.getBlock(roundf(camera.posX + deltaX / chunk.blockSize),
		(camera.posY - 2 * chunk.blockSize) / chunk.blockSize,
		-roundf(camera.posZ + deltaZ / chunk.blockSize));

	bool limit = false;
	if (nextFloor == nullptr)
	{
		cout << "End of world" << endl;
		limit = true;
	}
	else
	{
		Block* nextWall = chunk.getAdjacentBlocks(nextFloor).top;
		if (nextWall != nullptr && !nextWall->isTransparent)
		{
			cout << "Wall" << endl;
			limit = true;
		}
	}

	if (limit)
	{
		// Left
		if (!transp(feetContext.left, headContext.left) && blockX < -0.49f) blockX = -0.49f;
		// Right
		if (!transp(feetContext.right, headContext.right) && blockX > 0.49f) blockX = 0.49f;
		// Back (Block behind)
		if (!transp(feetContext.back, headContext.back) && blockZ < -0.49f) blockZ = -0.49f;
		// Front (Block in front)
		if (!transp(feetContext.front, headContext.front) && blockZ > 0.49f) blockZ = 0.49f;
	}

	camera.posX = (chunkX + blockX) * chunk.blockSize;
	camera.posZ = (chunkZ + blockZ) * chunk.blockSize;
}

void idle()
{
	float frameTime = glutGet(GLUT_ELAPSED_TIME)/1000.0f;
	float deltaTime = frameTime - lastFrameTime;
	lastFrameTime = frameTime;
	lastUpdate += deltaTime;

	const float speed = 3;
	if (keys['a']) move(270, deltaTime*speed);
	if (keys['d']) move(90, deltaTime*speed);
	if (keys['w']) move(0, deltaTime*speed);
	if (keys['s']) move(180, deltaTime*speed);
	if (keys[' ']) camera.posY += deltaTime * speed;
	if (keys['z']) camera.posY -= deltaTime * speed;
	//if (lastUpdate > 1.0f)
	//{
		lastUpdate = 0;
		Block* curBlock = chunk.getBlock(roundf(camera.posX / chunk.blockSize), (camera.posY - 2 * chunk.blockSize) / chunk.blockSize,
			-roundf(camera.posZ / chunk.blockSize));
		//cout << "Camera: x=" << camera.posX << ", y=" << camera.posY << ", z=" << camera.posZ;
		//cout << "|Position: " << (curBlock == nullptr ? "Unknown" : curBlock->getPositionString()) << endl;
		if (curBlock != nullptr)
		{
			curBlock->isTransparent = false;
			curBlock->setColor(Color4f::RED);
		}
	//}

	glutPostRedisplay();
}

bool justMovedMouse = false;
void mousePassiveMotion(int x, int y)
{
	int dx = x - width / 2;
	int dy = y - height / 2;
	if ((dx != 0 || dy != 0) && abs(dx) < 400 && abs(dy) < 400 && !justMovedMouse)
	{
		camera.rotY += dx / 10.0f;
		camera.rotX += dy / 10.0f;
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
		if (firstBlock == nullptr || spawnBlock->isTransparent)
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
		camera.posY = spawnBlock->y * chunk.blockSize + 2 * chunk.blockSize;
		camera.posZ = spawnBlock->z * chunk.blockSize;
		break;
	}

	glutMainLoop();


	return 0;
}