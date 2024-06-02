#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "texture2d.h"

//define PI
#define M_PI 3.1415926535897932384626433832795

using std::vector;

#pragma once
class Mesh
{

public:

	//Default constructor
	Mesh()
	{
		DiffuseTexture = Texture2D();
		SpecularTexture = Texture2D();
	};

	//Position of the object:
	glm::vec3 Position;

	//Vector of vertices
	vector<float> Vertices;

	//VAO and VBO
	unsigned int VAO, VBO;

	//Binds the VAO associated with this object
	void BindVAO() {
		glBindVertexArray(VAO);
	}

	//Draws the object
	void Draw() {

		//Bind Vert Array
		BindVAO();

		if (DiffuseTexture.Texture && SpecularTexture.Texture)
		{
			//Bind the Textures
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, DiffuseTexture.Texture);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, SpecularTexture.Texture);
		}

		//Check if Overlay Textures exist and bind if so
		if (HasOverlay())
		{
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, OverlayDiffuseTexture.Texture);
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, OverlaySpecularTexture.Texture);
		}
		else
		{
			//Unbind old textures
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, 0);
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		//Render the object
		glDrawArrays(GL_TRIANGLES, 0, (Vertices.size() / numVertexAttributes));
	}

	//De-allocates the resources associated with the VAO/VBO
	void DeallocateVertexArrayBuffers() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}

	//Sets the base textures
	void SetTextures(Texture2D diffuse, Texture2D specular)
	{
		this->DiffuseTexture = diffuse;
		this->SpecularTexture = specular;
	}

	//Sets the overlay textures
	void SetOverlayTextures(Texture2D diffuse, Texture2D specular)
	{
		this->OverlayDiffuseTexture = diffuse;
		this->OverlaySpecularTexture = specular;
	}

	//Returns 1 if there is an overlay, else 0
	char HasOverlay()
	{
		return (OverlayDiffuseTexture.Texture != 0 || OverlaySpecularTexture.Texture != 0) ? 1 : 0;
	}

	//Returns the shininess of the base texture
	float GetShininess()
	{
		return DiffuseTexture.GetShininess();
	}

protected:
	const int numVertexAttributes = 11;

	//Textures
	Texture2D DiffuseTexture;
	Texture2D SpecularTexture;
	Texture2D OverlayDiffuseTexture;
	Texture2D OverlaySpecularTexture;

	//Helper function to add the vertices
	void AddVertex(float x, float y, float z, const glm::vec3& color, const glm::vec3& normals, float u, float v) {
		Vertices.push_back(x);
		Vertices.push_back(y);
		Vertices.push_back(z);
		Vertices.push_back(color.r);
		Vertices.push_back(color.g);
		Vertices.push_back(color.b);
		Vertices.push_back(normals.x);
		Vertices.push_back(normals.y);
		Vertices.push_back(normals.z);
		Vertices.push_back(u);
		Vertices.push_back(v);
	}

	//Helper function to add the vertices
	void AddVertex(const glm::vec3& pos, const glm::vec3& color, const glm::vec3& normals, float u, float v) {
		Vertices.push_back(pos.x);
		Vertices.push_back(pos.y);
		Vertices.push_back(pos.z);
		Vertices.push_back(color.r);
		Vertices.push_back(color.g);
		Vertices.push_back(color.b);
		Vertices.push_back(normals.x);
		Vertices.push_back(normals.y);
		Vertices.push_back(normals.z);
		Vertices.push_back(u);
		Vertices.push_back(v);
	}

	//Generates the VAO and VBO for the object
	void GenerateVertexArrayAndBuffer() {

		//Gen the vertex array
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		//Gen and bind the buffer
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(float), &Vertices[0], GL_STATIC_DRAW);

		//Configure the Buffer Attributes

		//Position (x, y, z)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, numVertexAttributes * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute (r, g, b)
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, numVertexAttributes * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// normals attribute (x, y, z)
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, numVertexAttributes * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
		// texture attribute (U, V)
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, numVertexAttributes * sizeof(float), (void*)(9 * sizeof(float)));
		glEnableVertexAttribArray(3);

	}

};

#endif // !MESH_H