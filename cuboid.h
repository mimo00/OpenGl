#include <glm/glm.hpp>
#include "mesh.h"
#include <vector>


class Cuboid
{
public:
	Cuboid(glm::vec3 start_point, glm::vec3 dimensions, unsigned int numSteps){
		this->start_point = start_point;
		this->dimensions = dimensions;
		this->numSteps = numSteps;
		c = 0;
	}

	Mesh getMesh(Texture* t){
		float x_step = dimensions.x / numSteps;
		float y_step = dimensions.y / numSteps;
		float z_step = dimensions.z / numSteps;
		glm::vec3 x_step_vec(x_step, 0, 0);
		glm::vec3 y_step_vec(0, y_step, 0);
		glm::vec3 z_step_vec(0, 0, z_step);
		//generate_bottom
		glm::vec3 start_point_negative = start_point + dimensions;
		glm::vec3 x_step_vec_neg(-x_step, 0, 0);
		glm::vec3 y_step_vec_neg(0, -y_step, 0);
		glm::vec3 z_step_vec_neg(0, 0, -z_step);

		geterate_wall(start_point, x_step_vec, z_step_vec, t);
		geterate_wall(start_point, z_step_vec, y_step_vec, t);
		geterate_wall(start_point, y_step_vec, x_step_vec, t);
		geterate_wall(start_point_negative, z_step_vec_neg, x_step_vec_neg, t);
		geterate_wall(start_point_negative, y_step_vec_neg, z_step_vec_neg, t);
		geterate_wall(start_point_negative, y_step_vec_neg, x_step_vec_neg, t);
		Mesh mesh(vertices, indices, t);
		return mesh;
	}

private:
	glm::vec3 start_point;
	glm::vec3 dimensions;
	unsigned int numSteps;
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	int c;
	
	void geterate_wall(glm::vec3 start_wall_point, glm::vec3 vec1, glm::vec3 vec2, Texture* t){
		glm::vec3 actual_vec(start_wall_point.x, start_wall_point.y, start_wall_point.z);
		for (int i = 0; i <= numSteps; i++){
			for (int j = 0; j <= numSteps; j++){
				Vertex vertex;
				vertex.Position = actual_vec;
				vertex.Normal = glm::cross(vec1, vec2);
				vertex.TexCoords = glm::vec2((1.0f / numSteps)*j, (1.0f / numSteps)*i);
				vertices.push_back(vertex);
				actual_vec = actual_vec + vec1;
			}
			for (int j = 0; j <= numSteps; j++){
				actual_vec = actual_vec - vec1;
			}
			actual_vec = actual_vec + vec2;
		}
		int skok = c*  (numSteps + 1)  * (numSteps + 1);
		for (int i = 0; i < numSteps; i++){
			for (int j = 0; j < numSteps; j++){
				int index_1 = (numSteps + 1)*i + j;
				int index_2 = index_1 + 1;
				int index_3 = (numSteps+1)*(i + 1) + j;
				int index_4 = index_3 + 1;

				index_1 = index_1 + skok;
				index_2 = index_2 + skok;
				index_3 = index_3 + skok;
				index_4 = index_4 + skok;
				addTriangleIndex(index_1, index_2, index_3);
				addTriangleIndex(index_2, index_3, index_4);
			}
		}
		c++;
	}


	void addTriangleIndex(unsigned int index1, unsigned int index2, unsigned int index3){
		indices.push_back(index1);
		indices.push_back(index2);
		indices.push_back(index3);
	}
};