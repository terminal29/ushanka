#include "components/Camera.h"
#include <glm/ext.hpp>
#include "platform/Renderer.h"

using namespace U;

Camera::Camera(std::shared_ptr<Renderer> renderer) noexcept :
	_renderer(renderer)
{
	recalc();
}

void Camera::recalc() noexcept {
	const auto windowSize = _renderer->getWindowSize();
	_direction = glm::normalize(_position - _target);
	_right = glm::normalize(glm::cross(UP_VECTOR, _direction));
	_up = glm::cross(_direction, _right);
	_viewMatrix = glm::lookAt(_position, _target, UP_VECTOR);
	_projectionMatrix = glm::perspective(glm::radians(45.0f), (float)windowSize.width / windowSize.height, 0.1f, 100.0f);
}

void Camera::setTarget(const glm::fvec3& target) noexcept
{
	_target = target;
	recalc();
}

void Camera::setPosition(const glm::fvec3& position) noexcept
{
	_position = position;
	recalc();
}

const glm::fvec3& Camera::getPosition() const noexcept
{
	return _position;
}

const glm::fvec3& Camera::getTarget() const noexcept
{
	return _target;
}

const glm::fvec3& Camera::getDirection() const noexcept
{
	return _direction;
}

const glm::fmat4& Camera::getViewMatrix() const noexcept
{
	return _viewMatrix;
}

const glm::fmat4& Camera::getProjectionMatrix() const noexcept
{
	return _projectionMatrix;
}

