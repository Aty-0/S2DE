#include "UI_Drawable.h"
#include "Base/Engine.h"
#include "Graphics/Renderer.h"

namespace S2DE::GameObjects::UI
{
	void UI_Drawable::OnRender()
	{
		Core::Engine::GetRenderer()->TurnZBufferOff();
		OnRenderIn2D();
		Core::Engine::GetRenderer()->TurnZBufferOn();
	}
}