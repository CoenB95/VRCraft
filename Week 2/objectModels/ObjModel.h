#pragma once

#include <string>
#include <vector>
#include <list>

#include <windows.h>
#include <gl/GL.h>
#include "Vec.h"
class Texture
{
private:
	GLuint id = 0;
	std::string fileName;
public:
	Texture(std::string fileName);
	void apply() const;
	void load();
};



class ObjModel
{
private:
	class Vertex
	{
	public:
		int position = -1;
		int normal = -1;
		int texcoord = -1; 
	};

	class Face
	{
	public:
		std::list<Vertex> vertices;
	};
	class MaterialInfo
	{
	public:
		MaterialInfo();
		std::string name;
		Texture* texture;
	};

	class ObjGroup
	{
	public:
		std::string name;
		int materialIndex;
		std::list<Face> faces;
	};


	std::vector<Vec3f>	vertices;
	std::vector<Vec3f>	normals;
	std::vector<Vec2f>	texcoords;
	std::vector<ObjGroup*> groups;
	std::vector<MaterialInfo*> materials;

	void loadMaterialFile(const std::string &fileName, const std::string &dirName);
public:
	ObjModel(const std::string &filename);
	~ObjModel(void);

	void draw();
};

