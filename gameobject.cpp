#include <iostream>
#include <GL/glew.h>

#include "gameobject.h"
#include "gameobjectcomponent.h"

using namespace std;

GameObject::GameObject()
{

}

GameObject::GameObject(GameObject& other)
{
	position = other.position;
	rotateX = other.rotateX;
	rotateY = other.rotateY;
	rotateZ = other.rotateZ;
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

void GameObject::removeAllComponents()
{
	for (GameObjectComponent* component : components)
	{
		if (component != nullptr)
		{
			delete component;
		}
	}

	components.clear();
}

void GameObject::update(float elapsedSeconds)
{
	for (GameObjectComponent* component : components)
	{
		component->update(elapsedSeconds);
	}
}
