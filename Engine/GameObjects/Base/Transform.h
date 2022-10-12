#pragma once
#include "Base/Main/Common.h"

namespace S2DE::GameObjects
{
	class S2DE_API Transform
	{
	//Helpers
	public:
		//Convert euler angles to quaternion		
		static DirectX::SimpleMath::Quaternion	ToQuaternion(DirectX::SimpleMath::Vector3 rot);

	public:
		Transform();
		~Transform();

		//Reset all object coordinates
		void									Reset();

		//Set position by vector3
		void									SetPosition(DirectX::SimpleMath::Vector3 pos);
		//Set X axis for postion vector
		void									SetPosition_X(float x);
		//Set Y axis for postion vector
		void									SetPosition_Y(float y);
		//Set Z axis for postion vector
		void									SetPosition_Z(float z);

		//Set rotation by vector3
		void									SetRotation(DirectX::SimpleMath::Vector3 rot);
		//Set X axis for rotation vector (Roll)
		void									SetRotation_X(float x);
		//Set Y axis for rotation vector (Pitch)
		void									SetRotation_Y(float y);
		//Set Z axis for rotation vector (Yaw)
		void									SetRotation_Z(float z);

		//Set scale by vector3
		void									SetScale(DirectX::SimpleMath::Vector3 scale);
		//Set X axis for scale vector
		void									SetScale_X(float x);
		//Set Y axis for scale vector
		void									SetScale_Y(float y);
		//Set Z axis for scale vector
		void									SetScale_Z(float z);

		//Get transform pointer
		inline Transform*						GetTransform() { return this; }

		//Get object position
		inline DirectX::SimpleMath::Vector3		GetPosition()   const { return m_Position; }
		//Get object rotation
		inline DirectX::SimpleMath::Vector3		GetRotation()   const { return m_Rotation; }
		//Get object scale
		inline DirectX::SimpleMath::Vector3		GetScale()      const { return m_Scale; }

		//Get object world matrix
		inline DirectX::SimpleMath::Matrix&		GetWorldMatrix() { return m_WorldMatrix; }

		//Update matrix functions
		virtual DirectX::SimpleMath::Matrix		UpdateTransformation();
		virtual DirectX::SimpleMath::Matrix		UpdateTransformation2D();

	protected:
		virtual void							OnPositionChanged() { }
		virtual void							OnRotationChanged() { }
		virtual void							OnScaleChanged() {	}


		DirectX::SimpleMath::Vector3			m_Position;
		DirectX::SimpleMath::Vector3			m_Rotation;
		DirectX::SimpleMath::Vector3			m_Scale;
		DirectX::SimpleMath::Matrix				m_WorldMatrix;
	};
}