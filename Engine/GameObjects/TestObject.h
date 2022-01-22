#pragma once
#include "GameObjects/Drawable.h"
#include "Base/ResourceManager.h"
#include "Graphics/Renderer.h"
#include "Graphics/Buffers.h"

namespace S2DE::GameObjects
{
	//Dumb class for dumb tests :)
	//!Warning! Code in this class created just for lulz XDD

	class S2DE_API TestObject : public Drawable
	{
	public:
		TestObject();
		~TestObject();

		//Get new version of current shader from resource manager
		virtual void	UpdateShader() override;

		//Get new version of current texture from resource manager
		virtual void	UpdateTexture() override;

	protected:
		virtual void OnUpdate(float DeltaTime) override;
		virtual void OnRender() override;
		virtual Math::XMatrix UpdateTransformation() override;
		virtual bool CheckOnIntersection() override;

	private:
		Render::Texture*	  m_texture;
		Render::Shader*		  m_shader;
		Render::VertexBuffer<Render::Vertex>* m_vbuffer;
		Render::IndexBuffer<std::int32_t>*  m_ibuffer;
		float				  m_r;
		Math::Vector3		  m_scale_factor;
	};
}