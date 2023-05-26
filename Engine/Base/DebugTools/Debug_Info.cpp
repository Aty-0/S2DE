#include "Debug_Info.h"

#include "Base/Engine.h"
#include "Base/ResourceManager.h"
#include "Base/Utils/AppUsage.h"
#include "Base/GameWindow.h"
#include "Base/GameTime.h"
#include "Base/Main/BuildDate.h"

#include "Render/LightGlobals.h"
#include "Render/CB.h"
#include "Render/Texture.h"
#include "Render/Shader.h"
#include "Render/Mesh.h"
#include "Render/Renderer.h"
#include "Render/Font.h"

#include "GameObjects/Base/GameObject.h"
#include "GameObjects/Components/UI/UIText.h"

#include "Scene/SceneManager.h"

#define TEXT_COUNT 15

namespace S2DE::Core::Debug
{	
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
			text->SetHeight(GlobalHeight);
		}
	}

	void Debug_Info::CreateDbgTexts()
	{
		// Get default font 
		const auto default_font = Core::Resources::ResourceManager::GetInstance()->Get<Render::Font>("default");

		for (std::int32_t i = 1; i <= TEXT_COUNT; i++)
		{
			const auto text_go = Scene::CreateGameObject<S2DE::GameObjects::GameObject>("__Debug_Info_TextGO_" + std::to_string(i), S2DE_ENGINE_GAMEOBJECT_TYPE, -1,
				Math::float3(100.0f, i * 50.0f, 0));
			const auto text_ft = text_go->CreateComponent<GameObjects::Components::UI::UIText>();
			text_ft->SetFont(default_font);
			text_ft->SetHeight(GlobalHeight);
			text_go->SetVisible(m_draw);
			m_texts.push_back(text_ft);
		}

		// The last text is a version text
		const auto lastComp = m_texts[TEXT_COUNT - 1];
		lastComp->SetText("S2DE Build %s", S2DE_BUILD_DATE);
		lastComp->SetColor({ 1.0f, 1.0f, 1.0f, 0.3f });

		// Add update text position callback on window resize
		const auto window = Core::GameWindow::GetInstance();
		window->onWindowResized.AddCallback(std::bind(&Debug::Debug_Info::UpdateTextPos, this));
	}

	void Debug_Info::UpdateTextPos()
	{
		const static auto window = Core::GameWindow::GetInstance();
		// TODO: Update pos on callback
		const auto lastComp = m_texts[TEXT_COUNT - 1];
		const auto lastGo = lastComp->GetOwner();
		const auto textlen = lastComp->GetText().size() * 4;
		lastGo->GetTransform()->SetPosition({ static_cast<float>(window->GetWidth() / 2) - textlen, window->GetHeight() - 30.0f, 0 });
	}

	void Debug_Info::UpdateTexts()
	{
		// Just do not update text when we are not drawing anything...
		if (!m_draw)
			return;

		static const auto time = Core::GameTime::GetInstance();
		static const auto resourceManager = Core::Resources::ResourceManager::GetInstance();
		static const auto renderer = Render::Renderer::GetInstance();
		static const auto scene = Scene::SceneManager::GetInstance()->GetScene();
		static const auto lightGlobals = Render::LightGlobals::GetInstance();

		const auto fps = time->GetFPS();
		if (fps >= 60)
		{
			m_texts[0]->SetColor({ 0,1,0,1 });
		}
		else if (fps >= 30)
		{
			m_texts[0]->SetColor({ 1,1,0,1 });
		}
		else
		{
			m_texts[0]->SetColor({ 1,0,0,1 });
		}

		m_texts[0]->SetText("FPS: %d", fps);

		m_texts[1]->SetText("Mem: %d mb", Core::Utils::GetUsedMem());
		m_texts[2]->SetText("vsync: %s", renderer->GetVsync() ? "True" : "False");
		m_texts[3]->SetText("DeltaTime: %.3f ms/frame", time->GetDeltaTime());
		m_texts[4]->SetText("Time: %.3f", time->GetTime());
		m_texts[5]->SetText("isEditor: %d", Engine::isEditor());
		m_texts[6]->SetText("Textures loaded: %d", resourceManager->GetResourceCount<Render::Texture>());
		m_texts[7]->SetText("Shaders loaded: %d",resourceManager->GetResourceCount<Render::Shader>());
		m_texts[8]->SetText("Mesh loaded: %d", resourceManager->GetResourceCount<Render::Mesh>());
		m_texts[9]->SetText("Fonts loaded: %d", resourceManager->GetResourceCount<Render::Font>());
		m_texts[10]->SetText("Lights: %d", lightGlobals->GetLightCount());
		m_texts[11]->SetText("Lights Limit: %d", Render::CB::MAX_LIGHT_COUNT);
		m_texts[12]->SetText("Scene name: %s", scene->GetName().c_str());
		m_texts[13]->SetText("Objects in scene: %d", scene->GetStorage().size());
	}
}