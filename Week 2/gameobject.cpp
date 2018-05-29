#include <iostream>
#include <gl/freeglut.h>

#include "gameobject.h"
#include "gameobjectcomponent.h"

using namespace std;

GameObject::GameObject()
{

}

void GameObject::addComponent(GameObjectComponent* component)
{
	component->setParent(this);

	DrawComponent* dc = dynamic_cast<DrawComponent*>(component);
	if (dc != nullptr)
	{
		if (drawComponent != nullptr)
			cout << "Warning: multiple draw-components supplied." << endl;

		drawComponent = dc;
	}

	this->components.push_back(component);
}

void GameObject::draw()
{
	if (drawComponent != nullptr)
	{
		glPushMatrix();
		
		glTranslatef(position.x, position.y, -position.z);
		
		glRotatef(rotateZ, 0, 0, 1);
		glRotatef(rotateX, 1, 0, 0);
		glRotatef(rotateY, 0, 1, 0);

		drawComponent->draw();

		glPopMatrix();
	}
}

void GameObject::update(float elapsedSeconds)
{
	for (GameObjectComponent* component : components)
	{
		component->update(elapsedSeconds);
	}
}
