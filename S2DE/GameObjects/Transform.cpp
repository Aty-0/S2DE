#include "Transform.h"
#include "Base/Engine.h"
#include "Base/GameWindow.h"

namespace S2DE
{
	Transform::Transform() : 
		m_Position(Vector3::Zero()),
		m_Rotation(Vector3::Zero()),
		m_Scale(Vector3(1.0f, 1.0f, 1.0f))
	{

	}

	Transform::~Transform()
	{
		m_Position = Vector3::Zero();
		m_Scale = Vector3::Zero();
		m_Rotation = Vector3::Zero();
	}

	void Transform::Reset()
	{
		m_Position = Vector3::Zero();
		m_Scale = Vector3(1.0f, 1.0f, 1.0f);

		OnPositionChanged();
		OnRotationChanged();
		OnScaleChanged();
	}

	void Transform::SetScale_Z(float z)
	{
		m_Scale = Vector3(GetScale().x, GetScale().y, z);
		OnScaleChanged();
	}

	void Transform::SetScale_Y(float y)
	{
		m_Scale = Vector2(GetScale().x, y);
		OnScaleChanged();
	}

	void Transform::SetScale_X(float x)
	{
		m_Scale = Vector2(x, GetScale().y);
		OnScaleChanged();
	}

	void Transform::SetPosition_Y(float y)
	{
		m_Position = Vector2(GetPosition().x, y);
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

	void Transform::SetRotation(Vector3 r)
	{
		m_Rotation = r;
		OnRotationChanged();
	}

	void Transform::SetPosition_Z(float z)
	{
		m_Position = Vector3(GetPosition().x, GetPosition().y, z);
		OnPositionChanged();
	}

	void Transform::SetPosition_X(float x)
	{
		m_Position = Vector2(x, GetPosition().y);
		OnPositionChanged();
	}

	void Transform::SetPosition(Vector3 pos)
	{
		m_Position = pos;
		OnPositionChanged();
	}

	void Transform::SetScale(Vector3 scale)
	{
		m_Scale = scale;
		OnScaleChanged();
	}

	Vector3 Transform::GetRotation()  const
	{
		return m_Rotation;
	}

	Vector3 Transform::GetPosition()  const
	{
		return m_Position;
	}

	Vector3 Transform::GetScale()     const
	{
		return m_Scale;
	}

	XVector Transform::ToQuaternion(Vector3 rot)
	{
		return DirectX::XMQuaternionRotationRollPitchYawFromVector(To_XMVector3(rot));
	}

	XMatrix Transform::UpdateTransformation2D()
	{
		m_WorldMatrix = DirectX::XMMatrixTransformation2D(
			//Scale
			//Scale center | scale rotation | scale vec
			XVector(), 0.0f, To_XMVector2(Vector2(m_Scale.x, m_Scale.y)),
			//Angle in radians
			//Rotation center | Angle
			To_XMVector2(Vector2(float(Engine::GetGameWindow()->GetWidth() / 2),
				float(Engine::GetGameWindow()->GetHeight() / 2))), m_Rotation.x,
			//Position 
			To_XMVector2(Vector2(m_Position.x, m_Position.y)));

		return DirectX::XMMatrixTranspose(m_WorldMatrix);
	}

	XMatrix Transform::UpdateTransformation()
	{	
		m_WorldMatrix = DirectX::XMMatrixTransformation(
			//Scale
			//Center | Rotation | Scaling
			XVector(), XVector(), To_XMVector3(m_Scale),
			//Rotation
			//Center | Quatarnion
			XVector(), ToQuaternion(m_Rotation),
			//Translation
			To_XMVector3(m_Position));

		return DirectX::XMMatrixTranspose(m_WorldMatrix);
	}
}