#pragma once
#include "GameObjects/Components/DrawableComponent.h"
#include "Math/Color.h"

namespace S2DE::Render::CB
{
	struct CB_Sprite;
}

namespace S2DE::Render
{
	class Texture;
	class Shader;

	template<typename T>
	class VertexBuffer;

	template<typename T>
	class IndexBuffer;

	template<typename T>
	class ConstantBuffer;
}

namespace S2DE::GameObjects::Components
{	
	class S2DE_API Sprite : public Components::DrawableComponent
	{
	public:
		Sprite();
		virtual ~Sprite();

		// Load sprite texture
		bool											LoadTexture(std::string name) override;
		// Load sprite texture with some options
		bool											LoadTextureA(std::string name, bool unload_texture = false, bool auto_load_texture = true) override;
		// Get new version of current shader from resource manager
		void											UpdateShader() override;
		// Get new version of current texture from resource manager
		void											UpdateTexture() override;
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

		void											SetUIMode(bool mode);

		void											OnRender(Render::Renderer* renderer) override;

		void											OnCreate() override;

	protected:
		void											CreateVertexBuffer();
		void											CreateIndexBuffer();
		void											SetDefaultShader();
		void											SetDefaultTexture();

		[[nodiscard]] virtual inline DirectX::SimpleMath::Vector3		CalcScaleFactor();
		
	private:
		Render::Texture*								m_texture;
		bool											m_unloadTexture;
		Render::Shader*									m_shader;
		Render::VertexBuffer<Render::Vertex>*			m_vertexBuffer;
		Render::IndexBuffer<std::int32_t>*				m_indexBuffer;
		DirectX::SimpleMath::Vector4					m_tileFrame;
		Math::Color<float>								m_color;
		Render::ConstantBuffer<struct Render::CB::CB_Sprite>*	m_spriteCB;
		bool											m_billboard;
		bool											m_twoSided;
		bool											m_uiMode;
	};
}