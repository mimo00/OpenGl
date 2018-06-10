#include <glm/glm.hpp>
#include "constant.h"

enum {GOING_DOWN, GOING_UP, MOVE_BOARD, END};

class Animation {
public:
	glm::vec3 cylinder_model_translation;
	glm::vec3 board_model_translation;
	float cylinder_speed = 1.5f;
	float board_speed = 1.5f;
	float max_cylinder = CYLINDER_HEIGHT / 2;
	float min_cylinder = 0.0f;
	int state;
	int number_of_moves_iterator = 1;
	float move_lenght = BOARD_LENGHT / (NUMBER_OF_MOVES-1);
	
	Animation() {
		this->cylinder_model_translation = glm::vec3(0.0f, CYLINDER_HEIGHT / 2, 0.0f);
		this->board_model_translation = glm::vec3(0.0f, 0.0f, 0.0f);
		this->state = GOING_DOWN;
	}

	void update_translations_models(float deltaTime) {
		switch (state) {
			case GOING_UP:
				if (cylinder_model_translation.y < max_cylinder) {
					cylinder_model_translation += glm::vec3(0.0f, 1.0f, 0.0f)*deltaTime*cylinder_speed;
				}
				else {
					state = MOVE_BOARD;
				}
			break;
			case GOING_DOWN:
				if (cylinder_model_translation.y > min_cylinder) {
					cylinder_model_translation -= glm::vec3(0.0f, 1.0f, 0.0f)*deltaTime*cylinder_speed;
				} else {
					state = GOING_UP;
				}
			break;
			case MOVE_BOARD:
				if (number_of_moves_iterator < NUMBER_OF_MOVES) {
					
					if (board_model_translation.z < number_of_moves_iterator * move_lenght) {
						board_model_translation += glm::vec3(0.0f, 0.0f, 1.0f)*deltaTime*board_speed;
					}
					else
					{
						number_of_moves_iterator++;
						state = GOING_DOWN;
					}
				}
				else {
					state = END;
				}
			break;
		}
	}

};