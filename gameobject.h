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
	mutex componentsMutex;
	bool dirty = true;

protected:
	//Called when the object should (re)build its vertices to represent its current state. Could be called from a worker thread.
	virtual void build(vec3 offsetPosition);

	//Notifies that this object has become dirty and should be rebuild.
	inline void notifyDirty() { dirty = true; };

public:
	bool ut = true;
	vec3 position;
	quat orientation;
	vec3 scale = vec3(1, 1, 1);
	vec3 pivot = vec3(0, 0, 0);
	vector<vrlib::gl::VertexP3N3T2> vertices;
	mutex verticesMutex;

	vrlib::gl::Shader<Shaders::Uniforms>* shader;

	GameObject();
	virtual ~GameObject();
	GameObject(GameObject& other);
	void addComponent(GameObjectComponent* component);
	//Builds the object as being part of a parent mesh. Meaning its current position is used to determine vertice positions.
	virtual void buildEmbedded(vec3 offset = vec3(0, 0, 0));
	//Builds the object as being a standalone mesh. Meaning its current position is ignored when determining vertice positions.
	virtual void buildStandalone(bool pivotAsCenter = true);
	mat4 calcModelMatrix(const mat4& parentModelMatrix = mat4());
	void deleteAllComponents();
	void deleteComponent(GameObjectComponent* component);
	virtual void draw(const mat4& projectionMatrix, const mat4& viewMatrix, const mat4& parentModelMatrix = mat4());
	GameObjectComponent* findComponentByTag(string tag);
	virtual vec3 globalPosition() { return position; };
	bool hasComponent(string tag) { return findComponentByTag(tag) != nullptr; };
	void removeComponent(GameObjectComponent* component);
	inline bool shouldRebuild() { return dirty; };
	//Updates the object 
	virtual void update(float elapsedSeconds);
};
