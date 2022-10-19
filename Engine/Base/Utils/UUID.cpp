#include "UUID.h"
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include <sstream>

namespace S2DE::Core::Utils
{
	UUID::UUID()
		:	m_uuid(boost::uuids::random_generator()())
	{

	}

	UUID::~UUID()
	{
		m_uuid = boost::uuids::nil_uuid();
	}
	
	void UUID::RegenerateUUID()
	{
		m_uuid = boost::uuids::random_generator()();
	}

	void UUID::SetUUID(boost::uuids::uuid uuid)
	{
		m_uuid = uuid;
	}

	void UUID::SetUUID(const std::string id_str)
	{
		m_uuid = boost::uuids::nil_uuid();
		std::istringstream strm(id_str);
		strm >> m_uuid;
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