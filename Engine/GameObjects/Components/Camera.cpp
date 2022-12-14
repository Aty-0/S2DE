#include "Camera.h"

#include "Base/Engine.h"
#include "Base/GameWindow.h"
#include "Base/InputManager.h"

#include "GameObjects/Base/GameObject.h"

#define CAMERA_DEFAULT_FOV 100.0f
#define CAMERA_DEFAULT_ZOOM 0.009f
#define CAMERA_DEFAULT_SPEED 20.0f
#define CAMERA_DEFAULT_ZNEAR 0.01f
#define CAMERA_DEFAULT_ZFAR 1000.0f
#define CAMERA_DEFAULT_ORTHO_ZOOM 0.15f
#define CAMERA_DEFAULT_SENSITIVITY 2.7f 
#define CAMERA_MAX_SPEED_BOOST 10.0f 
#define CAMERA_MIN_SPEED_BOOST 0.001f

namespace S2DE::GameObjects::Components
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
		auto transform = GetOwner()->GetTransform();

		switch (m_mode)
		{
		case Camera::CameraProjectionMode::Orthographics:
			m_projectionMatrix = DirectX::SimpleMath::Matrix::CreateOrthographic(
				(float)Core::Engine::GetGameWindow()->GetWidth() * m_zoom,
				(float)Core::Engine::GetGameWindow()->GetHeight() * m_zoom,
				m_zNear, m_zFar);

			m_target = DirectX::SimpleMath::Vector3(transform->GetPosition().x, transform->GetPosition().y, -1);

			m_viewMatrix = DirectX::SimpleMath::Matrix::CreateLookAt(transform->GetPosition(), m_target, DirectX::SimpleMath::Vector3::UnitY);
			break;
		case Camera::CameraProjectionMode::Perspective:
			m_projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView((m_fov / 360.0f) * DirectX::XM_2PI,
				(float)Core::Engine::GetGameWindow()->GetWidth() / (float)Core::Engine::GetGameWindow()->GetHeight(), m_zNear, m_zFar);

			m_rotationMatrix = DirectX::SimpleMath::Matrix::CreateFromYawPitchRoll(-transform->GetRotation().z, -transform->GetRotation().y, 0);
			m_target = DirectX::SimpleMath::Vector3::TransformNormal(DirectX::SimpleMath::Vector3::Forward, m_rotationMatrix);
			m_right = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Right, m_rotationMatrix);
			m_forward = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Forward, m_rotationMatrix);

			m_up.Cross(m_forward, m_up);
			m_up.Cross(m_right, m_up);


			m_target = transform->GetPosition() + m_target;

			m_viewMatrix = DirectX::SimpleMath::Matrix::CreateLookAt(transform->GetPosition(), m_target, DirectX::SimpleMath::Vector3::Up);
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
		auto transform = GetOwner()->GetTransform();
		auto position = transform->GetPosition();

		switch (m_mode)
		{
		case Camera::CameraProjectionMode::Perspective:
			if (Core::Engine::isEditor())
			{
				if (Core::Engine::GetInputManager()->IsKeyDown(Core::Other::KeyCode::KEY_LEFT_SHIFT))
				{
					transform->SetRotation_Z(transform->GetRotation().z + Core::Engine::GetInputManager()->GetMousePositionRelative().x * m_sensitivity * Core::Engine::GetGameTime().GetDeltaTime());
					transform->SetRotation_Y(transform->GetRotation().y + Core::Engine::GetInputManager()->GetMousePositionRelative().y * m_sensitivity * Core::Engine::GetGameTime().GetDeltaTime());
					Core::Engine::GetGameWindow()->SetMouseVisible(false);
				}
				else
				{
					Core::Engine::GetGameWindow()->SetMouseVisible(true);
				}

				if (Core::Engine::GetInputManager()->IsMouseWheelTurnsForward())
				{
					if (m_speedBoost < CAMERA_MAX_SPEED_BOOST)
					{
						if (m_speedBoost <= 0.1f)
						{
							m_speedBoost += 0.001f;
						}
						else
						{
							m_speedBoost += 0.1f;
						}
					}
				}

				if (Core::Engine::GetInputManager()->IsMouseWheelTurnsBackward())
				{
					if (m_speedBoost > CAMERA_MIN_SPEED_BOOST)
					{
						if (m_speedBoost <= 0.1f)
						{
							m_speedBoost -= 0.001f;
						}
						else
						{
							m_speedBoost -= 0.1f;
						}
					}
				}

			}
			else
			{
				transform->SetRotation_Z(transform->GetRotation().z + Core::Engine::GetInputManager()->GetMousePositionRelative().x * m_sensitivity * Core::Engine::GetGameTime().GetDeltaTime());
				transform->SetRotation_Y(transform->GetRotation().y + Core::Engine::GetInputManager()->GetMousePositionRelative().y * m_sensitivity * Core::Engine::GetGameTime().GetDeltaTime());
			}

			
			if (Core::Engine::GetInputManager()->IsKeyDown(Core::Other::KeyCode::KEY_W))
			{
				position += m_speed * m_speedBoost * Core::Engine::GetGameTime().GetDeltaTime() * m_forward;
				transform->SetPosition(position);
			}

			if (Core::Engine::GetInputManager()->IsKeyDown(Core::Other::KeyCode::KEY_S))
			{
				position -= m_speed * m_speedBoost * Core::Engine::GetGameTime().GetDeltaTime() * m_forward;
				transform->SetPosition(position);
			}

			if (Core::Engine::GetInputManager()->IsKeyDown(Core::Other::KeyCode::KEY_D))
			{
				position += m_speed * m_speedBoost * Core::Engine::GetGameTime().GetDeltaTime() * m_right;
				transform->SetPosition(position);
			}

			if (Core::Engine::GetInputManager()->IsKeyDown(Core::Other::KeyCode::KEY_A))
			{
				position -= m_speed * m_speedBoost * Core::Engine::GetGameTime().GetDeltaTime() * m_right;
				transform->SetPosition(position);
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
		case Camera::CameraProjectionMode::Orthographics:
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
		auto transform = GetOwner()->GetTransform();
		transform->SetPosition_X(transform->GetPosition().x + side * m_speedBoost * m_speed * Core::Engine::GetGameTime().GetDeltaTime());
	}

	void Camera::Fly(float side)
	{
		auto transform = GetOwner()->GetTransform();
		transform->SetPosition_Y(transform->GetPosition().y + side * m_speedBoost * m_speed * Core::Engine::GetGameTime().GetDeltaTime());
	}

	void Camera::Walk(float side)
	{
		auto transform = GetOwner()->GetTransform();
		transform->SetPosition_Z(transform->GetPosition().z + side * m_speedBoost * m_speed * Core::Engine::GetGameTime().GetDeltaTime());
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


	inline DirectX::SimpleMath::Matrix& Camera::GetViewMatrix()
	{
		return m_viewMatrix;
	}

	inline DirectX::SimpleMath::Matrix& Camera::GetProjectionMatrix()
	{
		return m_projectionMatrix;
	}

	inline DirectX::SimpleMath::Matrix& Camera::GetOrthoMatrix()
	{
		return m_ortho_Matrix;
	}

	inline Camera::CameraProjectionMode	Camera::GetProjectionMode() const
	{
		return m_mode;
	}

	inline float Camera::GetSensitivity() const
	{
		return m_sensitivity;
	}

	inline float Camera::GetSpeed() const
	{
		return m_speed;
	}

	inline float Camera::GetZoom() const
	{
		return m_zoom;
	}

	inline float Camera::GetzNear() const
	{
		return m_zNear;
	}

	inline float Camera::GetzFar() const
	{
		return m_zFar;
	}

	inline float Camera::GetFov() const
	{
		return m_fov;
	}

	inline float Camera::GetSpeedBoost() const
	{
		return m_speedBoost;
	}
}