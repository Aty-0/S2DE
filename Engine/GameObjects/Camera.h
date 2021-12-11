#pragma once
#include "Base/Main/Common.h"
#include "GameObjects/GameObject.h"

#define S2DE_MAIN_CAMERA_NAME "MainCamera"

namespace S2DE::GameObjects
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
		void						DisableMouseControl(bool block);
		void						DisableControl(bool block);


		inline float				GetFov() const { return m_Fov; }
		inline CameraProjectionMode	GetMode() const { return m_mode; }
		inline float				GetZoom() const { return m_Zoom; };
		inline Math::XMatrix&		GetViewMatrix() { return m_viewMatrix; }
		inline Math::XMatrix&		GetProjectionMatrix() { return m_projectionMatrix; }
		inline float				GetZNear() const { return m_zNear; }
		inline float				GetZFar() const { return m_zFar; }

	private:
		Math::XMatrix				m_orthoMatrix;
		Math::XMatrix				m_projectionMatrix;
		Math::XMatrix				m_rotationMatrix;
		Math::XMatrix				m_viewMatrix;
		float						m_Zoom;
		float						m_Fov;
		CameraProjectionMode		m_mode;
		Math::XVector						m_up;
		Math::XVector						m_right;
		Math::XVector						m_forward;
		Math::XVector						m_target;
		float						m_speed;
		bool						m_block_mouse_control;
		bool						m_block_movement_control;
		float						m_sensitivity;
		float					    m_zNear;
		float					    m_zFar;

	protected:
		virtual void				OnRender() override;
		virtual void				OnUpdate(float DeltaTime) override;
		virtual void				OnUpdateInput() override;
		virtual void				OnDebugRenderGUI() override;
	};

}
