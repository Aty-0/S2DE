#include "Transform.h"
#include "Base/Engine.h"
#include "Base/GameWindow.h"
#include "GameObjects/Base/GameObject.h"

namespace S2DE::GameObjects::Components
{
	Transform::Transform() : 
		m_position(DirectX::SimpleMath::Vector3::Zero),
		m_rotation(DirectX::SimpleMath::Vector3::Zero),
		m_scale(DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f)),
		m_worldMatrix(DirectX::SimpleMath::Matrix::Identity)
	{

	}

	Transform::~Transform()
	{
		m_position	= DirectX::SimpleMath::Vector3::Zero;
		m_rotation	= DirectX::SimpleMath::Vector3::Zero;
		m_scale		= DirectX::SimpleMath::Vector3::Zero;
	}

	void Transform::Reset()
	{
		m_position	= DirectX::SimpleMath::Vector3::Zero;
		m_rotation	= DirectX::SimpleMath::Vector3::Zero;
		m_scale		= DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);


		RunOnPositionChangedCallbacks();
		RunOnRotationChangedCallbacks();
		RunOnScaleChangedCallbacks();
	}

	bool Transform::isChildOf(GameObject* go) const
	{
		return GetChild() == go->GetTransform()->GetChild();
	}

	void Transform::SetParent(GameObject* go)
	{
		m_parent = go;
	}

	void Transform::RunOnPositionChangedCallbacks()
	{
		for (const auto& callback : onPositionChanged)
		{
			if (callback != nullptr)
			{
				callback();
			}
		}
	}

	void Transform::RunOnRotationChangedCallbacks()
	{
		for (const auto& callback : onRotationChanged)
		{
			if (callback != nullptr)
			{
				callback();
			}
		}
	}

	void Transform::RunOnScaleChangedCallbacks()
	{
		for (const auto& callback : onScaleChanged)
		{
			if (callback != nullptr)
			{
				callback();
			}
		}
	}

	void Transform::SetScale_Z(float z)
	{
		m_scale.z = z;
		RunOnScaleChangedCallbacks();
	}

	void Transform::SetScale_Y(float y)
	{
		m_scale.y = y;
		RunOnScaleChangedCallbacks();
	}

	void Transform::SetScale_X(float x)
	{
		m_scale.x = x;
		RunOnScaleChangedCallbacks();
	}

	void Transform::SetPosition_Y(float y)
	{
		m_position.y = y;
		RunOnPositionChangedCallbacks();
	}

	void Transform::SetPosition_Z(float z)
	{
		m_position.z = z;
		RunOnPositionChangedCallbacks();
	}

	void Transform::SetPosition_X(float x)
	{
		m_position.x = x;
		RunOnPositionChangedCallbacks();
	}

	void Transform::SetRotation_X(float x)
	{
		m_rotation.x = x;
		RunOnRotationChangedCallbacks();
	}

	void Transform::SetRotation_Y(float y)
	{
		m_rotation.y = y;
		RunOnRotationChangedCallbacks();
	}

	void Transform::SetRotation_Z(float z)
	{
		m_rotation.z = z;
		RunOnRotationChangedCallbacks();
	}

	void Transform::SetPosition(DirectX::SimpleMath::Vector3 pos)
	{
		m_position = pos;
		RunOnPositionChangedCallbacks();
	}

	void Transform::SetRotation(DirectX::SimpleMath::Vector3 rot)
	{
		m_rotation = rot;
		RunOnRotationChangedCallbacks();
	}

	void Transform::SetScale(DirectX::SimpleMath::Vector3 scale)
	{
		m_scale = scale;
		RunOnScaleChangedCallbacks();
	}

	inline DirectX::SimpleMath::Quaternion TransformHelpers::ToQuaternion(DirectX::SimpleMath::Vector3 rot)
	{
		return DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(DirectX::XMConvertToRadians(rot.z), 
			DirectX::XMConvertToRadians(rot.y), 
			DirectX::XMConvertToRadians(rot.x));
	}

	DirectX::SimpleMath::Matrix Transform::UpdateTransformation2D()
	{
		Transform* transformParent = nullptr;

		DirectX::SimpleMath::Vector3 parentPosition = DirectX::SimpleMath::Vector3::Zero;
		DirectX::SimpleMath::Vector3 parentRotation = DirectX::SimpleMath::Vector3::Zero;
		DirectX::SimpleMath::Vector3 parentScale = DirectX::SimpleMath::Vector3::Zero;

		if (m_parent != nullptr)
		{
			transformParent = m_parent->GetTransform();
			parentPosition = transformParent->GetPosition();
			parentRotation = transformParent->GetPosition();
			parentScale = transformParent->GetPosition();
		}


		m_worldMatrix = DirectX::XMMatrixTransformation2D(
			//Scale
			//Scale center | scale rotation | scale vec
			DirectX::SimpleMath::Vector2::Zero, 0.0f, DirectX::SimpleMath::Vector2(m_scale.x * parentScale.x, m_scale.y * parentScale.y),
			//Angle in radians
			//Rotation center | Angle
			DirectX::SimpleMath::Vector2(float(Core::Engine::GetGameWindow()->GetWidth() / 2),
				float(Core::Engine::GetGameWindow()->GetHeight() / 2)), m_rotation.x + parentRotation.x,
			//Position 
			DirectX::SimpleMath::Vector2(m_position.x + parentPosition.x, m_position.y + parentPosition.y));

		m_worldMatrix.Transpose(m_worldMatrix);

		return m_worldMatrix;
	}

	DirectX::SimpleMath::Matrix Transform::UpdateTransformation()
	{	
		Transform* transformParent = nullptr;

		DirectX::SimpleMath::Vector3 parentPosition = DirectX::SimpleMath::Vector3::Zero;
		DirectX::SimpleMath::Vector3 parentRotation = DirectX::SimpleMath::Vector3::Zero;
		DirectX::SimpleMath::Vector3 parentScale = DirectX::SimpleMath::Vector3::One;

		if (m_parent != nullptr)
		{
			transformParent = m_parent->GetTransform();
			parentPosition = transformParent->GetPosition();
			parentRotation = transformParent->GetPosition();
			parentScale	   = transformParent->GetPosition();
		}

		m_worldMatrix = DirectX::XMMatrixTransformation(
			//Scale
			//Center | Rotation | Scaling
			DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::Zero, m_scale * parentScale,
			//Rotation
			//Center | Quatarnion
			DirectX::SimpleMath::Vector3::Zero, TransformHelpers::ToQuaternion(m_rotation + parentRotation),
			//Translation
			m_position + parentPosition);

		m_worldMatrix.Transpose(m_worldMatrix);

		return m_worldMatrix;
	}
}