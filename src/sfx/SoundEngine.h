#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <thread>

#include <irrKlang.h>

#include "ren/Camera.h"

struct SoundEngine {
public:
	/* constructor */
	SoundEngine(Camera* listener);

	/* plays a 2d sound from the filepath */
	void Play2D(const char* filepath);
	/* plays a 3d sound from the filepath */
	void Play3D(const char* filepath);
private:
	irrklang::ISoundEngine* m_engine;
	Camera* m_listener;
};