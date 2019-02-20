#include <VrLib/Log.h>

#include "gameobjectcomponent.h"
#include "texturedrawcomponent.h"

using vrlib::Log;
using vrlib::logger;

TextureDrawComponent::TextureDrawComponent(const string& fileName) : GameObjectComponent() {
	logger << "Loading texture '" << fileName << "'..." << Log::newline;

	texture = vrlib::Texture::loadCached(fileName);

	if (texture == nullptr) {
		logger << "Failed to load texture" << Log::newline;
		return;
	}

	texture->setNearestFilter();
	logger << "Texture loading succeeded" << Log::newline;
}

void TextureDrawComponent::onDraw(const glm::mat4 &projectionMatrix, const glm::mat4 &modelViewMatrix) {
	if (verticesPtr == nullptr)
		return;

	if (texture == nullptr)
		return;

	texture->bind();

	glTranslatef(parentObject->position.x, parentObject->position.y, parentObject->position.z);
	vrlib::gl::setAttributes<vrlib::gl::VertexP3N3T2>(&(*verticesPtr)[0]);
	glDrawArrays(GL_TRIANGLES, 0, verticesPtr->size());
	glTranslatef(-parentObject->position.x, -parentObject->position.y, -parentObject->position.z);
	//glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);
	//glDisableVertexAttribArray(2);
}
