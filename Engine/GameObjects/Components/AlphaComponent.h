#pragma once
#include "GameObjects/Components/Component.h"

namespace S2DE::GameObjects::Components
{
	class S2DE_API AlphaComponent : public Component
	{
	public:
		AlphaComponent() = default;
		virtual ~AlphaComponent() { }

		inline bool HaveAlpha() const { return m_haveAlpha; }
		void SetAlpha(bool alpha) { m_haveAlpha = alpha; }

	private:
		bool m_haveAlpha;
	};
}