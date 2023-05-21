#include "marchingCube.h"

// Reference from http://paulbourke.net/geometry/polygonise/

float marchingCube::getGridValue(vec3 gridPoint, vec3 centerPoint) 
{	
	float x = gridPoint.x - centerPoint.x;
	float y = gridPoint.y - centerPoint.y;
	float z = gridPoint.z - centerPoint.z;
	
	return sqrt( pow(x,2) + pow(y,2) + pow(z,2) );
}

void marchingCube::drawMetaBall() 
{
	cout << "draw metaball" << endl;
	vector<vec3> gridPoints;
	vector<float> gridValues;

	float gridSize = 0.1;
	vec3 gridCenter = { 0.5, 0.5, 0.5 };
	glBegin(GL_TRIANGLES);
	// Initialize the grid and draw metaball
	for (float z = 0; z < 1; z += gridSize) {
		for (float y = 0; y < 1; y += gridSize) {
			for (float x = 0; x < 1; x += gridSize) {

				//printf("At x: %f, y: %f, z: %f \n", x, y, z);
				gridPoints.push_back({ x, y, z });
				gridValues.push_back( getGridValue(gridPoints.at(0), gridCenter) );

				gridPoints.push_back({ x+gridSize, y, z } );
				gridValues.push_back( getGridValue({ x + gridSize, y, z }, gridCenter) );
				
				gridPoints.push_back({ x + gridSize, y, z + gridSize });
				gridValues.push_back( getGridValue({ x + gridSize, y, z + gridSize }, gridCenter) );
				
				gridPoints.push_back({ x, y, z + gridSize });
				gridValues.push_back( getGridValue({ x, y, z + gridSize }, gridCenter) );
				
				gridPoints.push_back({ x, y + gridSize, z});
				gridValues.push_back( getGridValue({ x, y + gridSize, z }, gridCenter) );
				
				gridPoints.push_back({ x + gridSize, y + gridSize, z });
				gridValues.push_back( getGridValue({ x + gridSize, y + gridSize, z }, gridCenter) );
				
				gridPoints.push_back({ x + gridSize, y + gridSize, z + gridSize });
				gridValues.push_back( getGridValue({ x + gridSize, y + gridSize, z + gridSize }, gridCenter) );

				gridPoints.push_back({ x, y + gridSize, z + gridSize });
				gridValues.push_back( getGridValue({ x, y + gridSize, z + gridSize }, gridCenter) );

				polygonize(gridPoints, gridValues);
				
				gridPoints.clear();
				gridValues.clear();
			}
		}
	}
	glEnd();
}

void marchingCube::polygonize(vector<vec3> grids, vector<float> gridValues)
{
	vector<vec3> vertexList(12, {0, 0, 0});


	int vertexIndex = 0;
	double threshold = 0.5;

	// Check whether each of 8 vertices is within the range of metaball
	if (gridValues[0] <= threshold) vertexIndex |= 1;
	if (gridValues[1] <= threshold) vertexIndex |= 2;
	if (gridValues[2] <= threshold) vertexIndex |= 4;
	if (gridValues[3] <= threshold) vertexIndex |= 8;
	if (gridValues[4] <= threshold) vertexIndex |= 16;
	if (gridValues[5] <= threshold) vertexIndex |= 32;
	if (gridValues[6] <= threshold) vertexIndex |= 64;
	if (gridValues[7] <= threshold) vertexIndex |= 128;


	// If the vertex is outside or inside the surface
	if (edgeTable[vertexIndex] == 0) return;

	if (edgeTable[vertexIndex] & 1)
		vertexList.at(0) = interpolate(grids[0], grids[1], gridValues[0], gridValues[1], threshold);
	if (edgeTable[vertexIndex] & 2)
		vertexList.at(1) = interpolate(grids[1], grids[2], gridValues[1], gridValues[2], threshold);
	if (edgeTable[vertexIndex] & 4)
		vertexList.at(2) = interpolate(grids[2], grids[3], gridValues[2], gridValues[3], threshold);
	if (edgeTable[vertexIndex] & 8)
		vertexList.at(3) = interpolate(grids[3], grids[0], gridValues[3], gridValues[0], threshold);
	if (edgeTable[vertexIndex] & 16)
		vertexList.at(4) = interpolate(grids[4], grids[5], gridValues[4], gridValues[5], threshold);
	if (edgeTable[vertexIndex] & 32)
		vertexList.at(5) = interpolate(grids[5], grids[6], gridValues[5], gridValues[6], threshold);
	if (edgeTable[vertexIndex] & 64)
		vertexList.at(6) = interpolate(grids[6], grids[7], gridValues[6], gridValues[7], threshold);
	if (edgeTable[vertexIndex] & 128)
		vertexList.at(7) = interpolate(grids[7], grids[4], gridValues[7], gridValues[4], threshold);
	if (edgeTable[vertexIndex] & 256)
		vertexList.at(8) = interpolate(grids[0], grids[4], gridValues[0], gridValues[4], threshold);
	if (edgeTable[vertexIndex] & 512)
		vertexList.at(9) = interpolate(grids[1], grids[5], gridValues[1], gridValues[5], threshold);
	if (edgeTable[vertexIndex] & 1024)
		vertexList.at(10) = interpolate(grids[2], grids[6], gridValues[2], gridValues[6], threshold);
	if (edgeTable[vertexIndex] & 2048)
		vertexList.at(11) = interpolate(grids[3], grids[7], gridValues[3], gridValues[7], threshold);
	
	// Create triangles with vertices on edges
	vec3 n1;
	vec3 n2;
	vec3 n3;

	for (int i = 0; triTable[vertexIndex][i] != -1; i += 3) {

		vec3 vertex1 = vertexList[ triTable[vertexIndex][i] ];
		vec3 vertex2 = vertexList[ triTable[vertexIndex][i+1] ];
		vec3 vertex3 = vertexList[ triTable[vertexIndex][i+2] ];
	
		vec3 v1_1 = { vertex2.x - vertex1.x, vertex2.y - vertex1.y, vertex2.z - vertex1.z };
		vec3 v1_2 = { vertex3.x - vertex1.x, vertex3.y - vertex1.y, vertex3.z - vertex1.z };
		n1 = cross(v1_1, v1_2);
		
		glNormal3f(n1.x, n1.y, n1.z);
		glVertex3f(vertex1.x, vertex1.y, vertex1.z);
		
		glVertex3f(vertex2.x, vertex2.y, vertex2.z);
		
		glVertex3f(vertex3.x, vertex3.y, vertex3.z);
	}

	return;
}

vec3 marchingCube::interpolate(vec3 grid1, vec3 grid2, float gridValue1, float gridValue2, float threshold)
{
	float newX;
	float newY;
	float newZ;
	float t = (threshold - gridValue1) / (gridValue2 - gridValue1);;

	newX = grid1.x + t * (grid2.x - grid1.x);
	newY = grid1.y + t * (grid2.y - grid1.y);
	newZ = grid1.z + t * (grid2.z - grid1.z);

	return { newX, newY, newZ };
}

