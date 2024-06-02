#ifndef CYLINDER_H
#define CYLINDER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include <random>
#include "Mesh.h"

using namespace std;

class Cylinder : public Mesh
{
public:
	//Dimensions (r, h)
	glm::vec2 Dimensions;

	int SideCount;
	int SubDivisions;
	bool TopDrawn;
	bool BtmDrawn;

	//Constructor
	Cylinder(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float radius = 2.0f, float height = 2.0f, int sides = 8, int subdivisions = 1, bool drawTop = true, bool drawBottom = true)
	{
		Position = position;

		//X-axis = raidus, Y-axis = height
		Dimensions.x = radius;
		Dimensions.y = height;

		TopDrawn = drawTop;
		BtmDrawn = drawBottom;

		SideCount = sides;

		//Need at least one subdivision
		if (subdivisions <= 0) { subdivisions = 1; }
		SubDivisions = subdivisions;

		//Calculate the vertices
		CalculateVertices();

		//Generate the VAO/VBO
		GenerateVertexArrayAndBuffer();
	}

private:

	//Calculates the vertices that make up the plane
	void CalculateVertices() {

		// Generate vertices for a Cylinder (ground plane)
		glm::vec3 vertColor;
		//vertColor = GenerateRandomVertColor();
		vertColor = glm::vec3(1.0f);
		float radius = Dimensions.x;
		float height = Dimensions.y;

		float u = (1.0f / (float)SideCount);
		float v = (1.0f / (float)SubDivisions);

		glm::vec3 normals;

		// Connect the vertices to form triangles for the sides
		for (int i = 0; i < SideCount; i++) {
			// Calculate our points along the circumference
			float theta1 = (float)(2.0f * M_PI * i) / SideCount;
			float theta2 = (float)(2.0f * M_PI * (i + 1)) / SideCount;

			float x1 = Position.x + radius * cos(theta1);
			float z1 = Position.z + radius * sin(theta1);

			float x2 = Position.x + radius * cos(theta2);
			float z2 = Position.z + radius * sin(theta2);

			float divHeight = height / SubDivisions;

			//Subdivide, starting from the bottom, then stack up
			for (int j = 0; j < SubDivisions; j++) {

				float btmY = Position.y + (divHeight * j);
				float topY = Position.y + (divHeight * (j + 1));

				//Generate the normals
				glm::vec3 vert1 = glm::vec3(x1, btmY, z1);
				glm::vec3 vert2 = glm::vec3(x1, topY, z1);
				glm::vec3 vert3 = glm::vec3(x2, topY, z2);

				glm::vec3 edge1 = vert3 - vert1;
				glm::vec3 edge2 = vert2 - vert1;

				normals = glm::normalize(glm::cross(edge1, edge2));

				//Right triangle
				AddVertex(x1, btmY, z1, vertColor, normals, 1 - (u * i), v * j); //Downward Tip (Bottom Right)
				AddVertex(x1, topY, z1, vertColor, normals, 1 - (u * i), v * (j + 1)); //Top Right
				AddVertex(x2, topY, z2, vertColor, normals, 1 - (u * (i + 1)), v * (j + 1)); //Top Left

				vert1 = glm::vec3(x2, topY, z2);
				vert2 = glm::vec3(x2, btmY, z2);
				vert3 = glm::vec3(x1, btmY, z1);

				edge1 = vert3 - vert1;
				edge2 = vert2 - vert1;

				normals = glm::normalize(glm::cross(edge1, edge2));

				//Left triangle					   
				AddVertex(x2, topY, z2, vertColor, normals, 1 - (u * (i + 1)), v * (j + 1)); //Upward Tip (Top Left)
				AddVertex(x2, btmY, z2, vertColor, normals, 1 - (u * (i + 1)), v * j); //Bottom Left
				AddVertex(x1, btmY, z1, vertColor, normals, 1 - (u * i), v * j); //Bottom Right
			}
		}

		// Create the vertices for the bottom, I will probably use the bottom everytime.
		if (BtmDrawn) {

			normals = glm::vec3(0.0f, -1.0f, 0.0);

			for (int i = 0; i <= SideCount; ++i) {

				float curTheta = (float)(2.0f * M_PI * i) / SideCount;
				float nxtTheta = (float)(2.0f * M_PI * (i + 1)) / SideCount;

				float curX = Position.x + radius * cos(curTheta);
				float curZ = Position.z + radius * sin(curTheta);

				float nxtX = Position.x + radius * cos(nxtTheta);
				float nxtZ = Position.z + radius * sin(nxtTheta);

				float textureCurThetaU = 1.0f * cos(curTheta);
				float textureCurThetaV = 1.0f * sin(curTheta);

				float textureNxtThetaU = 1.0f * cos(nxtTheta);
				float textureNxtThetaV = 1.0f * sin(nxtTheta);

				// Triangle connecting the center, current, and next vertices
				AddVertex(Position.x, Position.y, Position.z, vertColor, normals, 0.0f, 0.0f); //Center
				AddVertex(curX, Position.y, curZ, vertColor, normals, textureCurThetaU, textureCurThetaV);
				AddVertex(nxtX, Position.y, nxtZ, vertColor, normals, textureNxtThetaU, textureNxtThetaV);

			}
		}

		// Create the vertices for the bottom, I will probably use the bottom everytime.
		if (TopDrawn) {

			normals = glm::vec3(0.0f, 1.0f, 0.0f);

			for (int i = 0; i <= SideCount; ++i) {

				float curTheta = (float)(2.0f * M_PI * i) / SideCount;
				float nxtTheta = (float)(2.0f * M_PI * (i + 1)) / SideCount;

				float curX = Position.x + radius * cos(curTheta);
				float curZ = Position.z + radius * sin(curTheta);

				float nxtX = Position.x + radius * cos(nxtTheta);
				float nxtZ = Position.z + radius * sin(nxtTheta);

				float textureCurThetaU = 1.0f * cos(curTheta);
				float textureCurThetaV = 1.0f * sin(curTheta);

				float textureNxtThetaU = 1.0f * cos(nxtTheta);
				float textureNxtThetaV = 1.0f * sin(nxtTheta);

				// Triangle connecting the center, current, and next vertices
				AddVertex(Position.x, Position.y + height, Position.z, vertColor, normals, 0.0f, 0.0f); //Center
				AddVertex(curX, Position.y + height, curZ, vertColor, normals, textureCurThetaU, textureCurThetaV);
				AddVertex(nxtX, Position.y + height, nxtZ, vertColor, normals, textureNxtThetaU, textureNxtThetaV);

			}
		}
	}
};

#endif