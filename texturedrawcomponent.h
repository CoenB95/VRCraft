#pragma once

#include <mutex>
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
	TextureDrawComponent(const string& fileName);

	void onDraw() override;
};
