#pragma once
#include "GameObjects/Components/Component.h"



namespace S2DE::GameObjects::Components
{
	class S2DE_API Camera : public Component
	{
	public:
		Camera();
		~Camera();

		virtual void Walk(float side, float delta);
		virtual void Fly(float side, float delta);
		virtual void Strafe(float side, float delta);

		void SetFov(float fov);
		void SetSpeed(float speed);
		void SetZoom(float zoom);
		void SetSensitivity(float sensitivity);

		[[nodiscard]] inline Math::float4x4& GetViewMatrix();
		[[nodiscard]] inline Math::float4x4& GetProjectionMatrix();
		[[nodiscard]] inline Math::float4x4& GetOrthoMatrix();
		[[nodiscard]] inline float GetSensitivity() const;
		[[nodiscard]] inline float GetSpeed() const;
		[[nodiscard]] inline float GetZoom() const;
		[[nodiscard]] inline float GetzNear() const;
		[[nodiscard]] inline float GetzFar() const;
		[[nodiscard]] inline float GetFov() const;
		[[nodiscard]] inline float GetSpeedBoost() const;

	protected:
		virtual void							OnRender(Render::Renderer* renderer) override;
		virtual void							OnUpdate(float DeltaTime) override;
		virtual void							UpdateCameraControls(float delta);

	public: 
		static constexpr auto EngineCameraName = "_MainCamera";
		static constexpr auto DefaultCameraFOV = 90.0f;
		static constexpr auto DefaultCameraOrthoZoom = 0.009f;
		static constexpr auto DefaultCameraSpeed = 20.0f;
		static constexpr auto DefaultCameraZNear = 0.01f;
		static constexpr auto DefaultCameraZFar = 1000.0f;
		static constexpr auto DefaultCameraUIOrthoZoom = 0.15f;
		static constexpr auto DefaultCameraSensitivity = 34.0f;
		static constexpr auto DefaultEditorSpeedBoost = 1.0f;
		static constexpr auto EditorSpeedBoostMax = 10.0f;
		static constexpr auto EditorSpeedBoostMin = 0.001f;

		enum class ProjectionMode
		{
			Perspective = 0,
			Orthographics,
		};

		void SetProjectionMode(ProjectionMode mode);
		[[nodiscard]] inline ProjectionMode GetProjectionMode() const;
	private:
		float m_speedBoost;
		float m_fov;
		float m_speed;
		float m_zoom;
		float m_zNear;
		float m_zFar;
		float m_sensitivity;
		ProjectionMode m_mode;
		Math::float4x4 m_viewMatrix;
		Math::float4x4 m_projectionMatrix;
		Math::float4x4 m_rotationMatrix;
		Math::float4x4 m_orthoMatrix;
		Math::float3 m_target;
		Math::float3 m_right;
		Math::float3 m_up;
		Math::float3 m_forward;
	};
}