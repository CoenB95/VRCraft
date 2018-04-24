#include <GL/freeglut.h>

class Block
{
public:
	void draw() {
		// Front
		if (!useHue)
			glColor3f(1, 0, 0);
		glVertex3f(-hw, hh, hd);
		glVertex3f(-hw, -hh, hd);
		glVertex3f(hw, -hh, hd);
		glVertex3f(hw, hh, hd);
		glVertex3f(-hw, hh, hd);
		glVertex3f(hw, -hh, hd);

		// Top
		if (!useHue)
			glColor3f(0, 1, 0);
		glVertex3f(-hw, hh, -hd);
		glVertex3f(-hw, hh, hd);
		glVertex3f(hw, hh, hd);
		glVertex3f(hw, hh, -hd);
		glVertex3f(-hw, hh, -hd);
		glVertex3f(hw, hh, hd);

		// Right
		if (!useHue)
			glColor3f(0, 0, 1);
		glVertex3f(hw, hh, hd);
		glVertex3f(hw, -hh, hd);
		glVertex3f(hw, -hh, -hd);
		glVertex3f(hw, hh, -hd);
		glVertex3f(hw, hh, hd);
		glVertex3f(hw, -hh, -hd);

		// Back
		if (!useHue)
			glColor3f(1, 1, 0);
		glVertex3f(hw, hh, -hd);
		glVertex3f(hw, -hh, -hd);
		glVertex3f(-hw, -hh, -hd);
		glVertex3f(-hw, hh, -hd);
		glVertex3f(hw, hh, -hd);
		glVertex3f(-hw, -hh, -hd);

		// Bottom
		if (!useHue)
			glColor3f(0, 1, 1);
		glVertex3f(-hw, -hh, hd);
		glVertex3f(-hw, -hh, -hd);
		glVertex3f(hw, -hh, -hd);
		glVertex3f(hw, -hh, hd);
		glVertex3f(-hw, -hh, hd);
		glVertex3f(hw, -hh, -hd);

		// Left
		if (!useHue)
			glColor3f(1, 0, 1);
		glVertex3f(-hw, hh, -hd);
		glVertex3f(-hw, -hh, -hd);
		glVertex3f(-hw, -hh, hd);
		glVertex3f(-hw, hh, hd);
		glVertex3f(-hw, hh, -hd);
		glVertex3f(-hw, -hh, hd);

		glEnd();
	}
};
