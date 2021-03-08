#include "userDao.hpp"

#include "dao/userDao/userModel.hpp"
#include "oatpp/core/data/stream/BufferStream.hpp"

#include <mongocxx/client.hpp>
#include <mongocxx/options/insert.hpp>
#include <mongocxx/exception/operation_exception.hpp>

namespace dao {

userDao::userDao(const 
    mongocxx::uri &uri, 
    const std::string &dbName, 
    const std::string &collectionName) 
    : m_pool(std::make_shared<mongocxx::pool>(uri)), 
      m_databaseName(dbName), 
      m_collectionName(collectionName)
{}

oatpp::Object<User> userDao::userFromDto(const oatpp::Object<UserDto>& dto) {
  auto user = User::createShared();
  user->_id = dto->username;
  user->username = dto->username;
  user->email = dto->email;
  user->password = dto->password;
  user->phone = dto->phone;
  user->role = dto->role;
  user->lastActivityDateTime = dto->lastActivityDateTime;
  user->isActive = dto->isActive;
  return user;
}

oatpp::Object<UserDto> userDao::dtoFromUser(const oatpp::Object<User>& user) {
  auto dto = UserDto::createShared();
  dto->username = user->username;
  dto->email = user->email;
  dto->password = user->password;
  dto->phone = user->phone;
  dto->role = user->role;
  dto->lastActivityDateTime = user->lastActivityDateTime;
  dto->isActive = user->isActive;
  return dto;
}

bsoncxx::document::value userDao::createMongoDocument(const oatpp::Void &polymorph) {
  // if you have huge docs, you may want to increase starting BufferOutputStream size.
  // Or you may want to use oatpp::data::stream::ChunkedBuffer instead - for no-copy growth.
  oatpp::data::stream::BufferOutputStream stream;
  m_objectMapper.write(&stream, polymorph);
  bsoncxx::document::view view(stream.getData(), stream.getCurrentPosition());
  return bsoncxx::document::value(view);
}

oatpp::Object<UserDto> userDao::createUser(const oatpp::Object<UserDto> &userDto) {
  auto conn = m_pool->acquire();
  auto collection = (*conn)[m_databaseName][m_collectionName];
  collection.insert_one(createMongoDocument(userFromDto(userDto)));
  return userDto;
}

oatpp::Object<UserDto> userDao::updateUser(const oatpp::Object<UserDto> &userDto) {
  auto conn = m_pool->acquire();
  auto collection = (*conn)[m_databaseName][m_collectionName];

  collection.update_one(
    createMongoDocument( // <-- Filter
      oatpp::Fields<oatpp::String>({
        {"_id", userDto->username}
      })
    ),
    createMongoDocument( // <-- Set
      oatpp::Fields<oatpp::Any>({ // map
        { // pair
          "$set", oatpp::Fields<oatpp::Any>({ // you can also define a "strict" DTO for $set operation.
            {"username", userDto->username},
            {"email", userDto->email},
            {"password", userDto->password},
            {"phone", userDto->phone},
            {"lastActivityDateTime", userDto->lastActivityDateTime},
            {"isActive", userDto->isActive},
            {"role", userDto->role}
          })
        } // pair
      }) // map
    )
  );

  return userDto;
}

oatpp::Object<UserDto> userDao::getUser(const oatpp::String& username) {
  auto conn = m_pool->acquire();
  auto collection = (*conn)[m_databaseName][m_collectionName];

  auto result =
    collection.find_one(createMongoDocument( // <-- Filter
      oatpp::Fields<oatpp::String>({
        {"_id", username}
      })
    ));

  if(result) {
    auto view = result->view();
    auto bson = oatpp::String((const char*)view.data(), view.length(), false /* to not copy view data */);
    auto user = m_objectMapper.readFromString<oatpp::Object<User>>(bson);
    return dtoFromUser(user);
  }

  return nullptr;
}

oatpp::Object<UserDto> userDao::login(const oatpp::String& username, const oatpp::String& password) {
  auto conn = m_pool->acquire();
  auto collection = (*conn)[m_databaseName][m_collectionName];

  auto result = collection.find_one(createMongoDocument(
      oatpp::Fields<oatpp::Any>({
        {"username", username},
        {"password", password}
      })
    ));

  if (result) {
    auto view = result->view();
    auto bson = oatpp::String((const char*)view.data(), view.length(), false /* to not copy view data */);
    auto user = m_objectMapper.readFromString<oatpp::Object<User>>(bson);
    return dtoFromUser(user);
  }

  return nullptr;
}

oatpp::List<oatpp::Object<UserDto>> userDao::getAllUsers() {
  auto conn = m_pool->acquire();
  auto collection = (*conn)[m_databaseName][m_collectionName];

  auto cursor = collection.find(
    createMongoDocument(oatpp::Fields<oatpp::String>({})
  ));

  oatpp::List<oatpp::Object<UserDto>> list({});

  for(auto view : cursor) {
    auto bson = oatpp::String((const char*)view.data(), view.length(), false /* to not copy view data */);
    auto user = m_objectMapper.readFromString<oatpp::Object<User>>(bson);
    list->push_back(dtoFromUser(user));
  }

  return list;

}

bool userDao::deleteUser(const oatpp::String& username) {
  auto conn = m_pool->acquire();
  auto collection = (*conn)[m_databaseName][m_collectionName];

  auto result =
    collection.delete_one(createMongoDocument( // <-- Filter
      oatpp::Fields<oatpp::String>({
        {"_id", username}
      })
    ));

  if(result) {
    return result->deleted_count() == 1;
  }
  return false;
}

}