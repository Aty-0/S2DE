#include "Camera.h"

#include "Base/Engine.h"
#include "Base/GameWindow.h"
#include "Base/InputManager.h"

#include "GameObjects/Base/GameObject.h"


namespace S2DE::GameObjects::Components
{
	Camera::Camera() :
		m_projectionMatrix(Math::float4x4::Identity),
		m_viewMatrix(Math::float4x4::Identity),
		m_orthoMatrix(Math::float4x4::Identity),
		m_speed(DefaultCameraSpeed),
		m_zoom(DefaultCameraOrthoZoom),
		m_zNear(DefaultCameraZNear),
		m_zFar(DefaultCameraZFar),
		m_fov(DefaultCameraFOV),
		m_speedBoost(DefaultEditorSpeedBoost),
		m_sensitivity(DefaultCameraSensitivity),
		m_mode(ProjectionMode::Orthographics)
	{

	}

	Camera::~Camera()
	{

	}

	void Camera::OnRender(Render::Renderer* renderer)
	{
		static const auto transform = GetOwner()->GetTransform();
		static const auto gameWindow = Core::GameWindow::GetInstance();

		switch (m_mode)
		{
		case Camera::ProjectionMode::Orthographics:
			m_projectionMatrix = Math::float4x4::CreateOrthographic(
				static_cast<float>(gameWindow->GetWidth())  * m_zoom,
				static_cast<float>(gameWindow->GetHeight()) * m_zoom,
				m_zNear, m_zFar);

			m_target = Math::float3(transform->GetPosition().x, transform->GetPosition().y, 1);

			m_viewMatrix = Math::float4x4::CreateLookAt(transform->GetPosition(), m_target, Math::float3::UnitY);
			break;
		case Camera::ProjectionMode::Perspective:
			m_projectionMatrix = Math::float4x4::CreatePerspectiveFieldOfView((m_fov / 360.0f) * DirectX::XM_2PI,
				static_cast<float>(gameWindow->GetWidth() / gameWindow->GetHeight()), m_zNear, m_zFar);

			// Create the rotation matrix 
			const auto x = transform->GetRotation().x;
			const auto y = transform->GetRotation().y;
			const auto z = transform->GetRotation().z;
			// Just simple check on Y angle to avoid breaking view
			if (y > 85.0f)
			{
				transform->SetRotationY(85.0f);
			}
			else if (y < -85.0f)
			{
				transform->SetRotationY(-85.0f);
			}

			m_rotationMatrix = Math::float4x4::CreateFromYawPitchRoll(
				-DirectX::XMConvertToRadians(z), 
				-DirectX::XMConvertToRadians(y),
				-DirectX::XMConvertToRadians(x));
		
			// Use transform to make target vector from rotation matrix 
			m_target = Math::float3::TransformNormal(Math::float3::Forward, m_rotationMatrix);


			// Calculate movement vectors
			m_right = Math::float3::Transform(Math::float3::Right, m_rotationMatrix);
			m_forward = Math::float3::Transform(Math::float3::Forward, m_rotationMatrix);

			m_up.Cross(m_forward, m_up);
			m_up.Cross(m_right, m_up);

			m_target = transform->GetPosition() + m_target;

			m_viewMatrix = Math::float4x4::CreateLookAt(transform->GetPosition(), m_target, Math::float3::Up);
			break;
		}


		m_orthoMatrix = Math::float4x4::CreateOrthographic(
			static_cast<float>(gameWindow->GetWidth()) * DefaultCameraUIOrthoZoom,
			static_cast<float>(gameWindow->GetHeight()) * DefaultCameraUIOrthoZoom,
			0.0f, 10.0f);

		m_projectionMatrix.Transpose(m_projectionMatrix);
		m_viewMatrix.Transpose(m_viewMatrix);
	}


	void Camera::OnUpdate(float DeltaTime)
	{
		UpdateCameraControls(DeltaTime);
	}

	void Camera::UpdateCameraControls(float delta)
	{
		static const auto inputManager = Core::InputManager::GetInstance();

		const auto transform = GetOwner()->GetTransform();
		auto position = transform->GetPosition();

		switch (m_mode)
		{
		case Camera::ProjectionMode::Perspective:
			if (Core::Engine::isEditor())
			{
				static const auto window = Core::GameWindow::GetInstance();

				if (inputManager->IsKeyDown(Core::Other::KeyCode::KEY_LEFT_SHIFT))
				{
					transform->SetRotationZ(transform->GetRotation().z + inputManager->GetMousePositionRelative().x * m_sensitivity * delta);
					transform->SetRotationY(transform->GetRotation().y + inputManager->GetMousePositionRelative().y * m_sensitivity * delta);
					window->SetMouseVisible(false);
				}
				else
				{
					window->SetMouseVisible(true);
				}

				if (inputManager->IsMouseWheelTurnsForward())
				{
					if (m_speedBoost < EditorSpeedBoostMax)
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

				if (inputManager->IsMouseWheelTurnsBackward())
				{
					if (m_speedBoost > EditorSpeedBoostMin)
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
				transform->SetRotationZ(transform->GetRotation().z + inputManager->GetMousePositionRelative().x * m_sensitivity * delta);
				transform->SetRotationY(transform->GetRotation().y + inputManager->GetMousePositionRelative().y * m_sensitivity * delta);
			}

			
			if (inputManager->IsKeyDown(Core::Other::KeyCode::KEY_W))
			{
				position += m_speed * m_speedBoost * delta * m_forward;
				transform->SetPosition(position);
			}

			if (inputManager->IsKeyDown(Core::Other::KeyCode::KEY_S))
			{
				position -= m_speed * m_speedBoost * delta * m_forward;
				transform->SetPosition(position);
			}

			if (inputManager->IsKeyDown(Core::Other::KeyCode::KEY_D))
			{
				position += m_speed * m_speedBoost * delta * m_right;
				transform->SetPosition(position);
			}

			if (inputManager->IsKeyDown(Core::Other::KeyCode::KEY_A))
			{
				position -= m_speed * m_speedBoost * delta * m_right;
				transform->SetPosition(position);
			}

			if (inputManager->IsKeyDown(Core::Other::KeyCode::KEY_SPACE))
			{
				Fly(1.0f, delta);
			}

			if (inputManager->IsKeyDown(Core::Other::KeyCode::KEY_C))
			{
				Fly(-1.0f, delta);
			}

			break;
		case Camera::ProjectionMode::Orthographics:
			if (Core::Engine::isEditor())
			{
				if (inputManager->IsMouseWheelTurnsForward())
				{
					if (m_speedBoost < EditorSpeedBoostMax)
						if (m_speedBoost <= 0.1f)
							m_speedBoost += 0.001f;
						else
							m_speedBoost += 0.1f;
				}

				if (inputManager->IsMouseWheelTurnsBackward())
				{
					if (m_speedBoost > EditorSpeedBoostMin)
						if (m_speedBoost <= 0.1f)
							m_speedBoost -= 0.001f;
						else
							m_speedBoost -= 0.1f;
				}
			}

			if (inputManager->IsKeyDown(Core::Other::KeyCode::KEY_W))
			{
				Fly(1.0f, delta);
			}

			if (inputManager->IsKeyDown(Core::Other::KeyCode::KEY_S))
			{
				Fly(-1.0f, delta);
			}

			if (inputManager->IsKeyDown(Core::Other::KeyCode::KEY_D))
			{
				Strafe(1.0f, delta);
			}

			if (inputManager->IsKeyDown(Core::Other::KeyCode::KEY_A))
			{
				Strafe(-1.0f, delta);
			}
			break;
		}

	}

	void Camera::Strafe(float side, float delta)
	{
		auto transform = GetOwner()->GetTransform();
		transform->SetPositionX(transform->GetPosition().x + side * m_speedBoost * m_speed * delta);
	}

	void Camera::Fly(float side, float delta)
	{
		auto transform = GetOwner()->GetTransform();
		transform->SetPositionY(transform->GetPosition().y + side * m_speedBoost * m_speed * delta);
	}

	void Camera::Walk(float side, float delta)
	{
		auto transform = GetOwner()->GetTransform();
		transform->SetPositionZ(transform->GetPosition().z + side * m_speedBoost * m_speed * delta);
	}

	void Camera::SetProjectionMode(ProjectionMode mode)
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


	inline Math::float4x4& Camera::GetViewMatrix()
	{
		return m_viewMatrix;
	}

	inline Math::float4x4& Camera::GetProjectionMatrix()
	{
		return m_projectionMatrix;
	}

	inline Math::float4x4& Camera::GetOrthoMatrix()
	{
		return m_orthoMatrix;
	}

	inline Camera::ProjectionMode Camera::GetProjectionMode() const
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