#ifndef PLANE_H
#define PLANE_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include <random>
#include "stb_image.h"
#include "Mesh.h"

using namespace std;

class Plane : public Mesh
{
public:	
	//Dimensions (w, h, l)
	glm::vec3 Dimensions;

	//Constructor
	Plane(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float length = 2.0f, float width = 2.0f) 
	{
		Position = position;

		//X-axis = width, Y-axis = height, Z-axis = length
		Dimensions.x = width;
		Dimensions.z = length;

		//Calculate the vertices
		CalculateVertices();

		//Generate the VAO/VBO
		GenerateVertexArrayAndBuffer();
	}

private:

	//Calculates the vertices that make up the plane
	void CalculateVertices() {
		// Determine originOffset point
		glm::vec3 originOffset;
		originOffset.x = Dimensions.x / 2; // Half the width
		originOffset.z = Dimensions.z / 2; // Half the length

		// Generate vertices for a rectangle (ground plane)
		glm::vec3 vertColor;
		//vertColor = GenerateRandomVertColor();
		vertColor = glm::vec3(1.0f);

		glm::vec3 normals = glm::vec3(0.0f, 1.0f, 0.0f);

		// Triangle 1		
		AddVertex(Position.x - originOffset.x, Position.y, Position.z - originOffset.z, vertColor, normals, 0.0f, 0.0f);// Bottom left
		AddVertex(Position.x - originOffset.x, Position.y, Position.z + originOffset.z, vertColor, normals, 0.0f, 1.0f);// Top left
		AddVertex(Position.x + originOffset.x, Position.y, Position.z + originOffset.z, vertColor, normals, 1.0f, 1.0f);// Top right
																								   
		// Triangle 2																			   
		AddVertex(Position.x + originOffset.x, Position.y, Position.z + originOffset.z, vertColor, normals, 1.0f, 1.0f);// Top right
		AddVertex(Position.x + originOffset.x, Position.y, Position.z - originOffset.z, vertColor, normals, 1.0f, 0.0f);// Bottom right
		AddVertex(Position.x - originOffset.x, Position.y, Position.z - originOffset.z, vertColor, normals, 0.0f, 0.0f);// Bottom left
	}
};

#endif