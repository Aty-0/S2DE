#pragma once
#include "GameObjects/Components/Component.h"

#include "Base/Main/Common.h"

//If we need to get main camera in scene 
#define S2DE_MAIN_CAMERA_NAME "_MainCamera"

namespace S2DE::GameObjects::Components
{
	// TODO: Make EditorCamera 

	class S2DE_API Camera : public Component
	{
	public:
		enum CameraProjectionMode
		{
			Perspective,
			Orthographics,
		};

		Camera();
		~Camera();

		virtual void							Walk(float side);
		virtual void							Fly(float side);
		virtual void							Strafe(float side);


		void									SetFov(float fov);
		void									SetSpeed(float speed);
		void									SetZoom(float zoom);
		void									SetSensitivity(float sensitivity);

		void									SetProjectionMode(CameraProjectionMode mode);

		[[nodiscard]] inline DirectX::SimpleMath::Matrix&		GetViewMatrix();
		[[nodiscard]] inline DirectX::SimpleMath::Matrix&		GetProjectionMatrix();
		[[nodiscard]] inline DirectX::SimpleMath::Matrix&		GetOrthoMatrix();
		[[nodiscard]] inline CameraProjectionMode				GetProjectionMode() const;
		[[nodiscard]] inline float								GetSensitivity() const;
		[[nodiscard]] inline float								GetSpeed() const;
		[[nodiscard]] inline float								GetZoom() const;
		[[nodiscard]] inline float								GetzNear() const;
		[[nodiscard]] inline float								GetzFar() const;
		[[nodiscard]] inline float								GetFov() const;
		[[nodiscard]] inline float								GetSpeedBoost() const;
	protected:
		virtual void							OnRender() override;
		virtual void							OnUpdate(float DeltaTime) override;
		virtual void							UpdateCameraControls();
	private:
		DirectX::SimpleMath::Matrix				m_viewMatrix;
		DirectX::SimpleMath::Matrix				m_projectionMatrix;
		float									m_speedBoost;
		float									m_fov;
		float									m_speed;
		float									m_zoom;
		float									m_zNear;
		float									m_zFar;
		float									m_sensitivity;
		CameraProjectionMode					m_mode;
		DirectX::SimpleMath::Matrix				m_rotationMatrix;
		DirectX::SimpleMath::Vector3			m_target;
		DirectX::SimpleMath::Vector3			m_right;
		DirectX::SimpleMath::Vector3			m_up;
		DirectX::SimpleMath::Vector3			m_forward;
		DirectX::SimpleMath::Matrix				m_ortho_Matrix;
	};
}