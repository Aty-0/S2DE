#pragma once
#include "Base/Main/Common.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Graphics/Renderer.h"

namespace S2DE::GameObjects
{
	class S2DE_API Transform
	{
	public:
		Transform();
		~Transform();

		//Reset all object coordinates
		void                Reset();

		//Set position by vector3
		void                SetPosition(Math::Vector3 pos);
		//Set X axis for postion vector
		void                SetPosition_X(float x);
		//Set Y axis for postion vector
		void                SetPosition_Y(float y);
		//Set Z axis for postion vector
		void                SetPosition_Z(float z);

		//Set rotation by vector3
		void                SetRotation(Math::Vector3 r);
		//Set X axis for rotation vector
		void                SetRotation_X(float x);
		//Set Y axis for rotation vector
		void                SetRotation_Y(float y);
		//Set Z axis for rotation vector
		void                SetRotation_Z(float z);

		//Set scale by vector3
		void                SetScale(Math::Vector3 scale);
		//Set X axis for scale vector
		void                SetScale_X(float x);
		//Set Y axis for scale vector
		void                SetScale_Y(float y);
		//Set Z axis for scale vector
		void                SetScale_Z(float z);

		inline Transform*   GetTransform() { return this; }

		inline Math::Vector3      GetPosition()   const { return m_Position; }
		inline Math::Vector3      GetRotation()   const { return m_Rotation; }
		inline Math::Vector3      GetScale()      const { return m_Scale; }

		//Get object world matrix
		inline Math::XMatrix&		 GetWorldMatrix() { return m_WorldMatrix; }

		//Update matrix functions
		virtual Math::XMatrix      UpdateTransformation();
		virtual Math::XMatrix 	 UpdateTransformation2D();

	protected:
		virtual void         OnPositionChanged() { }
		virtual void         OnRotationChanged() { }
		virtual void         OnScaleChanged() {	}

	private:
		Math::Vector3              m_Position;
		Math::Vector3              m_Rotation;
		Math::Vector3              m_Scale;

		Math::XMatrix             m_WorldMatrix;

	public:
		static Math::XVector	ToQuaternion(Math::Vector3 rot);
	};
}