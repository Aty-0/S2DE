#pragma once
#include "Base/Main/Common.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Graphics/Renderer.h"

namespace S2DE
{
	class S2DE_API Transform
	{
	public:
		Transform();
		~Transform();

		//Reset all object coordinates
		void                Reset();

		//Set position by vector3
		void                SetPosition(Vector3 pos);
		//Set X axis for postion vector
		void                SetPosition_X(float x);
		//Set Y axis for postion vector
		void                SetPosition_Y(float y);
		//Set Z axis for postion vector
		void                SetPosition_Z(float z);

		//Set rotation by vector3
		void                SetRotation(Vector3 r);
		//Set X axis for rotation vector
		void                SetRotation_X(float x);
		//Set Y axis for rotation vector
		void                SetRotation_Y(float y);
		//Set Z axis for rotation vector
		void                SetRotation_Z(float z);

		//Set scale by vector3
		void                SetScale(Vector3 scale);
		//Set X axis for scale vector
		void                SetScale_X(float x);
		//Set Y axis for scale vector
		void                SetScale_Y(float y);
		//Set Z axis for scale vector
		void                SetScale_Z(float z);

		inline Transform*   GetTransform() { return this; }

		inline Vector3      GetPosition()   const { return m_Position; }
		inline Vector3      GetRotation()   const { return m_Rotation; }
		inline Vector3      GetScale()      const { return m_Scale; }

		//Get object world matrix
		inline XMatrix&		 GetWorldMatrix() { return m_WorldMatrix; }

		//Update matrix functions
		virtual XMatrix      UpdateTransformation();
		virtual XMatrix 	 UpdateTransformation2D();

	protected:
		virtual void         OnPositionChanged() { }
		virtual void         OnRotationChanged() { }
		virtual void         OnScaleChanged() {	}

	private:
		Vector3              m_Position;
		Vector3              m_Rotation;
		Vector3              m_Scale;

		XMatrix             m_WorldMatrix;

	public:
		static XVector	ToQuaternion(Vector3 rot);
	};
}