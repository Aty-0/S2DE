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
		ZeroMemory(&m_uuid, sizeof(m_uuid));
	}
	
	void GameObjectIDGenerator::SetID(const std::string id_str)
	{
		ZeroMemory(&m_uuid, sizeof(m_uuid));
		std::istringstream strm(id_str);
		strm >> m_uuid;
	}

	std::string GameObjectIDGenerator::GetIDString() const
	{
		return boost::uuids::to_string(m_uuid);
	}

	boost::uuids::uuid GameObjectIDGenerator::ConvertStringToUUID(const std::string id)
	{
		boost::uuids::uuid uuid;
		ZeroMemory(&uuid, sizeof(uuid));
		std::istringstream strm(id);
		strm >> uuid;
		return uuid;
	}
}