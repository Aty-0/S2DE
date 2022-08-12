#pragma once
#include "GameObjects/Base/Drawable.h"
#include "Base/ResourceManager.h"
#include "Render/Renderer.h"
#include "Render/Buffers.h"

namespace S2DE::GameObjects
{
	class S2DE_API NoTextureTestObject : public Drawable
	{
	public:
		NoTextureTestObject();
		~NoTextureTestObject();

		//Get new version of current shader from resource manager
		virtual void	UpdateShader() override;

	protected:
		virtual void OnUpdate(float DeltaTime) override;
		virtual void OnRender() override;

	private:
		Render::Texture*	  m_texture;
		Render::Shader*		  m_shader;
		Render::VertexBuffer<Render::Vertex>* m_vbuffer;
		Render::IndexBuffer<std::int32_t>*  m_ibuffer;
		
	};
}