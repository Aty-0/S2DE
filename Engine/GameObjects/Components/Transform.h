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
		static DirectX::SimpleMath::Quaternion	ToQuaternion(DirectX::SimpleMath::Vector3 rot);
	};

	class S2DE_API Transform : public Component
	{
	public:
		Transform();
		~Transform();

		// Reset all object coordinates
		void									Reset();

		void									SetPosition(DirectX::SimpleMath::Vector3 pos);
		void									SetPosition_X(float x);
		void									SetPosition_Y(float y);
		void									SetPosition_Z(float z);
		void									SetRotation(DirectX::SimpleMath::Vector3 rot); // X Roll Y Pitch Z Yaw
		void									SetRotation_X(float x);
		void									SetRotation_Y(float y);
		void									SetRotation_Z(float z);
		void									SetScale(DirectX::SimpleMath::Vector3 scale);
		void									SetScale_X(float x);
		void									SetScale_Y(float y);
		void									SetScale_Z(float z);


		inline DirectX::SimpleMath::Vector3		GetPosition()   const { return m_position; }
		inline DirectX::SimpleMath::Vector3		GetRotation()   const { return m_rotation; }
		inline DirectX::SimpleMath::Vector3		GetScale()      const { return m_scale; }

		// Get object world matrix
		inline DirectX::SimpleMath::Matrix&		GetWorldMatrix() { return m_worldMatrix; }

		DirectX::SimpleMath::Matrix				UpdateTransformation();
		DirectX::SimpleMath::Matrix				UpdateTransformation2D();

		void									SetParent(GameObject* go);

		inline bool								isChildOf(GameObject* go) const;

		inline GameObject*						GetChild() const { return m_child; }
		inline GameObject*						GetParent() const { return m_parent; }

	private:
		DirectX::SimpleMath::Vector3			m_position;
		DirectX::SimpleMath::Vector3			m_rotation;
		DirectX::SimpleMath::Vector3			m_scale;


		DirectX::SimpleMath::Matrix				m_worldMatrix;

		GameObject*								m_parent;
		GameObject*								m_child;

		void									RunOnPositionChangedCallbacks();
		void									RunOnRotationChangedCallbacks();
		void									RunOnScaleChangedCallbacks();
	public:
		std::vector<std::function<void()>>		onPositionChanged;
		std::vector<std::function<void()>>		onRotationChanged;
		std::vector<std::function<void()>>		onScaleChanged;
	};
}