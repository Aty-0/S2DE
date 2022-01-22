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

		virtual void		  Walk(float side);
		virtual void		  Fly(float side);
		virtual void		  Strafe(float side);

		inline Math::XMatrix  GetViewMatrix() { return m_viewMatrix; }
		inline Math::XMatrix  GetProjectionMatrix() { return m_projectionMatrix; }
		inline Math::XMatrix  GetOrthoMatrix() { return m_ortho_Matrix; }
		inline Math::Vector3  GetDirection() const { return m_direction; }
		inline float		  GetSpeed() const { return m_speed;  }
		inline float		  GetZoom() const { return m_zoom; }
		inline float		  GetzNear() const { return m_zNear; }
		inline float		  GetzFar() const { return m_zFar; }

	protected:
		virtual void		  OnRender() override;
		virtual void		  OnUpdate(float DeltaTime) override;
		virtual void		  OnUpdateInput() override;
		virtual void		  OnRenderImGUI() override;
							  
		virtual void		  UpdateProjectionMatrix();

	private:
		Math::XMatrix m_viewMatrix;
		Math::XMatrix m_projectionMatrix;
		Math::XMatrix m_ortho_Matrix;
		Math::Vector3 m_direction;
		Math::Vector3 m_offset;
		float		  m_speed;
		float		  m_zoom;
		float		  m_zNear;
		float		  m_zFar;
		CameraProjectionMode m_mode;
	};
}