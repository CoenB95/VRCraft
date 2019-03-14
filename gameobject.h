#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <mutex>
#include <vector>
#include <VrLib/gl/shader.h>
#include <VrLib/gl/Vertex.h>

#include "shaders.h"

using namespace glm;
using namespace std;

class GameObjectComponent;

class GameObject
{
private:
	vector<GameObjectComponent*> components;
	bool dirty = true;

protected:
	//Notifies that this object has become dirty and should be rebuild.
	inline void notifyDirty() { dirty = true; };

public:
	vec3 position;
	quat orientation;
	vec3 scale = vec3(1, 1, 1);

	vector<vrlib::gl::VertexP3N3T2> vertices;
	mutex verticesMutex;
	vec3 verticesOffset;

	vrlib::gl::Shader<Shaders::Uniforms>* shader;

	GameObject();
	GameObject(GameObject& other);
	void addComponent(GameObjectComponent* component);
	//(Re-)builds the object to represent its current state. Could be called from a worker thread.
	virtual void build();
	mat4 calcModelMatrix(const mat4& parentModelMatrix = mat4());
	virtual void draw(const mat4& projectionMatrix, const mat4& viewMatrix, const mat4& parentModelMatrix = mat4());
	virtual vec3 globalPosition() { return position; };
	void removeAllComponents();
	inline bool shouldRebuild() { return dirty; };
	//Updates the object 
	virtual void update(float elapsedSeconds);
};
