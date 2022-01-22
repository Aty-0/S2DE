#include "Camera.h"

#include "Base/Engine.h"
#include "Base/GameWindow.h"
#include "Base/InputManager.h"

#define CAMERA_DEFAULT_ZOOM 0.009f
#define CAMERA_DEFAULT_SPEED 20.0f
#define CAMERA_DEFAULT_ZNEAR 0.01f
#define CAMERA_DEFAULT_ZFAR 1000.0f
#define CAMERA_DEFAULT_ORTHO_ZOOM 0.15f

namespace S2DE::GameObjects
{
	Camera::Camera() : 
		m_projectionMatrix(DirectX::XMMatrixIdentity()),
		m_viewMatrix(DirectX::XMMatrixIdentity()),
		m_ortho_Matrix(DirectX::XMMatrixIdentity()),
		m_speed(CAMERA_DEFAULT_SPEED),
		m_zoom(CAMERA_DEFAULT_ZOOM),
		m_zNear(CAMERA_DEFAULT_ZNEAR),
		m_zFar(CAMERA_DEFAULT_ZFAR),
		m_mode(CameraProjectionMode::Orthographics)
	{
		SetPosition_Z(0.0f);
	}

	Camera::~Camera()
	{

	}

	void Camera::UpdateProjectionMatrix()
	{		
		switch (m_mode)
		{
		case S2DE::GameObjects::Camera::Perspective:
			//TODO
			m_projectionMatrix = DirectX::XMMatrixIdentity();
			break;
		case S2DE::GameObjects::Camera::Orthographics:
		m_projectionMatrix = DirectX::XMMatrixOrthographicOffCenterLH(
			-(float)Core::Engine::GetGameWindow()->GetWidth() * m_zoom,
			(float)Core::Engine::GetGameWindow()->GetWidth() * m_zoom, 
			-(float)Core::Engine::GetGameWindow()->GetHeight() * m_zoom,
			(float)Core::Engine::GetGameWindow()->GetHeight() * m_zoom, 
			m_zNear, 
			m_zFar);
			break;
		}
		m_ortho_Matrix = DirectX::XMMatrixOrthographicLH((float)Core::Engine::GetGameWindow()->GetWidth() * 
			CAMERA_DEFAULT_ORTHO_ZOOM, (float)Core::Engine::GetGameWindow()->GetHeight() * CAMERA_DEFAULT_ORTHO_ZOOM, m_zNear, m_zFar);
	}

	void Camera::OnRender()
	{	 
		UpdateProjectionMatrix();

		Math::XVector Eye = DirectX::XMVectorSet(GetPosition().x, GetPosition().y, -1.0f, 0.0f);
		Math::XVector LookAt = DirectX::XMVectorSet(GetPosition().x, GetPosition().y, 0.0f, 0.0f);
		Math::XVector Up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		m_viewMatrix = DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtLH(Eye, LookAt, Up));
	}	 

		 
	void Camera::OnUpdate(float DeltaTime)
	{	 
		 
	}	 
		 
	void Camera::OnUpdateInput()
	{	 
		if (Core::Engine::GetInputManager()->IsKeyDown(Core::Other::KeyCode::KEY_W))
		{
			Fly(1.0f);
		}

		if (Core::Engine::GetInputManager()->IsKeyDown(Core::Other::KeyCode::KEY_S))
		{
			Fly(-1.0f);
		}

		if (Core::Engine::GetInputManager()->IsKeyDown(Core::Other::KeyCode::KEY_D))
		{
			Strafe(1.0f);
		}

		if (Core::Engine::GetInputManager()->IsKeyDown(Core::Other::KeyCode::KEY_A))
		{
			Strafe(-1.0f);
		}
	}	

	void Camera::Strafe(float side)
	{
		SetPosition_X(GetPosition().x + side * m_speed * Core::Engine::GetGameTime().GetDeltaTime());
	}

	void Camera::Fly(float side)
	{
		SetPosition_Y(GetPosition().y + side * m_speed * Core::Engine::GetGameTime().GetDeltaTime());
	}

	void Camera::Walk(float side)
	{
		SetPosition_Z(GetPosition().z + side * m_speed * Core::Engine::GetGameTime().GetDeltaTime());
	}

	void Camera::OnRenderImGUI()
	{
		ImGui::Begin(GetName().c_str(), nullptr);

		ImGui::Text("Position: x:%f y:%f z:%f", GetPosition().x, GetPosition().y, GetPosition().z);
		ImGui::Text("Rotation: x:%f y:%f z:%f", GetRotation().x, GetRotation().y, GetRotation().z);

		ImGui::Text("ZNear %f", m_zNear);
		ImGui::Text("zFar %f", m_zFar);

		ImGui::Separator();
		ImGui::SliderFloat("ZNear", &m_zNear, 0.1f, 1000.0f, "", 1.0f);
		ImGui::SliderFloat("zFar", &m_zFar, 0.1f, 1000.0f, "", 1.0f);

		ImGui::Text("Zoom %f", m_zoom);
		ImGui::Text("Speed %f", m_speed);
		ImGui::Separator();

		ImGui::SliderFloat("Zoom", &m_zoom, 0.001f, 1.0f, "", 1.0f);
		ImGui::SliderFloat("Speed", &m_speed, 0.1f, 45.0f, "", 1.0f);
		ImGui::Separator();

		if (ImGui::Button("Reset"))
		{
			m_zoom = CAMERA_DEFAULT_ZOOM;
			m_speed = CAMERA_DEFAULT_SPEED;
			m_zNear = CAMERA_DEFAULT_ZNEAR;
			m_zFar = CAMERA_DEFAULT_ZFAR;
			
			Reset();
			SetPosition_Z(-1.0f);
		}

		if (ImGui::Button("Change camera mode\nto orthographics"))
			m_mode = CameraProjectionMode::Orthographics;
		if (ImGui::Button("Change camera mode\nto perspective"))
			m_mode = CameraProjectionMode::Perspective;


		ImGui::End();
	}
}