#include "Transform.h"
#include "Base/Engine.h"
#include "Base/GameWindow.h"

namespace S2DE::GameObjects
{
	Transform::Transform() : 
		m_Position(Math::Vector3::Reset()),
		m_Rotation(Math::Vector3::Reset()),
		m_Scale(Math::Vector3(1.0f, 1.0f, 1.0f)),
		m_WorldMatrix(DirectX::XMMatrixIdentity())
	{

	}

	Transform::~Transform()
	{
		m_Position = Math::Vector3::Reset();
		m_Scale = Math::Vector3::Reset();
		m_Rotation = Math::Vector3::Reset();
	}

	void Transform::Reset()
	{
		m_Position = Math::Vector3::Reset();
		m_Rotation = Math::Vector3::Reset();
		m_Scale = Math::Vector3(1.0f, 1.0f, 1.0f);

		OnPositionChanged();
		OnRotationChanged();
		OnScaleChanged();
	}

	void Transform::SetScale_Z(float z)
	{
		m_Scale = Math::Vector3(GetScale().x, GetScale().y, z);
		OnScaleChanged();
	}

	void Transform::SetScale_Y(float y)
	{
		m_Scale = Math::Vector2(GetScale().x, y);
		OnScaleChanged();
	}

	void Transform::SetScale_X(float x)
	{
		m_Scale = Math::Vector2(x, GetScale().y);
		OnScaleChanged();
	}

	void Transform::SetPosition_Y(float y)
	{
		m_Position = Math::Vector2(GetPosition().x, y);
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

	void Transform::SetRotation(Math::Vector3 r)
	{
		m_Rotation = r;
		OnRotationChanged();
	}

	void Transform::SetPosition_Z(float z)
	{
		m_Position = Math::Vector3(GetPosition().x, GetPosition().y, z);
		OnPositionChanged();
	}

	void Transform::SetPosition_X(float x)
	{
		m_Position = Math::Vector2(x, GetPosition().y);
		OnPositionChanged();
	}

	void Transform::SetPosition(Math::Vector3 pos)
	{
		m_Position = pos;
		OnPositionChanged();
	}

	void Transform::SetScale(Math::Vector3 scale)
	{
		m_Scale = scale;
		OnScaleChanged();
	}

	inline Math::XVector Transform::ToQuaternion(Math::Vector3 rot)
	{
		return DirectX::XMQuaternionRotationRollPitchYawFromVector(To_XMVector3(rot));
	}

	Math::XMatrix Transform::UpdateTransformation2D()
	{
		m_WorldMatrix = DirectX::XMMatrixTransformation2D(
			//Scale
			//Scale center | scale rotation | scale vec
			Math::XVector(), 0.0f, Math::To_XMVector2(Math::Vector2(m_Scale.x, m_Scale.y)),
			//Angle in radians
			//Rotation center | Angle
			Math::To_XMVector2(Math::Vector2(float(Core::Engine::GetGameWindow()->GetWidth() / 2),
				float(Core::Engine::GetGameWindow()->GetHeight() / 2))), m_Rotation.x,
			//Position 
			Math::To_XMVector2(Math::Vector2(m_Position.x, m_Position.y)));

		return DirectX::XMMatrixTranspose(m_WorldMatrix);
	}

	Math::XMatrix Transform::UpdateTransformation()
	{	
		m_WorldMatrix = DirectX::XMMatrixTransformation(
			//Scale
			//Center | Rotation | Scaling
			Math::XVector(), Math::XVector(), To_XMVector3(m_Scale),
			//Rotation
			//Center | Quatarnion
			Math::XVector(), ToQuaternion(m_Rotation),
			//Translation
			To_XMVector3(m_Position));

		return DirectX::XMMatrixTranspose(m_WorldMatrix);
	}
}