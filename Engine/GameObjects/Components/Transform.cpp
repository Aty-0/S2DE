#include "Transform.h"
#include "Base/Engine.h"
#include "Base/GameWindow.h"
#include "GameObjects/Base/GameObject.h"

namespace S2DE::GameObjects::Components
{
	Transform::Transform() :
		m_position(Math::float3::Zero),
		m_rotation(Math::float3::Zero),
		m_scale(Math::float3::One),
		m_scaleFactor(Math::float3::One),
		m_worldMatrix(Math::float4x4::Identity),
		m_parent(nullptr),
		m_child(nullptr)
	{

	}

	Transform::~Transform()
	{
		m_position = Math::float3::Zero;
		m_rotation = Math::float3::Zero;
		m_scale = Math::float3::Zero;
		m_parent = nullptr;
		m_child = nullptr;

		onPositionChanged.Clear();
		onRotationChanged.Clear();
		onScaleChanged.Clear();
	}

	void Transform::Reset() 
	{
		m_position = Math::float3::Zero;
		m_rotation = Math::float3::Zero;
		m_scale = Math::float3::One;


		RunPositionChangedCallbacks();
		RunRotationChangedCallbacks();
		RunScaleChangedCallbacks();
	}

	bool Transform::isChildOf(GameObject* go) const 
	{
		if (m_child == nullptr)
		{
			Core::Utils::Logger::Error("Child is empty, so what's we need to check ?");
			return false;
		}

		return m_child == go->GetTransform()->GetChild();
	}

	void Transform::SetChild(GameObject* go) 
	{
		if (GetOwner() == go)
		{
			Core::Utils::Logger::Error("You can't assign yourself as child!");
			return;
		}

		m_child = go;
	}

	void Transform::SetParent(GameObject* go)
	{
		if (GetOwner() == go)
		{
			Core::Utils::Logger::Error("You can't assign yourself as parent!");
			return;
		}

		go->GetTransform()->SetChild(GetOwner());

		m_parent = go;
	}

	void Transform::RunPositionChangedCallbacks() 
	{
		onPositionChanged.RunAllCallbacks();

		// If parent have changes, we should call change and for child
		if (m_child != nullptr)
		{
			m_child->GetTransform()->RunPositionChangedCallbacks();
		}
	}

	void Transform::RunRotationChangedCallbacks()
	{
		onRotationChanged.RunAllCallbacks();

		// If parent have changes, we should call change and for child
		if (m_child != nullptr)
		{
			m_child->GetTransform()->RunRotationChangedCallbacks();
		}
	}

	void Transform::RunScaleChangedCallbacks()
	{
		onScaleChanged.RunAllCallbacks();

		// If parent have changes, we should call change and for child
		if (m_child != nullptr)
		{
			m_child->GetTransform()->RunScaleChangedCallbacks();
		}
	}

	void Transform::SetScaleZ(float z)
	{
		m_scale.z = z;
		RunScaleChangedCallbacks();
	}

	void Transform::SetScaleY(float y)
	{
		m_scale.y = y;
		RunScaleChangedCallbacks();
	}

	void Transform::SetScaleX(float x)
	{
		m_scale.x = x;
		RunScaleChangedCallbacks();
	}

	void Transform::SetPositionY(float y) 
	{
		m_position.y = y;
		RunPositionChangedCallbacks();
	}

	void Transform::SetPositionZ(float z) 
	{
		m_position.z = z;
		RunPositionChangedCallbacks();
	}

	void Transform::SetPositionX(float x) 
	{
		m_position.x = x;
		RunPositionChangedCallbacks();
	}

	void Transform::SetRotationX(float x) 
	{
		m_rotation.x = x;
		RunRotationChangedCallbacks();
	}

	void Transform::SetRotationY(float y) 
	{
		m_rotation.y = y;
		RunRotationChangedCallbacks();
	}

	void Transform::SetRotationZ(float z) 
	{
		m_rotation.z = z;
		RunRotationChangedCallbacks();
	}

	void Transform::SetPosition(Math::float3 pos)
	{
		m_position = pos;
		RunPositionChangedCallbacks();
	}

	void Transform::SetRotation(Math::float3 rot)
	{
		m_rotation = rot;
		RunRotationChangedCallbacks();
	}

	void Transform::SetScale(Math::float3 scale)
	{
		m_scale = scale;
		RunScaleChangedCallbacks();
	}

	void Transform::SetScaleFactor(Math::float3 _scaleFactor)
	{
		m_scaleFactor = _scaleFactor;
	}

	inline DirectX::SimpleMath::Quaternion TransformHelpers::ToQuaternion(Math::float3 rot)
	{
		return DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(DirectX::XMConvertToRadians(rot.z),
			DirectX::XMConvertToRadians(rot.y),
			DirectX::XMConvertToRadians(rot.x));
	}

	inline Math::float4x4 Transform::UpdateTransformation2D()
	{
		auto parentPosition = Math::float3::Zero;
		auto parentRotation = Math::float3::Zero;
		auto parentScale = Math::float3::One;

		if (m_parent != nullptr)
		{
			const auto transformParent = m_parent->GetTransform();

			if (transformParent != nullptr)
			{
				parentPosition = transformParent->GetPosition();
				parentRotation = transformParent->GetRotation();
				parentScale = transformParent->GetScale();
			}
		}

		const auto windowHeight = static_cast<float>(Core::GameWindow::GetInstance()->GetHeight());

		m_worldMatrix = DirectX::XMMatrixTransformation2D(
			//Scale
			//Scale center | scale rotation | scale vec
			Math::float2::Zero, 0.0f, Math::float2(m_scale.x * parentScale.x * m_scaleFactor.x, m_scale.y * parentScale.y * m_scaleFactor.y),
			//Angle in radians
			//Rotation center | Angle
			Math::float3::Zero,
			DirectX::XMConvertToRadians(m_rotation.x + parentRotation.x),
			//Position 
			Math::float2(m_position.x + parentPosition.x, windowHeight - (m_position.y + parentPosition.y)) * 0.15f);


		m_worldMatrix.Transpose(m_worldMatrix);

		return m_worldMatrix;
	}

	inline Math::float4x4 Transform::UpdateTransformation()
	{
		auto parentPosition = Math::float3::Zero;
		auto parentRotation = Math::float3::Zero;
		auto parentScale = Math::float3::One;

		if (m_parent != nullptr)
		{
			const auto transformParent = m_parent->GetTransform();

			if (transformParent != nullptr)
			{
				parentPosition = transformParent->GetPosition();
				parentRotation = transformParent->GetRotation();
				parentScale = transformParent->GetScale();
			}
		}

		m_worldMatrix = DirectX::XMMatrixTransformation(
			//Scale
			//Center | Rotation | Scaling
			Math::float3::Zero, Math::float3::Zero, m_scale * parentScale * m_scaleFactor,
			//Rotation
			//Center | Quatarnion
			Math::float3::Zero, TransformHelpers::ToQuaternion(m_rotation + parentRotation),
			//Translation
			m_position + parentPosition);

		m_worldMatrix.Transpose(m_worldMatrix);

		return m_worldMatrix;
	}

	inline Math::float3 Transform::GetPosition() const
	{
		return m_position;
	}

	inline Math::float3 Transform::GetRotation() const
	{
		return m_rotation;
	}

	inline Math::float3 Transform::GetScale() const
	{
		return m_scale;
	}

	inline GameObject* Transform::GetChild() const 
	{
		return m_child;
	}

	inline GameObject* Transform::GetParent() const
	{
		return m_parent;
	}

	inline Math::float3	Transform::GetScaleFactor() const
	{
		return m_scaleFactor;
	}

	inline Math::float4x4& Transform::GetWorldMatrix()
	{
		return m_worldMatrix;
	}
}