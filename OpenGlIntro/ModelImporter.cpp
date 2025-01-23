/*
Nicole Vilkoff (40321430)
Christa Abou Arraje (40226631)
Anabel Prévost (40265371)
*/

#include "ModelImporter.h"
#include <sstream>
#include <fstream>

//constructor
ModelImporter::ModelImporter()
{
	//does nothing
}

void ModelImporter::parseOBJ(const char* filePath)
{
	//coordinates read in the file
	float x, y, z;

	//open the file for reading
	std::ifstream fileStream(filePath, std::ios::in);

	//will hold what is read
	std::string line = "";

	//repeat until we reach the end of the file
	while (!fileStream.eof())
	{
		//read a line and store it in line
		getline(fileStream, line);

		//if the firs character of the line is v alone
		if (line.compare(0, 2, "v ") == 0)
		{
			//extract the vertex position values (vertices)
			std::stringstream ss(line.erase(0, 1)); //start extracting after the v
			ss >> x;
			ss >> y;
			ss >> z;
			//push the values in the vector
			vertVals.push_back(x);
			vertVals.push_back(y);
			vertVals.push_back(z);
		}

		//if the firsr character of the line is f (faces)
		if (line.compare(0, 2, "f ") == 0)
		{
			//different types of values in the face line
			std::string grouping, v, t, n;

			//start extracting after the f  
			std::stringstream ss(line.erase(0, 2)); 

			//loop 3 times because we read 3 pack of information (v/t/n v/t/n v/t/n)
			for (int i = 0; i < 3; i++)
			{
				//read one grouping at a time 
				getline(ss, grouping, ' '); 
				std::stringstream groupingSS(grouping);

				//extract each informations
				getline(groupingSS, v, '/');
				getline(groupingSS, t, '/');
				getline(groupingSS, n, '/');

				//keep the v part associated with the a vertex reference then convert it to int and substract 1(because indexes in .obj start at 1)
				int vertRef = (stoi(v) - 1);

				//push the vertRef into the vector used for organizing the triangle faces
				triangleFaces.push_back(vertRef);
			}
		}
	}//end while
}

//accessors
std::vector<float> ModelImporter::getVertVals()
{
	return vertVals;
}

std::vector<float> ModelImporter::getTriangleFaces()
{
	return triangleFaces;
}
