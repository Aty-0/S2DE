#pragma once
#include "Base/Main/Common.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_serialize.hpp>

namespace S2DE::Core::Utils
{
	class S2DE_API UUID
	{
	public:
		UUID();
		~UUID();

		// Generate new UUID
		void RegenerateUUID();
		[[nodiscard]] static inline bool isUUIDValid(std::string const& uuidInString, boost::uuids::uuid& result);
		// Get current uuid in string format 
		[[nodiscard]] inline std::string GetUUIDString() const;
		// Get current uuid
		[[nodiscard]] inline boost::uuids::uuid GetUUID() const;
		// Convert string to uuid 
		[[nodiscard]] static inline boost::uuids::uuid ConvertStringToUUID(std::string id);
		// Convert uuid to string 
		[[nodiscard]] static inline std::string ConvertUUIDToString(boost::uuids::uuid uuid);

	protected:
		// Set uuid by string 
		bool SetUUID(std::string const& id_str);
		// Set uuid by existing uuid
		void SetUUID(boost::uuids::uuid uuid);
		// Use for serialization
		std::string m_uuidInStringFIXME;
	private:
		boost::uuids::uuid m_uuid;

	};
}
