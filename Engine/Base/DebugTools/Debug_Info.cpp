#include "Debug_Info.h"

#include "Base/Engine.h"
#include "Base/ResourceManager.h"
#include "Base/Utils/AppUsage.h"
#include "Base/GameWindow.h"
#include "Base/GameTime.h"

#include "Render/LightGlobals.h"
#include "Render/CB.h"
#include "Render/Texture.h"
#include "Render/Shader.h"
#include "Render/Mesh.h"
#include "Render/Font.h"

#include "GameObjects/Base/GameObject.h"
#include "GameObjects/Components/UI/UIText.h"

#include "Scene/SceneManager.h"

#define TEXT_COUNT 14

namespace S2DE::Core::Debug
{
	Debug_Info Debug_Info::debugInfoTool = { };

	Debug_Info::Debug_Info() : m_draw(), m_texts()
	{

	}

	Debug_Info::~Debug_Info()
	{

	}

	void Debug_Info::ToggleDraw()
	{
		m_draw = !m_draw;
		for (const auto text : m_texts)
		{
			text->GetOwner()->SetVisible(m_draw);
		}
	}

	void Debug_Info::CreateDbgTexts()
	{
		const auto window = Core::Engine::GetGameWindow();
		const auto default_font = Core::Engine::GetResourceManager().Get<Render::Font>("default");
		for (std::int32_t i = 1; i <= TEXT_COUNT; i++)
		{
			const auto text_go = Scene::CreateGameObject<S2DE::GameObjects::GameObject>("__Debug_Info_TextGO_" + std::to_string(i), S2DE_ENGINE_GAMEOBJECT_TYPE, 1,
				DirectX::SimpleMath::Vector3(-window->GetWidth() / 15, 50 + -i * 7, 0));
			const auto text_ft = text_go->CreateComponent<GameObjects::Components::UI::UIText>();
			text_ft->SetFont(default_font);
			text_ft->SetHeight(24.0f);
			//text_ft->SetColor({ 255,0,0,255 });
			text_go->SetVisible(m_draw);
			m_texts.push_back(text_ft);
		}
	}

	void Debug_Info::UpdateTexts()
	{
		// Just do not update text when we are not drawing anything...
		if (!m_draw)
			return;
	
		m_texts[0]->SetText("isEditor: %d", Engine::isEditor());
		m_texts[1]->SetText("Mem:%d mb", GetUsedMem());
		m_texts[2]->SetText("vsync:%d mb", Engine::GetRenderer()->GetVsync());
		m_texts[3]->SetText("DeltaTime%.3f ms/frame", Engine::GetGameTime().GetDeltaTime());
		m_texts[4]->SetText("FPS: %d", Engine::GetGameTime().GetFPS());
		m_texts[5]->SetText("Engine time %.3f", Engine::GetGameTime().GetTime());
		m_texts[6]->SetText("Textures loaded %d", Engine::GetResourceManager().GetResourceCount<Render::Texture>());
		m_texts[7]->SetText("Shader loaded %d", Engine::GetResourceManager().GetResourceCount<Render::Shader>());
		m_texts[8]->SetText("Mesh loaded %d", Engine::GetResourceManager().GetResourceCount<Render::Mesh>());
		m_texts[9]->SetText("Fonts loaded %d", Engine::GetResourceManager().GetResourceCount<Render::Font>());
		m_texts[10]->SetText("Lights: %d", Render::LightGlobals::GetLightCount());
		m_texts[11]->SetText("Lights Limit: %d", Render::CB::MAX_LIGHT_COUNT);
		m_texts[12]->SetText("Scene name: %s", Engine::GetSceneManager()->GetScene()->GetName().c_str());
		m_texts[13]->SetText("Objects in scene: %d", Engine::GetSceneManager()->GetScene()->GetStorage().size());
	}
}