#include "Transform.h"
#include "Base/Engine.h"
#include "Base/GameWindow.h"

namespace S2DE::GameObjects
{
	Transform::Transform() : 
		m_Position(DirectX::SimpleMath::Vector3::Zero),
		m_Rotation(DirectX::SimpleMath::Vector3::Zero),
		m_Scale(DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f)),
		m_WorldMatrix(DirectX::SimpleMath::Matrix::Identity)
	{

	}

	Transform::~Transform()
	{
		m_Position	= DirectX::SimpleMath::Vector3::Zero;
		m_Scale		= DirectX::SimpleMath::Vector3::Zero;
		m_Rotation	= DirectX::SimpleMath::Vector3::Zero;
	}

	void Transform::Reset()
	{
		m_Position	= DirectX::SimpleMath::Vector3::Zero;
		m_Rotation	= DirectX::SimpleMath::Vector3::Zero;
		m_Scale		= DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);

		OnPositionChanged();
		OnRotationChanged();
		OnScaleChanged();
	}

	void Transform::SetScale_Z(float z)
	{
		m_Scale.z = z;
		OnScaleChanged();
	}

	void Transform::SetScale_Y(float y)
	{
		m_Scale.y = y;
		OnScaleChanged();
	}

	void Transform::SetScale_X(float x)
	{
		m_Scale.x = x;
		OnScaleChanged();
	}

	void Transform::SetPosition_Y(float y)
	{
		m_Position.y = y;
		OnPositionChanged();
	}

	void Transform::SetPosition_Z(float z)
	{
		m_Position.z = z;
		OnPositionChanged();
	}

	void Transform::SetPosition_X(float x)
	{
		m_Position.x = x;
		OnPositionChanged();
	}

	void Transform::SetRotation_X(float x)
	{
		m_Rotation.x = x;
		OnRotationChanged();
	}

	void Transform::SetRotation_Y(float y)
	{
		m_Rotation.y = y;
		OnRotationChanged();
	}

	void Transform::SetRotation_Z(float z)
	{
		m_Rotation.z = z;
		OnRotationChanged();
	}

	void Transform::SetPosition(DirectX::SimpleMath::Vector3 pos)
	{
		m_Position = pos;
		OnPositionChanged();
	}

	void Transform::SetRotation(DirectX::SimpleMath::Vector3 rot)
	{
		m_Rotation = rot;
		OnRotationChanged();
	}

	void Transform::SetScale(DirectX::SimpleMath::Vector3 scale)
	{
		m_Scale = scale;
		OnScaleChanged();
	}

	inline DirectX::SimpleMath::Quaternion Transform::ToQuaternion(DirectX::SimpleMath::Vector3 rot)
	{
		return DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(DirectX::XMConvertToRadians(rot.z), 
			DirectX::XMConvertToRadians(rot.y), 
			DirectX::XMConvertToRadians(rot.x));
	}

	DirectX::SimpleMath::Matrix Transform::UpdateTransformation2D()
	{
		m_WorldMatrix = DirectX::XMMatrixTransformation2D(
			//Scale
			//Scale center | scale rotation | scale vec
			DirectX::SimpleMath::Vector2::Zero, 0.0f, DirectX::SimpleMath::Vector2(m_Scale.x, m_Scale.y),
			//Angle in radians
			//Rotation center | Angle
			DirectX::SimpleMath::Vector2(float(Core::Engine::GetGameWindow()->GetWidth() / 2),
				float(Core::Engine::GetGameWindow()->GetHeight() / 2)), m_Rotation.x,
			//Position 
			DirectX::SimpleMath::Vector2(m_Position.x, m_Position.y));

		m_WorldMatrix.Transpose(m_WorldMatrix);

		return m_WorldMatrix;
	}

	DirectX::SimpleMath::Matrix Transform::UpdateTransformation()
	{	
		m_WorldMatrix = DirectX::XMMatrixTransformation(
			//Scale
			//Center | Rotation | Scaling
			DirectX::SimpleMath::Vector3::Zero, DirectX::SimpleMath::Vector3::Zero, m_Scale,
			//Rotation
			//Center | Quatarnion
			DirectX::SimpleMath::Vector3::Zero, ToQuaternion(m_Rotation),
			//Translation
			m_Position);

		m_WorldMatrix.Transpose(m_WorldMatrix);

		return m_WorldMatrix;
	}
}