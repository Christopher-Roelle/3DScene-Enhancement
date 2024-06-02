#ifndef SPHERE_H
#define SPHERE_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include <random>
#include "Mesh.h"

using namespace std;

class Sphere : public Mesh
{
public:

	float RadiusLong;
	float RadiusLat;

	int SideCount;
	int SubDivisions;
	bool SemiCircle;

	//Constructor - Uniform Sphere.
	Sphere(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float radius = 1.0f, int sides = 8, bool semiCircle = false)
	{
		Position = position;

		//X-axis = raidus, Y-axis = height
		RadiusLong = radius;
		RadiusLat = radius;

		SemiCircle = semiCircle;

		SideCount = sides;
		SubDivisions = sides;

		//Calculate the vertices
		CalculateVertices();

		//Generate the VAO/VBO
		GenerateVertexArrayAndBuffer();
	}

	//Constructor - Allows for differing longitude and latitude radius
	Sphere(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float radiusLong = 1.0f, float radiusLat = 1.0f, int sides = 8, bool semiCircle = false)
	{
		Position = position;

		//X-axis = raidus, Y-axis = height
		RadiusLong = radiusLong;
		RadiusLat = radiusLat;

		SemiCircle = semiCircle;

		SideCount = sides;
		SubDivisions = sides;

		//Calculate the vertices
		CalculateVertices();

		//Generate the VAO/VBO
		GenerateVertexArrayAndBuffer();
	}

private:

	//Calculates the vertices that make up the plane
	void CalculateVertices() {
		// Generate vertices for a Sphere
		glm::vec3 vertColor = glm::vec3(1.0f);
		float u = 1.0f / static_cast<float>(SideCount - 1);
		float v = 1.0f / static_cast<float>(SubDivisions);

		glm::vec3 normals;

		//Limit the sides generated based on if its a semi circle or not. Doing on latitude (N/S) because my application has a semicircle facing downward.
		int latitudeLimit = (SemiCircle)?SideCount / 2:SideCount;

		//Latitude Loop (N/S)
		for (int i = 0; i < latitudeLimit; i++) {
			float phi1 = static_cast<float>(M_PI * i) / (SideCount - 1);
			float phi2 = static_cast<float>(M_PI * (i + 1)) / (SideCount - 1);

			// Longitude Loop (E/W) where the actual faces are drawn.
			for (int j = 0; j < SubDivisions; j++) {
				float theta1 = static_cast<float>(2.0f * M_PI * j) / SubDivisions;
				float theta2 = static_cast<float>(2.0f * M_PI * (j + 1)) / SubDivisions;

				// Vertex 1
				glm::vec3 vert1 = CalculateSphereVertex(Position, RadiusLong, RadiusLat, phi1, theta1);

				// Vertex 2
				glm::vec3 vert2 = CalculateSphereVertex(Position, RadiusLong, RadiusLat, phi1, theta2);

				// Vertex 3
				glm::vec3 vert3 = CalculateSphereVertex(Position, RadiusLong, RadiusLat, phi2, theta1);

				// Vertex 4
				glm::vec3 vert4 = CalculateSphereVertex(Position, RadiusLong, RadiusLat, phi2, theta2);

				glm::vec3 edge1 = vert3 - vert1;
				glm::vec3 edge2 = vert2 - vert1;
				normals = glm::normalize(glm::cross(edge1, edge2));

				// Right triangle
				AddVertex(vert1.x, vert1.y, vert1.z, vertColor, normals, 1.0f - (u * j), v * i);
				AddVertex(vert2.x, vert2.y, vert2.z, vertColor, normals, 1.0f - (u * (j + 1)), v * i);
				AddVertex(vert3.x, vert3.y, vert3.z, vertColor, normals, 1.0f - (u * j), v * (i + 1));

				edge1 = vert2 - vert4;
				edge2 = vert3 - vert4;
				normals = glm::normalize(glm::cross(edge1, edge2));

				// Left triangle
				AddVertex(vert2.x, vert2.y, vert2.z, vertColor, normals, 1.0f - (u * (j + 1)), v * i);
				AddVertex(vert4.x, vert4.y, vert4.z, vertColor, normals, 1.0f - (u * (j + 1)), v * (i + 1));
				AddVertex(vert3.x, vert3.y, vert3.z, vertColor, normals, 1.0f - (u * j), v * (i + 1));
			}
		}

		// If this is a semi-circle, we need to cap it. Full sphere doesnt require a cap.
		if (SemiCircle) {
			normals = glm::vec3(0.0f, -1.0f, 0.0);

			for (int i = 0; i <= SideCount; ++i) {

				float curTheta = (float)(2.0f * M_PI * i) / SideCount;
				float nxtTheta = (float)(2.0f * M_PI * (i + 1)) / SideCount;

				float curX = Position.x + RadiusLong * cos(curTheta);
				float curZ = Position.z + RadiusLong * sin(curTheta);

				float nxtX = Position.x + RadiusLong * cos(nxtTheta);
				float nxtZ = Position.z + RadiusLong * sin(nxtTheta);

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
	}

	//Helper function to calculate the vertices for the sphere.
	glm::vec3 CalculateSphereVertex(const glm::vec3& position, float radiusLong, float radiusLat, float phi, float theta) {
		float x = position.x + radiusLong * sin(phi) * cos(theta);
		float y = position.y + radiusLat * cos(phi);
		float z = position.z + radiusLong * sin(phi) * sin(theta);
		return glm::vec3(x, y, z);
	}
};

#endif