#pragma once
#include "Base/Main/Common.h"

namespace S2DE::Core::Resources
{
	class S2DE_API Resource
	{
	public:
		friend class ResourceManager;

		Resource();
		explicit Resource(std::string type, std::string ex);
		explicit Resource(std::string type, std::string ex[]);

		~Resource();

		virtual bool Load(std::string name);
		
		[[nodiscard]] virtual inline std::string GetName() const;
		[[nodiscard]] virtual inline std::string GetType() const;
		[[nodiscard]] virtual inline std::vector<std::string> GetExtensions() const;

	protected:
		virtual std::vector<std::string> FindPath(std::vector<std::string> names);

		std::string m_name;
		std::string m_type;
		std::vector<std::string> m_extensions;
		bool m_cantDelete;
		bool m_notLoaded;
	};
}