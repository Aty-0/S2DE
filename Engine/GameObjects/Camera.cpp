#include "Camera.h"

#include "Base/Engine.h"
#include "Base/GameWindow.h"
#include "Base/InputManager.h"

#define CAMERA_DEFAULT_FOV 90.0f
#define CAMERA_DEFAULT_ZOOM 0.009f
#define CAMERA_DEFAULT_SPEED 20.0f
#define CAMERA_DEFAULT_ZNEAR 0.01f
#define CAMERA_DEFAULT_ZFAR 1000.0f
#define CAMERA_DEFAULT_ORTHO_ZOOM 0.15f
#define CAMERA_DEFAULT_SENSITIVITY 2.7f 
#define CAMERA_MAX_SPEED_BOOST 10.0f 
#define CAMERA_MIN_SPEED_BOOST 0.001f

namespace S2DE::GameObjects
{
	Camera::Camera() :
		m_projectionMatrix(DirectX::SimpleMath::Matrix::Identity),
		m_viewMatrix(DirectX::SimpleMath::Matrix::Identity),
		m_ortho_Matrix(DirectX::SimpleMath::Matrix::Identity),
		m_speed(CAMERA_DEFAULT_SPEED),
		m_zoom(CAMERA_DEFAULT_ZOOM),
		m_zNear(CAMERA_DEFAULT_ZNEAR),
		m_zFar(CAMERA_DEFAULT_ZFAR),
		m_fov(CAMERA_DEFAULT_FOV),
		m_speedBoost(1.0f),
		m_sensitivity(CAMERA_DEFAULT_SENSITIVITY),
		m_mode(CameraProjectionMode::Orthographics)
	{

	}

	Camera::~Camera()
	{

	}

	void Camera::OnRender()
	{
		switch (m_mode)
		{
		case S2DE::GameObjects::Camera::Orthographics:
			m_projectionMatrix = DirectX::SimpleMath::Matrix::CreateOrthographic(
				(float)Core::Engine::GetGameWindow()->GetWidth() * m_zoom,
				(float)Core::Engine::GetGameWindow()->GetHeight() * m_zoom,
				m_zNear, m_zFar);

			m_target = DirectX::SimpleMath::Vector3(m_Position.x, m_Position.y, -1);

			m_viewMatrix = DirectX::SimpleMath::Matrix::CreateLookAt(m_Position, m_target, DirectX::SimpleMath::Vector3::UnitY);
			break;
		case S2DE::GameObjects::Camera::Perspective:
			m_projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView((m_fov / 360.0f) * DirectX::XM_2PI,
				(float)Core::Engine::GetGameWindow()->GetWidth() / (float)Core::Engine::GetGameWindow()->GetHeight(), m_zNear, m_zFar);

			m_rotationMatrix = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(-m_Rotation.z, -m_Rotation.y, 0);
			m_target = DirectX::SimpleMath::Vector3::TransformNormal(DirectX::SimpleMath::Vector3::Forward, m_rotationMatrix);
			m_right = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Right, m_rotationMatrix);
			m_forward = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Forward, m_rotationMatrix);

			m_up.Cross(m_forward, m_up);
			m_up.Cross(m_right, m_up);


			m_target = m_Position + m_target;

			m_viewMatrix = DirectX::SimpleMath::Matrix::CreateLookAt(m_Position, m_target, DirectX::SimpleMath::Vector3::Up);
			break;
		}


		m_ortho_Matrix = DirectX::SimpleMath::Matrix::CreateOrthographic(
			(float)Core::Engine::GetGameWindow()->GetWidth() * CAMERA_DEFAULT_ORTHO_ZOOM,
			(float)Core::Engine::GetGameWindow()->GetHeight() * CAMERA_DEFAULT_ORTHO_ZOOM,
			0.0f, 10.0f);

		m_projectionMatrix.Transpose(m_projectionMatrix);
		m_viewMatrix.Transpose(m_viewMatrix);
	}


	void Camera::OnUpdate(float DeltaTime)
	{
		UpdateCameraControls();
	}

	void Camera::UpdateCameraControls()
	{
		switch (m_mode)
		{
		case S2DE::GameObjects::Camera::Perspective:
			if (Core::Engine::isEditor())
			{
				if (Core::Engine::GetInputManager()->IsKeyDown(Core::Other::KeyCode::KEY_LEFT_SHIFT))
				{
					m_Rotation.z += Core::Engine::GetInputManager()->GetMousePositionRelative().x * m_sensitivity * Core::Engine::GetGameTime().GetDeltaTime();
					m_Rotation.y += Core::Engine::GetInputManager()->GetMousePositionRelative().y * m_sensitivity * Core::Engine::GetGameTime().GetDeltaTime();
					Core::Engine::GetGameWindow()->SetMouseVisible(false);
				}
				else
					Core::Engine::GetGameWindow()->SetMouseVisible(true);


				if (Core::Engine::GetInputManager()->IsMouseWheelTurnsForward())
				{
					if (m_speedBoost < CAMERA_MAX_SPEED_BOOST)
						if (m_speedBoost <= 0.1f)
							m_speedBoost += 0.001f;
						else
							m_speedBoost += 0.1f;
				}

				if (Core::Engine::GetInputManager()->IsMouseWheelTurnsBackward())
				{
					if (m_speedBoost > CAMERA_MIN_SPEED_BOOST)
						if (m_speedBoost <= 0.1f)
							m_speedBoost -= 0.001f;
						else
							m_speedBoost -= 0.1f;
				}

			}
			else
			{
				m_Rotation.z += (float)Core::Engine::GetInputManager()->GetMousePositionRelative().x * m_sensitivity * Core::Engine::GetGameTime().GetDeltaTime();
				m_Rotation.y += (float)Core::Engine::GetInputManager()->GetMousePositionRelative().y * m_sensitivity * Core::Engine::GetGameTime().GetDeltaTime();
			}

			if (Core::Engine::GetInputManager()->IsKeyDown(Core::Other::KeyCode::KEY_W))
			{
				m_Position += m_speed * m_speedBoost * Core::Engine::GetGameTime().GetDeltaTime() * m_forward;
			}

			if (Core::Engine::GetInputManager()->IsKeyDown(Core::Other::KeyCode::KEY_S))
			{
				m_Position -= m_speed * m_speedBoost * Core::Engine::GetGameTime().GetDeltaTime() * m_forward;
			}

			if (Core::Engine::GetInputManager()->IsKeyDown(Core::Other::KeyCode::KEY_D))
			{
				m_Position += m_speed * m_speedBoost * Core::Engine::GetGameTime().GetDeltaTime() * m_right;
			}

			if (Core::Engine::GetInputManager()->IsKeyDown(Core::Other::KeyCode::KEY_A))
			{
				m_Position -= m_speed * m_speedBoost * Core::Engine::GetGameTime().GetDeltaTime() * m_right;
			}

			if (Core::Engine::GetInputManager()->IsKeyDown(Core::Other::KeyCode::KEY_SPACE))
			{
				Fly(1.0f);
			}

			if (Core::Engine::GetInputManager()->IsKeyDown(Core::Other::KeyCode::KEY_C))
			{
				Fly(-1.0f);
			}

			break;
		case S2DE::GameObjects::Camera::Orthographics:
			if (Core::Engine::isEditor())
			{
				if (Core::Engine::GetInputManager()->IsMouseWheelTurnsForward())
				{
					if (m_speedBoost < CAMERA_MAX_SPEED_BOOST)
						if (m_speedBoost <= 0.1f)
							m_speedBoost += 0.001f;
						else
							m_speedBoost += 0.1f;
				}

				if (Core::Engine::GetInputManager()->IsMouseWheelTurnsBackward())
				{
					if (m_speedBoost > CAMERA_MIN_SPEED_BOOST)
						if (m_speedBoost <= 0.1f)
							m_speedBoost -= 0.001f;
						else
							m_speedBoost -= 0.1f;
				}
			}

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
			break;
		}

	}

	void Camera::Strafe(float side)
	{
		SetPosition_X(GetPosition().x + side * m_speedBoost * m_speed * Core::Engine::GetGameTime().GetDeltaTime());
	}

	void Camera::Fly(float side)
	{
		SetPosition_Y(GetPosition().y + side * m_speedBoost * m_speed * Core::Engine::GetGameTime().GetDeltaTime());
	}

	void Camera::Walk(float side)
	{
		SetPosition_Z(GetPosition().z + side * m_speedBoost * m_speed * Core::Engine::GetGameTime().GetDeltaTime());
	}

	void Camera::OnCreate()
	{

	}

	void Camera::SetProjectionMode(CameraProjectionMode mode)
	{
		m_mode = mode;
	}

	void Camera::SetFov(float fov)
	{
		m_fov = fov;
	}

	void Camera::SetSpeed(float speed)
	{
		m_speed = speed;
	}
	
	void Camera::SetZoom(float zoom)
	{
		m_zoom = zoom;
	}

	void Camera::SetSensitivity(float sensitivity)
	{
		m_sensitivity = sensitivity;
	}

	void Camera::OnRenderImGUI()
	{
		//ImGui::Begin(GetName().c_str(), nullptr);
		//
		//ImGui::Text("Position: x:%f y:%f z:%f", GetPosition().x, GetPosition().y, GetPosition().z);
		//ImGui::Text("Rotation: x:%f y:%f z:%f", GetRotation().x, GetRotation().y, GetRotation().z);
		//ImGui::Separator();
		//
		//ImGui::Text("ZNear %f", m_zNear);
		//ImGui::Text("zFar %f", m_zFar);
		//
		//ImGui::Separator();
		//ImGui::SliderFloat("ZNear", &m_zNear, 0.1f, 1000.0f, "", 1.0f);
		//ImGui::SliderFloat("zFar", &m_zFar, 0.1f, 1000.0f, "", 1.0f);
		//
		//ImGui::Text("Zoom %f", m_zoom);
		//ImGui::Text("Speed %f", m_speed);
		//ImGui::Separator();
		//
		//ImGui::SliderFloat("FOV", &m_fov, 0.0f, 180.0f, "", 1.0f);
		//ImGui::SliderFloat("Zoom", &m_zoom, 0.0001f, 0.13f, "", 1.0f);
		//ImGui::SliderFloat("Speed", &m_speed, 0.1f, 45.0f, "", 1.0f);
		//ImGui::SliderFloat("Sensitivity", &m_sensitivity, 0.01f, 2.0f, "", 1.0f);
		//ImGui::Separator();
		//
		//if (ImGui::Button("Reset"))
		//{
		//	m_zoom = CAMERA_DEFAULT_ZOOM;
		//	m_speed = CAMERA_DEFAULT_SPEED;
		//	m_zNear = CAMERA_DEFAULT_ZNEAR;
		//	m_zFar = CAMERA_DEFAULT_ZFAR;
		//	m_fov = CAMERA_DEFAULT_FOV;
		//
		//	Reset();
		//	SetPosition_Z(1.0f);
		//}
		//
		//if (ImGui::Button("Orthographics"))
		//{
		//	m_mode = CameraProjectionMode::Orthographics;
		//	m_zNear = CAMERA_DEFAULT_ZNEAR;
		//}
		//if (ImGui::Button("Perspective"))
		//{
		//	m_mode = CameraProjectionMode::Perspective;
		//	m_zNear = 1.0f;
		//}
		//
		//ImGui::End();
	}
}