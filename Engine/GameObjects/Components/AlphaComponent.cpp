#include "AlphaComponent.h"

namespace S2DE::GameObjects::Components
{
	void AlphaComponent::SetAlpha(bool alpha)
	{
		m_haveAlpha = alpha;
	}

	inline bool AlphaComponent::HaveAlpha() const
	{
		return m_haveAlpha;
	}
}