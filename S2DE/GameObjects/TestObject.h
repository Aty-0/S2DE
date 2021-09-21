#pragma once
#include "GameObjects/GameObject.h"
#include "Base/ResourceManager.h"
#include "Graphics/Renderer.h"
#include "Graphics/Buffers.h"

namespace S2DE
{
	//Dumb class for test's

	class S2DE_API TestObject : public GameObject
	{
	public:
		TestObject();
		~TestObject();

	private:
		virtual void OnRender() override;
		virtual XMatrix UpdateTransformation() override;

		Texture*	  m_texture;
		Shader*		  m_shader;
		VertexBuffer* m_vbuffer;
		IndexBuffer*  m_ibuffer;
		float		  m_r;
		Vector3		  m_scale_factor;
	};
}