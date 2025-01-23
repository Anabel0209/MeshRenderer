/*
Nicole Vilkoff (40321430)
Christa Abou Arraje (40226631)
Anabel Prévost (40265371)
*/

#include "ImportedModel.h"
#include "ModelImporter.h"

//constructor
ImportedModel::ImportedModel(const char* filePath)
{
	//create a modelImporter object
	ModelImporter modelImporter = ModelImporter();

	//use model importer to get vertex information
	modelImporter.parseOBJ(filePath);

	//assign the imformation found in the file to our vectors certices and faces
	vertices = modelImporter.getVertVals();
	faces = modelImporter.getTriangleFaces();
}

//accessors
std::vector<float> ImportedModel::getVertices()
{
	return vertices;
}

std::vector<float> ImportedModel::getFaces()
{
	return faces;
}
