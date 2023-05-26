#include "UUID.h"
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include <sstream>

namespace S2DE::Core::Utils
{
	UUID::UUID()
		:	m_uuid(boost::uuids::random_generator()())
	{
		m_uuidInStringFIXME = GetUUIDString();
	}

	UUID::~UUID()
	{
		m_uuid = boost::uuids::nil_uuid();
	}
	
	void UUID::RegenerateUUID()
	{
		m_uuid = boost::uuids::random_generator()();

		m_uuidInStringFIXME = GetUUIDString();
	}

	bool UUID::isUUIDValid(std::string const& uuidInString, boost::uuids::uuid& result)
	{
		try
		{
			result = boost::uuids::string_generator()(uuidInString);
			return result.version() != boost::uuids::uuid::version_unknown;
		}
		catch (...)
		{
			return false;
		}
	}

	void UUID::SetUUID(boost::uuids::uuid uuid)
	{
		m_uuid = uuid;

		m_uuidInStringFIXME = GetUUIDString();
	}

	bool UUID::SetUUID(std::string const& id_str)
	{
		m_uuid = boost::uuids::nil_uuid();

		if (isUUIDValid(id_str, m_uuid))
		{
			//std::istringstream strm(id_str);
			//strm >> m_uuid;

			m_uuidInStringFIXME = GetUUIDString();
			return true;
		}
		else
		{
			RegenerateUUID();
			return false;
		}
	}

	inline boost::uuids::uuid UUID::GetUUID() const
	{
		return m_uuid;
	}

	inline std::string UUID::GetUUIDString() const
	{
		return boost::uuids::to_string(m_uuid);
	}

	boost::uuids::uuid UUID::ConvertStringToUUID(const std::string id)
	{
		boost::uuids::uuid uuid = boost::uuids::nil_uuid();
		std::istringstream strm(id);
		strm >> uuid;
		return uuid;
	}

	std::string UUID::ConvertUUIDToString(boost::uuids::uuid uuid)
	{
		return boost::uuids::to_string(uuid);
	}
}