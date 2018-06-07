#include <glm/glm.hpp>

enum {GOING_DOWN, GOING_UP, MOVE_BOARD};

class Animation {
public:
	glm::vec3 cylinder_model_translation;
	glm::vec3 board_model_translation;
	float cylinder_speed = 1.5f;
	float max_cylinder = 2.0f;
	float min_cylinder = -2.0f;
	int state;
	
	Animation() {
		this->cylinder_model_translation = glm::vec3(0.0f, 0.0f, 0.0f);
		this->board_model_translation = glm::vec3(0.0f, 0.0f, 0.0f);
		this->state = GOING_DOWN;
	}

	void update_translations_models(float deltaTime) {
		switch (state) {
			case GOING_DOWN:
				if (cylinder_model_translation.y < max_cylinder) {
					cylinder_model_translation += glm::vec3(0.0f, 1.0f, 0.0f)*deltaTime*cylinder_speed;
				}
				else {
					state = GOING_UP;
				}
			break;
			case GOING_UP:
				if (cylinder_model_translation.y > min_cylinder) {
					cylinder_model_translation -= glm::vec3(0.0f, 1.0f, 0.0f)*deltaTime*cylinder_speed;
				} else {
					state = GOING_DOWN;
				}
			break;
		}
	}

};