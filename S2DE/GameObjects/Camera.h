#pragma once
#include "Base/Main/Common.h"
#include "GameObjects/GameObject.h"

#define S2DE_MAIN_CAMERA_NAME "main_camera"

namespace S2DE
{
	enum class S2DE_API CameraProjectionMode
	{
		Perspective,
		Ortho,
	};

	class S2DE_API Camera : public GameObject
	{
	public:
		Camera();
		~Camera();


		void						UpdateControl();
		void						UpdateMouseControl();
		void						Walk(float side);
		void						Fly(float side);
		void						Strafe(float side);
		void						Move(float Right, float Forward);
		void						SetMode(CameraProjectionMode mode);
		void						SetZoom(float zoom);
		
		float						GetFov() const { return m_Fov; }
		CameraProjectionMode		GetMode() const { return m_mode; }
		float						GetZoom() const { return m_Zoom; };
		XMatrix&					GetViewMatrix() { return m_viewMatrix; }
		XMatrix&					GetProjectionMatrix() { return m_projectionMatrix; }

	private:
		XMatrix						m_projectionMatrix;
		XMatrix						m_rotationMatrix;
		XMatrix						m_viewMatrix;
		float						m_Zoom;
		float						m_Fov;
		CameraProjectionMode		m_mode;
		XVector						m_up;
		XVector						m_right;
		XVector						m_forward;
		XVector						m_target;
		float						m_speed;
		bool						m_block_mouse_control;
		bool						m_block_movement_control;
		float						m_sensitivity;

		virtual void				OnRender() override;
		virtual void				OnUpdate(float DeltaTime) override;
		virtual void				OnUpdateInput() override;
		virtual void				OnDebugRenderGUI() override;
	};

}
