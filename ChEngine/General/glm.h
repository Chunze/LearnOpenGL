#ifndef GLM_H
#define GLM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#define SMALL_NUMBER		1e-4f
#define KINDA_SMALL_NUMBER	1e-6f
#define REALLY_SMALL_NUMBER 1e-8f
#define PI					3.14159265359f

typedef glm::vec3 vec3;
typedef glm::vec4 vec4;
typedef glm::quat quat;
typedef glm::mat4 mat4;
typedef glm::mat3 mat3;
#endif
