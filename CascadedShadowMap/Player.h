#pragma once
#include "Camera.h"
#include "BasicShadow.h"

class Player :
	public Camera,
	public BasicShadow
{
public:
	Player();
	~Player();

	void OnInit(float fovY, float aspectRatio, float nearZ, float farZ);
	CameraShaderConstants GetCameraConstants();
	void OnKeyDown(WPARAM wParam);
	void OnKeyUp(WPARAM wParam);

	void Move(float elapsedTime);
private:
	XMFLOAT3 m_position;
	XMFLOAT3 m_linearVelocity;
};