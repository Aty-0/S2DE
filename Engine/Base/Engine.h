#pragma once
#include "Base/Main/Common.h"
#include "Base/Utils/Logger.h"

namespace S2DE::Core
{
	class ApplicationHandle;
	class S2DE_API Engine : public Core::Utils::Singleton<Engine>
	{
	public:
		Engine();
		~Engine();

		// Run engine function
		// Initialize all engine components, load resources, etc...
		void RunEngine(ApplicationHandle* applicationHandle, bool isEditor);
		
		// Destroy engine and all components
		// Application will be closed when this function is called
		void Destroy();

		[[nodiscard]] inline bool FindAppParam(std::string param) const;
		[[nodiscard]] inline ApplicationHandle* GetApplicationHandle() const;

		[[nodiscard]] static bool isEditor() noexcept;
	private:
		// Update input manager, check keys state
		void UpdateInput();

		// Engine hard codded keys input
		void UpdateEngineInputKeys();

		// Run game loop
		void RunLoop();
		void OnLoop();
		void OnGlobalUpdate(float DeltaTime);

		// TODO: Make engine resource json
		
		// Load engine resources
		// Configs, textures, shaders
		bool LoadEngineResources();

	
		ApplicationHandle* m_applicationHandle;
		static bool m_isEditor;
		std::string m_params;
	};
}