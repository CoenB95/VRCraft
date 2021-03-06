#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <mutex>
#include <vector>
#include <VrLib/gl/shader.h>
#include <VrLib/gl/Vertex.h>

using namespace glm;
using namespace std;

class GameObjectComponent;
class GameObjectGroup;

class GameObject
{
private:
	vector<GameObjectComponent*> components;
	mutex componentsMutex;
	bool dirty = true;

protected:
	//Called when the object should (re)build its vertices to represent its current state. Could be called from a worker thread.
	virtual void build(vec3 offsetPosition);

public:
	GameObjectGroup* parent;

	vec3 position;
	quat orientation;
	vec3 scale = vec3(1, 1, 1);
	vec3 pivot = vec3(0, 0, 0);
	vector<vrlib::gl::VertexP3N3T2> vertices;
	mutex verticesMutex;

	GameObject();
	virtual ~GameObject();
	GameObject(GameObject& other);
	void addComponent(GameObjectComponent* component);
	//Builds the object as being part of a parent mesh. Meaning its current position is used to determine vertice positions.
	void buildEmbedded(vec3 offset = vec3(0, 0, 0));
	//Builds the object as being a standalone mesh. Meaning its current position is ignored when determining vertice positions.
	void buildStandalone(bool pivotAsCenter = true);
	mat4 calcModelMatrix(const mat4& parentModelMatrix = mat4());
	void deleteAllComponents();
	void deleteComponent(GameObjectComponent* component);
	virtual void draw(const mat4& parentModelMatrix);
	GameObjectComponent* findComponentByTag(string tag);
	vec3 globalPosition();
	bool hasComponent(string tag) { return findComponentByTag(tag) != nullptr; };
	//Notifies that this object has become dirty and should be rebuild.
	void notifyDirty();
	void removeComponent(GameObjectComponent* component);
	inline bool shouldRebuild() { return dirty; };
	//Updates the object 
	virtual void update(float elapsedSeconds);
};
