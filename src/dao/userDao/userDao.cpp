#include "userDao.hpp"

#include "dao/userDao/userModel.hpp"
#include "oatpp/core/data/stream/BufferStream.hpp"

#include <mongocxx/client.hpp>
#include <mongocxx/options/insert.hpp>
#include <mongocxx/exception/operation_exception.hpp>
#include <iostream>
#include "boost/date_time/posix_time/posix_time.hpp"

#include <boost/algorithm/string.hpp>
#include <string>
#include "../../helper/encryption/encrypt.h"


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

  //create MongoDB Object ID
  oatpp::mongo::bson::ObjectId objId = oatpp::mongo::bson::type::ObjectId();

  

  user->_id = objId->toString();
  user->username = dto->username;
  user->email = dto->email;
  //create encrypted password
  std::string userPassword = dto->password->std_str();
  std::string testKey = "UNA";
  std::string encryptedPass = encrypt(userPassword, testKey);
  oatpp::String oatppEncryptedPassword = encryptedPass.c_str();
  //-------------------------
  user->password = oatppEncryptedPassword;
  user->phone = dto->phone;
  user->role = dto->role;

  boost::posix_time::ptime t(boost::posix_time::second_clock::local_time());
  oatpp::String currentDateTime = to_simple_string(t).c_str();

  user->lastActivityDateTime = currentDateTime;
  user->isActive = dto->isActive;
  return user;
}

oatpp::Object<UserDto> userDao::dtoFromUser(const oatpp::Object<User>& user) {
  auto dto = UserDto::createShared();

  dto->_id = user->_id;
  dto->username = user->username;
  dto->email = user->email;
  dto->password = user->password;
  dto->phone = user->phone;
  dto->role = user->role;
  dto->lastActivityDateTime = user->lastActivityDateTime;
  dto->isActive = user->isActive;
  return dto;
}

// object => json converter
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

  auto insertedUser = userFromDto(userDto);

  collection.insert_one(createMongoDocument(insertedUser));
  return dtoFromUser(insertedUser);
}

oatpp::Object<UserDto> userDao::updateUser(const oatpp::Object<UserDto> &userDto) {
  auto conn = m_pool->acquire();
  auto collection = (*conn)[m_databaseName][m_collectionName];

  //get current period
  boost::posix_time::ptime t(boost::posix_time::second_clock::local_time());
  oatpp::String currentDateTime = to_simple_string(t).c_str();

  collection.update_one(
    createMongoDocument( // <-- Filter
      oatpp::Fields<oatpp::String>({
        {"_id", userDto->_id}
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
            {"lastActivityDateTime", currentDateTime},
            {"isActive", userDto->isActive},
            {"role", userDto->role}
          })
        } // pair
      }) // map
    )
  );

  auto result =
    collection.find_one(createMongoDocument( // <-- Filter
      oatpp::Fields<oatpp::String>({
        {"_id", userDto->_id}
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

oatpp::Object<UserDto> userDao::getUserByID(const oatpp::String& userObjectID) {
  auto conn = m_pool->acquire();
  auto collection = (*conn)[m_databaseName][m_collectionName];

  auto result =
    collection.find_one(createMongoDocument( // <-- Filter
      oatpp::Fields<oatpp::String>({
        {"_id", userObjectID}
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

oatpp::Object<UserDto> userDao::getUserByUsername(const oatpp::String& username) {
  auto conn = m_pool->acquire();
  auto collection = (*conn)[m_databaseName][m_collectionName];

  auto result =
    collection.find_one(createMongoDocument( // <-- Filter
      oatpp::Fields<oatpp::String>({
        {"username", username}
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

  //encrypt the input password
  std::string userPassword = password->std_str();
  std::string testKey = "UNA";
  std::string encryptedPass = encrypt(userPassword, testKey);
  oatpp::String oatppEncryptedPassword = encryptedPass.c_str();
  
  //--------------------------

  auto result = collection.find_one(createMongoDocument(
      oatpp::Fields<oatpp::Any>({
        {"username", username},
        {"password", oatppEncryptedPassword}
      })
    ));

  if (result) {
    //get current period, for updating to user activity history
    boost::posix_time::ptime t(boost::posix_time::second_clock::local_time());
    oatpp::String currentDateTime = to_simple_string(t).c_str();


    //if login successfully, then update the activity date-time history
    collection.update_one(
      createMongoDocument( // <-- Filter
        oatpp::Fields<oatpp::Any>({
          {"username", username},
          {"password", oatppEncryptedPassword}
        })
      ),
      createMongoDocument( // <-- Set
        oatpp::Fields<oatpp::Any>({ // map
          { // pair
            "$set", oatpp::Fields<oatpp::String>({ 
              {"lastActivityDateTime", currentDateTime}
            
            })
          } // pair
        }) // map
      )
    );
    //----------------------

    //get the updated user
    auto updatedResult = collection.find_one(createMongoDocument(
      oatpp::Fields<oatpp::Any>({
        {"username", username},
        {"password", oatppEncryptedPassword}
      })
    ));

    auto view = updatedResult->view();
    auto bson = oatpp::String((const char*)view.data(), view.length(), false /* to not copy view data */);
    auto user = m_objectMapper.readFromString<oatpp::Object<User>>(bson);
    return dtoFromUser(user);
  }

  return nullptr;
}

//get all user from database
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

//get all user by role
oatpp::List<oatpp::Object<UserDto>> userDao::getAllUsersByRole(const oatpp::String& role) {
  auto conn = m_pool->acquire();
  auto collection = (*conn)[m_databaseName][m_collectionName];


  auto cursor = collection.find(
    createMongoDocument(
      oatpp::Fields<oatpp::Any>({ // map
        { // pair
          "role", oatpp::Fields<oatpp::String>({ 
            {"$regex", role->toUpperCase()}
          
          })
        } // pair
      }) // map

    )
  );

  oatpp::List<oatpp::Object<UserDto>> list({});

  for(auto view : cursor) {
    auto bson = oatpp::String((const char*)view.data(), view.length(), false /* to not copy view data */);
    auto user = m_objectMapper.readFromString<oatpp::Object<User>>(bson);
    list->push_back(dtoFromUser(user));
  }

  return list;

}

//delete user by userObjectID
bool userDao::deleteUser(const oatpp::String& userObjectID) {
  auto conn = m_pool->acquire();
  auto collection = (*conn)[m_databaseName][m_collectionName];

  auto result =
    collection.delete_one(createMongoDocument( // <-- Filter
      oatpp::Fields<oatpp::String>({
        {"_id", userObjectID}
      })
    ));

  if(result) {
    return result->deleted_count() == 1;
  }
  return false;
}

}