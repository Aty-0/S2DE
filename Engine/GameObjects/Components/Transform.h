#pragma once
#include "Base/Main/Common.h"
#include "GameObjects/Components/Component.h"

namespace S2DE::GameObjects
{
	class GameObject;
}
// TODO: Add scale factor 
//		 Will fix sprite tile scaling 

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
		void									SetParent(GameObject* go);
		void									SetScaleFactor(DirectX::SimpleMath::Vector3 _scaleFactor);

		[[nodiscard]] inline DirectX::SimpleMath::Matrix&		GetWorldMatrix();

		[[nodiscard]] inline DirectX::SimpleMath::Matrix		UpdateTransformation();

		[[nodiscard]] inline DirectX::SimpleMath::Matrix		UpdateTransformation2D();

		[[nodiscard]] inline bool								isChildOf(GameObject* go) const;
		[[nodiscard]] inline DirectX::SimpleMath::Vector3		GetPosition()   const;
		[[nodiscard]] inline DirectX::SimpleMath::Vector3		GetRotation()   const;
		[[nodiscard]] inline DirectX::SimpleMath::Vector3		GetScale()      const;
		[[nodiscard]] inline DirectX::SimpleMath::Vector3		GetScaleFactor() const;
		[[nodiscard]] inline GameObject*						GetChild() const;
		[[nodiscard]] inline GameObject*						GetParent() const;

	protected:
		void									SetChild(GameObject* go);

	private:
		DirectX::SimpleMath::Vector3			m_position;
		DirectX::SimpleMath::Vector3			m_rotation;

		DirectX::SimpleMath::Vector3			m_scale;
		DirectX::SimpleMath::Vector3			m_scaleFactor;


		DirectX::SimpleMath::Matrix				m_worldMatrix;

		GameObject*								m_parent;
		GameObject*								m_child;

		S2DE_SERIALIZE_BEGIN(S2DE::GameObjects::Components::Component);
		S2DE_SERIALIZE_ADD(m_position.x);
		S2DE_SERIALIZE_ADD(m_position.y);
		S2DE_SERIALIZE_ADD(m_position.z);
		S2DE_SERIALIZE_ADD(m_rotation.z);
		S2DE_SERIALIZE_ADD(m_rotation.z);
		S2DE_SERIALIZE_ADD(m_rotation.z);
		S2DE_SERIALIZE_ADD(m_scale.z);
		S2DE_SERIALIZE_ADD(m_scale.z);
		S2DE_SERIALIZE_ADD(m_scale.z);
		S2DE_SERIALIZE_END();

	protected:
		void									RunOnPositionChangedCallbacks();
		void									RunOnRotationChangedCallbacks();
		void									RunOnScaleChangedCallbacks();

	public:
		Core::Utils::CallbackWrapper<Core::Utils::DefaultCallback>		onPositionChanged;
		Core::Utils::CallbackWrapper<Core::Utils::DefaultCallback>		onRotationChanged;
		Core::Utils::CallbackWrapper<Core::Utils::DefaultCallback>		onScaleChanged;
	};
}


S2DE_SERIALIZE_CLASS_IMPLEMENTATION(S2DE::GameObjects::Components::Transform, boost::serialization::object_serializable);