#include "cModelLoader.h"

#include <iostream>			// cin cout
#include <fstream>		    // ifstream ofstream

#include <glm/glm.hpp>
#include <glm/vec3.hpp>

cModelLoader::cModelLoader()			// constructor
{
	std::cout << "A cModelLoader() is created!" << std::endl;
	return;
}

cModelLoader::~cModelLoader()			// destructor
{
	std::cout << "A cModelLoader() is destroyed" << std::endl;
	return;
}

// Takes the filename to load
// Returns by ref the mesh
bool cModelLoader::LoadPlyModel(
	std::string filename,
	cMesh &theMesh, 				// Note the "&"
	bool generateSphericalUVs /*=false*/)
{

	std::ifstream theFile( filename.c_str() );
	if ( ! theFile.is_open() )
	{	
		// On no! Where's the file??? 
		return false;
	}

	// Scan the file until I get to "vertex", and stop
	std::string temp;
	//theFile >> temp;
	while ( theFile >> temp ) 
	{
		// Have it hit the word "vertex"?
		if ( temp == "vertex" )
		{
			break;		// Exit the while loop
		}
	}// while ( theFile >> temp ) 
	
	unsigned int numberOfVertices;
	theFile >> numberOfVertices;



	while (theFile >> temp)
	{
		// Have it hit the word "face"?
		if (temp == "face")
		{
			break;		// Exit the while loop
		}
	}// while ( theFile >> temp ) 

	unsigned int numberOfTriangles;
	theFile >> numberOfTriangles;

	while (theFile >> temp)
	{
		// Have it hit the word "end_header"?
		if (temp == "end_header")
		{
			break;		// Exit the while loop
		}
	}// while ( theFile >> temp ) 



	// Read all the vertices
	for (unsigned int index = 0; index != numberOfVertices; index++)
	{
		// -0.0312216 0.126304 0.00514924
		sPlyVertexXYZ_N_UV tempVertex;

		// -0.0312216 0.126304 0.00514924
		theFile >> tempVertex.x >> tempVertex.y >> tempVertex.z;

		// Also load the normals
		glm::vec3 normal;
		theFile >> normal.x >> normal.y >> normal.z;
		normal = glm::normalize(normal);
		tempVertex.nx = normal.x; 
		tempVertex.ny = normal.y; 
		tempVertex.nz = normal.z;

		// Also load the texture coordinates
		theFile >> tempVertex.u >> tempVertex.v;


		// Add this temp vertex to the vector of vertices
		// (cMesh &theMesh)
		theMesh.vecVertices.push_back( tempVertex );
	}

	for (unsigned int index = 0; index != numberOfTriangles; index++)
	{
		// 3 166 210 265 
		int discardThis;
		sPlyTriangle tempTriangle;

		theFile >> discardThis 
			>> tempTriangle.vert_index_1 
			>> tempTriangle.vert_index_2
			>> tempTriangle.vert_index_3;

		// Add this triangle
		theMesh.vecTriangles.push_back(tempTriangle);
	}


	if (generateSphericalUVs)
	{
		theMesh.GenTextureCoordsSpherical();
	}

	return true;
}
