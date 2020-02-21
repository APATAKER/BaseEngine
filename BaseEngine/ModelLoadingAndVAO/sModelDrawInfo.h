#pragma once
#include <string>

#include "sVertex_xyz_rgba_n_uv2_bt_4Bones.h"


struct sModelDrawInfo
{
	sModelDrawInfo()
	{
		this->VAO_ID = 0;

		this->VertexBufferID = 0;
		this->VertexBuffer_Start_Index = 0;
		this->numberOfVertices = 0;

		this->IndexBufferID = 0;
		this->IndexBuffer_Start_Index = 0;
		this->numberOfIndices = 0;
		this->numberOfTriangles = 0;

		// The "local" (i.e. "CPU side" temporary array)
		this->pVertices = 0;	// or NULL
		this->pIndices = 0;		// or NULL

		this->maxX = this->maxY = this->maxZ = 0.0f;
		this->minX = this->minY = this->minZ = 0.0f;
		this->extentX = this->extentY = this->extentZ = this->maxExtent = 0.0f;

		return;
	}
	std::string meshName;

	unsigned int VAO_ID;

	unsigned int VertexBufferID;
	unsigned int VertexBuffer_Start_Index;
	unsigned int numberOfVertices;

	unsigned int IndexBufferID;
	unsigned int IndexBuffer_Start_Index;
	unsigned int numberOfIndices;
	unsigned int numberOfTriangles;

	// The "local" (i.e. "CPU side" temporary array)
	sVertex_xyz_rgba_n_uv2_bt_4Bones* pVertices;	//  = 0;
	// The index buffer (CPU side)
	unsigned int* pIndices;

	// You could store the max and min values of the 
	//  vertices here (determined when you load them):
	float maxX, maxY, maxZ;
	float minX, minY, minZ;

	//	scale = 5.0/maxExtent;		-> 5x5x5
	float maxExtent;
	float extentX, extentY, extentZ;

	void CalcExtents(void);
};
