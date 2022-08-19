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

		// Load sprite texture
		virtual bool									LoadTexture(std::string name) override;
		// Load sprite texture with some options
		virtual bool									LoadTextureA(std::string name, bool unload_texture = false, bool auto_load_texture = true) override;
		// Get new version of current shader from resource manager
		virtual void									UpdateShader() override;
		// Get new version of current texture from resource manager
		virtual void									UpdateTexture() override;
		// Set atlas frame position 
		void											SetAtlasFramePosition(DirectX::SimpleMath::Vector2 position);
		// Set atlas frame size								 
		void											SetAtlasSize(DirectX::SimpleMath::Vector2 size);
		// Set sprite color 
		void											SetColor(Math::Color<float> color);
		// Set billboard mode
		void											SetBillboard(bool billboard);
		// Set two sided mode
		void											SetTwoSidedMode(bool mode);

	protected:
		virtual void									CreateVertexBuffer();
		virtual void									CreateIndexBuffer();
		virtual void									SetDefaultShader();
		virtual void									SetDefaultTexture();
		virtual inline DirectX::SimpleMath::Vector3		CalcScaleFactor();
		virtual inline DirectX::SimpleMath::Matrix		UpdateTransformation() override;
		virtual void									OnRender() override;

	private:
		Render::Texture*								m_texture;
		bool											m_unloadTexture;
		Render::Shader*									m_shader;
		Render::VertexBuffer<Render::Vertex>*			m_vertexBuffer;
		Render::IndexBuffer<std::int32_t>*				m_indexBuffer;
		DirectX::SimpleMath::Vector4					m_tileFrame;
		Math::Color<float>								m_color;
		Render::ConstantBuffer<Render::CB::CB_Sprite>*	m_spriteCB;
		bool											m_billboard;
		bool											m_twoSided;
	};
}