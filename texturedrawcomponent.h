#pragma once

#include <vector>
#include <VrLib/Texture.h>
#include <VrLib/gl/Vertex.h>

#include "gameobjectcomponent.h"

using namespace std;

class TextureDrawComponent : public GameObjectComponent
{
private:
	vrlib::Texture* texture;

public:
	vector<vrlib::gl::VertexP3N3T2>* verticesPtr;

	TextureDrawComponent(const string& fileName);

	void onDraw(const glm::mat4 &projectionMatrix, const glm::mat4 &modelViewMatrix) override;
};
