/*
Nicole Vilkoff (40321430)
Christa Abou Arraje (40226631)
Anabel Prévost (40265371)
*/

#pragma once
#include <vector>

//class that contains the array of vertices and and faces index that will be used to render our model
class ImportedModel
{
private:
	std::vector<float> vertices;
	std::vector<float> faces;

public:
	//constructor
	ImportedModel(const char* filePath);

	//getters
	std::vector<float> getVertices();
	std::vector<float> getFaces();
};

