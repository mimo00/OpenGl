#include "mesh.h"
#include <vector>

#define M_PI	3.14159265358979323846

enum Cylinder_Movement {
	UP,
	DOWN
};

class Cylinder
{
public:
	float radius;
	float length;
	unsigned int numSteps;
	unsigned int how_many_vertex;
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	float MovementSpeed;
	float start_height;
	float SPEED = 2.5f;


	Cylinder(float radius, float length, unsigned int numSteps)
	{
		this->radius = radius;
		this->length = length;
		this->numSteps = numSteps;
		this->how_many_vertex = numSteps * 2;
		this->MovementSpeed = SPEED;
		this->start_height = 0;
	}

	Mesh getCylinderMesh(Texture* t)
	{
		float actual_radius = 0.0f;
		float radius_step = 2 * M_PI / (float)numSteps;
		float hl = length * 0.5f;
		float textureStep = 1.0 / (float)numSteps;
		float actualTextureWidth = 0.0;
		vertices.resize(how_many_vertex);
		for (int i = 0; i < numSteps; ++i)
		{
			float z = cos(actual_radius)*radius;
			float x = sin(actual_radius)*radius;

			Vertex vertex;

			vertex.Position = glm::vec3(x, start_height + hl, z);
			vertex.Normal = glm::vec3(2 * x, hl, 2 * z);
			vertex.TexCoords = glm::vec2(actualTextureWidth, 1.0f);

			Vertex vertex2;
			vertex2.Position = glm::vec3(x, start_height - hl, z);
			vertex2.Normal = glm::vec3(2 * x, -hl, 2 * z);
			vertex2.TexCoords = glm::vec2(actualTextureWidth, 0.0f);

			vertices[i] = vertex;
			vertices[i + numSteps] = vertex2;

			actual_radius += radius_step;
			actualTextureWidth += textureStep;
		}

		for (int i = 0; i < numSteps; ++i)
		{
			unsigned int i1 = i;
			unsigned int i2 = (i + 1) % numSteps;
			unsigned int i3 = i + numSteps;
			unsigned int i4 = i2 + numSteps;
			addTriangleIndex(i1, i3, i2);
			addTriangleIndex(i4, i2, i3);
		}
		Mesh mesh(vertices, indices, t);
		return mesh;
	}

	void processMove(Cylinder_Movement direction, float deltaTime)
	{
		cout << "SIEMA " << this->start_height << endl;

		float velocity = MovementSpeed * deltaTime;
		if (direction == UP)
			start_height += velocity;
		if (direction == DOWN)
			start_height -= velocity;
	}

private:
	void addTriangleIndex(unsigned int index1, unsigned int index2, unsigned int index3){
		indices.push_back(index1);
		indices.push_back(index2);
		indices.push_back(index3);
	}
};