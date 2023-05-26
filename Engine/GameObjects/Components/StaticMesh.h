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

		bool LoadMesh(std::string name) override;
		bool LoadTexture(std::string name) override;	
		// Get new version of current shader from resource manager
		void UpdateShader() override;
		// Get new version of current texture from resource manager
		void UpdateTexture() override;
		void SetColor(Math::Color<float> color);
		void UseIndices(bool use);
		// Cut one single object to many other
		void CutMeshToParts();
	protected:
		void OnUpdate(float deltaTime) override { }
		void OnRender(Render::Renderer* renderer) override;
	private:
		Render::Mesh* m_mesh;
		Render::Texture* m_textureCube;
		Render::Shader* m_shader;
		Math::Color<float>	m_color;
		bool m_useIndices;
		// TODO: Shader sync
		bool m_useSkyCube; 
		std::int32_t m_savedIndex;
	};
}

