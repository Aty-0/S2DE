#pragma once
#include "Base/Main/Common.h"
#include "GameObjects/Components/Component.h"

namespace S2DE::GameObjects
{
	class GameObject;
}

namespace S2DE::GameObjects::Components
{
	class S2DE_API TransformHelpers
	{
	public:
		// Convert euler angles to quaternion		
		static DirectX::SimpleMath::Quaternion	ToQuaternion(Math::float3 rot);
	};

	class S2DE_API Transform : public Component
	{
	public:
		Transform();
		~Transform();

		// Reset all coordinates
		void Reset();
			 
		// Set position vector for transform 
		void SetPosition(Math::float3 pos);
		void SetPositionX(float x);
		void SetPositionY(float y);
		void SetPositionZ(float z);

		// Set rotation vector for transform 
		// Note: X Roll Y Pitch Z Yaw
		void SetRotation(Math::float3 rot);
		void SetRotationX(float x);
		void SetRotationY(float y);
		void SetRotationZ(float z);
		// Set scale vector for transform 
		void SetScale(Math::float3 scale);
		void SetScaleX(float x);
		void SetScaleY(float y);
		void SetScaleZ(float z);
		void SetParent(GameObject* go);
		void SetScaleFactor(Math::float3 _scaleFactor);

		// Calculate world matrix for 3D object
		[[nodiscard]] inline Math::float4x4 UpdateTransformation();
		// Calculate world matrix for 2D object
		[[nodiscard]] inline Math::float4x4 UpdateTransformation2D();
		// is current GameObject is child for setted GameObject
		[[nodiscard]] inline bool isChildOf(GameObject* go) const;
		// Get current Transform matrix 
		[[nodiscard]] inline Math::float4x4& GetWorldMatrix();
		[[nodiscard]] inline Math::float3 GetPosition()   const;
		[[nodiscard]] inline Math::float3 GetRotation()   const;
		[[nodiscard]] inline Math::float3 GetScale()      const;
		[[nodiscard]] inline Math::float3 GetScaleFactor() const;
		[[nodiscard]] inline GameObject* GetChild() const;
		[[nodiscard]] inline GameObject* GetParent() const;

	protected:
		void SetChild(GameObject* go);

	public:
		Core::Utils::Callback<Core::Utils::DefaultCallbackFunction> onPositionChanged;
		Core::Utils::Callback<Core::Utils::DefaultCallbackFunction> onRotationChanged;
		Core::Utils::Callback<Core::Utils::DefaultCallbackFunction> onScaleChanged;
	protected:
		void RunPositionChangedCallbacks();
		void RunRotationChangedCallbacks();
		void RunScaleChangedCallbacks();

	private:
		Math::float3 m_position;
		Math::float3 m_rotation;
		Math::float3 m_scale;
		Math::float3 m_scaleFactor;
		Math::float4x4 m_worldMatrix;
		GameObject* m_parent;
		GameObject* m_child;

	};
}

