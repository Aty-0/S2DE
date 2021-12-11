#pragma once
#include "GameObjects/GameObject.h"
#include "Base/ResourceManager.h"
#include "Graphics/Renderer.h"
#include "Graphics/Buffers.h"

namespace S2DE::GameObjects
{
	//Dumb class for dumb tests :)
	//!Warning! Code in this class created just for lulz XDD

	class S2DE_API TestObject : public GameObject
	{
	public:
		TestObject();
		~TestObject();

	private:
		virtual void OnUpdate(float DeltaTime) override;
		virtual void OnRender() override;
		virtual Math::XMatrix UpdateTransformation() override;
		virtual bool CheckOnIntersection() override;
		Render::Texture*	  m_texture;
		Render::Shader*		  m_shader;
		Render::VertexBuffer* m_vbuffer;
		Render::IndexBuffer*  m_ibuffer;
		float				  m_r;
		Math::Vector3		  m_scale_factor;
	};
}