// Created by ari on 3/7/26.

#pragma once

#include "Basic.h"

class Camera {
public:
	static Camera& getInstance();
	void update(float dt);
	void shake(float duration, float intensity);
	Basic::Mat4 transform();
private:
	Basic::Vec2 pos{};
	float shakeDuration  = 0.0f;
	float shakeIntensity = 0.0f;
	Camera() = default;
};