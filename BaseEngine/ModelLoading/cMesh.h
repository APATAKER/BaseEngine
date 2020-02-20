#ifndef _cMesh_HG_
#define _cMesh_HG_

#include <vector>		// "smart array"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <string>

// This structure matches the file, so that our life is
//   simpler and happier, and we all get along.
//struct sPlyVertexXYZ_N
//{
//	float x, y, z;
//	float nx, ny, nz;		// NOW with NORMALS!!
//};
struct sPlyVertexXYZ_N_UV
{
	float x, y, z;
	float nx, ny, nz;		
	float u, v;				// NOW with TEXTURE COORDINATES!!!
	// Add this, too:
	float bi_x, bi_y, bi_z;
	float tan_x, tan_y, tan_z;
	float boneID[4];		// Which bone impacts this vertex
	float boneWeights[4];	// How much does each bone				// NOW with BONE ID AND WEIGHTS!!!
};
struct sPlyTriangle
{
	unsigned int vert_index_1;
	unsigned int vert_index_2;
	unsigned int vert_index_3;
};

class cMesh
{
public:
	std::string meshname;
	cMesh() {};			// Empty c'tor
	~cMesh() {};		// Empty d'tor
	// Store the vertices
	std::vector<sPlyVertexXYZ_N_UV> vecVertices;

	// Store the triangles
	std::vector<sPlyTriangle> vecTriangles;

	enum enumTEXCOORDBIAS
	{
		POSITIVE_X, POSITIVE_Y, POSITIVE_Z
	};

	void GenTextureCoordsSpherical(void);
	void GenTextureCoordsSpherical(enumTEXCOORDBIAS uBias, enumTEXCOORDBIAS vBias, bool basedOnNormals, float scale, bool fast);

};

#endif 
