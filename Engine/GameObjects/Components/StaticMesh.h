#pragma once
#include "GameObjects/Components/DrawableComponent.h"
#include "Math/Color.h"

namespace S2DE::Render
{
	class Mesh;
	class Texture;
	class Shader;
}


namespace S2DE::GameObjects::Components
{
	class S2DE_API StaticMesh : public DrawableComponent
	{
	public:
		StaticMesh();
		~StaticMesh();

		void					CutMeshToParts();

		bool					LoadMesh(std::string name) override;
		bool					LoadTexture(std::string name) override;
		bool					LoadTextureA(std::string name, bool unload_texture = false, bool auto_load_texture = true) override;

		//Get new version of current shader from resource manager
		void					UpdateShader() override;

		//Get new version of current texture from resource manager
		void					UpdateTexture() override;

		void					SetColor(Math::Color<float> color);
		void					UseIndices(bool use);
	protected:
		void					OnUpdate(float deltaTime) override { }
		void					OnRender(Render::Renderer* renderer) override;
	private:
		Render::Mesh*		m_mesh;

		Render::Texture*	m_textureCube;
		bool				m_unload_texture;
		Render::Shader*		m_shader;


		Math::Color<float>	m_color;
		bool				m_useIndices;
		bool				m_useSkyCube; // TODO: Shader sync
		
		// if we want to use partinal models
		std::int32_t							m_savedIndex;
	};
}

