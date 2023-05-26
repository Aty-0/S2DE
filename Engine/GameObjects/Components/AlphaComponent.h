#pragma once
#include "GameObjects/Components/Component.h"

namespace S2DE::GameObjects::Components
{
	class S2DE_API AlphaComponent : public Component
	{
	public:
		AlphaComponent() = default;
		virtual ~AlphaComponent() { }

		void SetAlpha(bool alpha);

		[[nodiscard]] inline bool HaveAlpha() const;
	private:
		bool m_haveAlpha = false;
	};
}