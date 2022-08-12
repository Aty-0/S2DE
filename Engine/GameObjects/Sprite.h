#pragma once
#include "GameObjects/Base/Drawable.h"
#include "Base/ResourceManager.h"
#include "Render/Renderer.h"
#include "Render/Buffers.h"

namespace S2DE::GameObjects
{
	class S2DE_API Sprite : public Drawable
	{
	public:
		Sprite();
		virtual ~Sprite();

		virtual bool									LoadTexture(std::string name) override;
		virtual bool									LoadTextureA(std::string name, bool unload_texture = false, bool auto_load_texture = true) override;

		//Get new version of current shader from resource manager
		virtual void									UpdateShader() override;

		//Get new version of current texture from resource manager
		virtual void									UpdateTexture() override;

		//Set frame position 
		void											SetAtlasFramePosition(std::int32_t x, std::int32_t y);

		//Set frame size								 
		void											SetAtlasSize(DirectX::SimpleMath::Vector2 size);

		void											SetColor(Math::Color<float> color);

	protected:
		virtual void									OnUpdate(float DeltaTime) override { }
		virtual void									OnRender() override;

		virtual void									CreateVertexBuffer();
		virtual void									CreateIndexBuffer();
		virtual void									SetDefaultShader();
		virtual void									SetDefaultTexture();
		virtual inline DirectX::SimpleMath::Vector3		CalcScaleFactor();
		virtual inline DirectX::SimpleMath::Matrix		UpdateTransformation() override;

	private:
		Render::Texture*								m_texture;
		bool											m_unloadTexture;
		Render::Shader*									m_shader;
		Render::VertexBuffer<Render::Vertex>*			m_vertexBuffer;
		Render::IndexBuffer<std::int32_t>*				m_indexBuffer;
		DirectX::SimpleMath::Rectangle					m_tileFrame;
		Math::Color<float>								m_color;
		Render::ConstantBuffer<Render::CB::CB_Sprite>*	m_spriteCB;
	};
}