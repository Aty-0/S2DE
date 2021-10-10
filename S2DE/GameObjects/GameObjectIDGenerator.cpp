#include "GameObjectIDGenerator.h"
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include <sstream>

namespace S2DE
{
	GameObjectIDGenerator::GameObjectIDGenerator()
		:	m_uuid(boost::uuids::random_generator()())
	{

	}

	GameObjectIDGenerator::~GameObjectIDGenerator()
	{
		m_uuid = boost::uuids::nil_uuid();
	}
	
	void GameObjectIDGenerator::RegenerateUUID()
	{
		m_uuid = boost::uuids::random_generator()();
	}

	void GameObjectIDGenerator::SetUUID(boost::uuids::uuid uuid)
	{
		m_uuid = uuid;
	}

	void GameObjectIDGenerator::SetUUID(const std::string id_str)
	{
		m_uuid = boost::uuids::nil_uuid();
		std::istringstream strm(id_str);
		strm >> m_uuid;
	}

	std::string GameObjectIDGenerator::GetIDString() const
	{
		return boost::uuids::to_string(m_uuid);
	}

	boost::uuids::uuid GameObjectIDGenerator::ConvertStringToUUID(const std::string id)
	{
		boost::uuids::uuid uuid = boost::uuids::nil_uuid();
		std::istringstream strm(id);
		strm >> uuid;
		return uuid;
	}

	std::string GameObjectIDGenerator::ConvertUUIDToString(boost::uuids::uuid uuid)
	{
		return boost::uuids::to_string(uuid);
	}
}