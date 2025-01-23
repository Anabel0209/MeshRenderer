/*
Nicole Vilkoff (40321430)
Christa Abou Arraje (40226631)
Anabel Prévost (40265371)
*/

#pragma once
#include <vector>

//class used to read the file of a model and exrtact its informations
class ModelImporter
{
private:
	//vector containing the values of the vertices 
	std::vector<float> vertVals;

	//vector containing the indexes of the faces of the triangles
	std::vector<float> triangleFaces;

public:

	//constructor
	ModelImporter();

	//method that reads the file and extract information
	void parseOBJ(const char* filePath);

	//accessors
	std::vector<float> getVertVals();
	std::vector<float> getTriangleFaces();
};
