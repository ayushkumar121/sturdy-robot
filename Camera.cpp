// Created by ari on 3/7/26.

#include "Camera.h"

#include <cstdlib>

Camera& Camera::getInstance() {
	static Camera instance;
	return instance;
}

void Camera::update(float dt) {
	if (shakeDuration > 0.0f) {
        float intensity = shakeDuration * shakeIntensity;
        pos.x = ((rand() % 200 - 100) / 100.0f) * intensity;
        pos.y = ((rand() % 200 - 100) / 100.0f) * intensity;
        shakeDuration -= dt;
    } else {
		pos.x = 0.0f;
		pos.y = 0.0f;
	}
}

void Camera::shake(float duration, float intensity) {
	shakeDuration = duration;
	shakeIntensity = intensity;
}

Basic::Mat4 Camera::transform() {
	return Basic::Mat4::identity() * Basic::Mat4::translate(pos.x, pos.y, 0.0f);
}