#include "Base/Main/Common.h"
#include "GameObjects/GameObject.h"

//If we need to get main camera in scene 
#define S2DE_MAIN_CAMERA_NAME "_MainCamera"

namespace S2DE::GameObjects
{
	class Camera : public GameObject
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

		inline DirectX::SimpleMath::Matrix		GetViewMatrix() { return m_viewMatrix; }
		inline DirectX::SimpleMath::Matrix		GetProjectionMatrix() { return m_projectionMatrix; }
		inline DirectX::SimpleMath::Matrix		GetOrthoMatrix() { return m_ortho_Matrix; }
		
		inline CameraProjectionMode				GetProjectionMode() const { return m_mode;  }
		inline float							GetSensitivity() const { return m_sensitivity;  }
		inline float							GetSpeed() const { return m_speed;  }
		inline float							GetZoom() const { return m_zoom; }
		inline float							GetzNear() const { return m_zNear; }
		inline float							GetzFar() const { return m_zFar; }
		inline float							GetFov() const { return m_fov; }
		inline float							GetSpeedBoost() const { return m_speedBoost; }

		void									SetFov(float fov);
		void									SetSpeed(float speed);
		void									SetZoom(float zoom);
		void									SetSensitivity(float sensitivity);
		void									SetProjectionMode(CameraProjectionMode mode);
	protected:
		virtual void							OnRender() override;
		virtual void							OnUpdate(float DeltaTime) override;
		virtual void							OnUpdateInput() override;
		virtual void							OnRenderImGUI() override;
		virtual void							OnCreate() override;

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