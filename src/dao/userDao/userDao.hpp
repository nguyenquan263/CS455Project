#ifndef example_oatpp_mongo_UserDao_hpp
#define example_oatpp_mongo_UserDao_hpp

#include "dto/userDTO.hpp"
#include "userModel.hpp"
#include "oatpp-mongo/bson/mapping/ObjectMapper.hpp"
#include <mongocxx/pool.hpp>
#include <bsoncxx/document/value.hpp>

namespace dao {

class userDao {
private:
  std::shared_ptr<mongocxx::pool> m_pool;
  std::string m_databaseName;
  std::string m_collectionName;
  oatpp::mongo::bson::mapping::ObjectMapper m_objectMapper;
private:
  oatpp::Object<User> userFromDto(const oatpp::Object<UserDto>& dto);
  oatpp::Object<UserDto> dtoFromUser(const oatpp::Object<User>& user);
private:
  bsoncxx::document::value createMongoDocument(const oatpp::Void &polymorph);
public:

  userDao(const mongocxx::uri &uri, const std::string &dbName, const std::string &collectionName);

  oatpp::Object<UserDto> createUser(const oatpp::Object<UserDto> &userDto);
  oatpp::Object<UserDto> updateUser(const oatpp::Object<UserDto> &userDto);
  oatpp::Object<UserDto> getUserByID(const oatpp::String& userObjectID);
  oatpp::Object<UserDto> getUserByUsername(const oatpp::String& username);
  oatpp::List<oatpp::Object<UserDto>> getAllUsers();
  oatpp::List<oatpp::Object<UserDto>> getAllUsersByRole(const oatpp::String& role);
  oatpp::Object<UserDto> login(const oatpp::String& username, const oatpp::String& password);
  bool deleteUser(const oatpp::String& userObjectID);

};

}

#endif //example_oatpp_mongo_Database_hpp
