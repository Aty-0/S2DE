#pragma once
#pragma warning(disable:4996)
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>

#include <boost/serialization/access.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/split_free.hpp>

#include <boost/serialization/export.hpp>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <iomanip>
#include <sstream>

#define S2DE_SERIALIZE_BEGIN(BASE_CLASS_NAME) friend class boost::serialization::access; \
 template <typename Archive> void serialize(Archive& ar, const std::uint32_t version) \
 { ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(BASE_CLASS_NAME); 
#define S2DE_SERIALIZE_BASE_BEGIN() friend class boost::serialization::access; template <typename Archive> void serialize(Archive& ar, const std::uint32_t version)  {
#define S2DE_SERIALIZE_ADD(obj) ar & BOOST_SERIALIZATION_NVP(obj);	
#define S2DE_SERIALIZE_ADD_N(obj) ar & obj;	
#define S2DE_SERIALIZE_END() }
#define S2DE_SERIALIZE_CLASS_IMPLEMENTATION(ClassName, LevelType) BOOST_CLASS_IMPLEMENTATION(ClassName, LevelType)

/*

// std::type_index serialization type
struct nothing {};

using named_typeindex = std::tuple<std::string, std::type_index>;
std::vector<named_typeindex> name_register =
{
};

std::type_index type_for_name(const std::string& name)
{
    auto i = std::find_if(std::begin(name_register), std::end(name_register),
        [&name](const auto& entry) { return std::get<std::string>(entry) == name; });
    if (i == std::end(name_register))
        return typeid(nothing);
    return std::get<std::type_index>(*i);
}

std::string const& name_for_type(std::type_index type)
{
    auto i = std::find_if(std::begin(name_register), std::end(name_register),
        [type](const auto& entry) { return std::get<std::type_index>(entry) == type; });

    using namespace std::string_literals;
    if (i == std::end(name_register))
        throw std::logic_error("unregistered type "s + type.name());

    return std::get<std::string>(*i);
}

bool register_name(std::string name, std::type_index ti)
{
    if (type_for_name(name) == typeid(nothing))
    {
        name_register.push_back(std::make_tuple(std::move(name), ti));
        return true;
    }
    return false;
}

namespace boost
{
    namespace serialization 
    {

        template<class Archive>
        void save(Archive& ar, const std::type_index& t, unsigned int version)
        {
            ar << name_for_type(t);
        }

        template<class Archive>
        void load(Archive& ar, std::type_index& t, unsigned int version)
        {
            std::string s;
            ar >> s;
            t = type_for_name(s);
        }

    } // namespace serialization
} // namespace boost

BOOST_SERIALIZATION_SPLIT_FREE(std::type_index);*/