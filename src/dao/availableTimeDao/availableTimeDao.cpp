#include "availableTimeDao.hpp"

#include "dao/availableTimeDao/availableTimeModel.hpp"
#include "oatpp/core/data/stream/BufferStream.hpp"

#include <mongocxx/client.hpp>
#include <mongocxx/options/insert.hpp>
#include <mongocxx/exception/operation_exception.hpp>
#include <iostream>
#include "boost/date_time/posix_time/posix_time.hpp"

#include <boost/algorithm/string.hpp>
#include <string>

namespace dao {

availableTimeDao::availableTimeDao(const 
    mongocxx::uri &uri, 
    const std::string &dbName, 
    const std::string &collectionName) 
    : m_pool(std::make_shared<mongocxx::pool>(uri)), 
      m_databaseName(dbName), 
      m_collectionName(collectionName)
{}

oatpp::Object<AvailableTime> availableTimeDao::availableTimeFromDto(const oatpp::Object<AvailableTimeDTO> &dto) {
    auto availableTime = AvailableTime::createShared();

    //create MongoDB Object ID
    oatpp::mongo::bson::ObjectId objId = oatpp::mongo::bson::type::ObjectId();

    availableTime->_id = objId->toString();
    availableTime->tutor_id = dto->tutor_id;
    availableTime->date = dto->date;
    availableTime->from_time = dto->from_time;
    availableTime->to_time = dto->to_time;
    availableTime->isActive = dto->isActive;

    return availableTime;
}

oatpp::Object<AvailableTimeDTO> availableTimeDao::dtoFromAvailableTime(const oatpp::Object<AvailableTime>& availableTime) {
    auto dto = AvailableTimeDTO::createShared();

    dto->_id = availableTime->_id;
    dto->tutor_id = availableTime->tutor_id;
    dto->date = availableTime->date;
    dto->from_time = availableTime->from_time;
    dto->to_time = availableTime->to_time;
    dto->isActive = availableTime->isActive;
    
    return dto;
}

// object => json converter
bsoncxx::document::value availableTimeDao::createMongoDocument(const oatpp::Void &polymorph) {
  // if you have huge docs, you may want to increase starting BufferOutputStream size.
  // Or you may want to use oatpp::data::stream::ChunkedBuffer instead - for no-copy growth.
  oatpp::data::stream::BufferOutputStream stream;
  m_objectMapper.write(&stream, polymorph);
  bsoncxx::document::view view(stream.getData(), stream.getCurrentPosition());
  return bsoncxx::document::value(view);
}

oatpp::Object<AvailableTimeDTO> availableTimeDao::createAvailableTime(const oatpp::Object<AvailableTimeDTO> &availableTimeDto) {
  auto conn = m_pool->acquire();
  auto availableTimdCollection = (*conn)[m_databaseName][m_collectionName];
  auto userCollection = (*conn)["CS455"]["users"];
  
  //FIRST CONDITION: CHECK THE TUTOR_ID IS CORRECT OR NOT!
  //get the target tutor_id-------------------------------------------------------------------------------------
  std::string targetTutorID = availableTimeDto->tutor_id->c_str();
  oatpp::String oatppTargetTutorID = targetTutorID.c_str();
  //------------------------------------------------------------------------------------------------------------

  //get all tutor from database--------------------------------------------------------------------------------- 
  std::string tutorString = "ADMIN";
  oatpp::String oatppTutorString = tutorString.c_str();
  auto cursor = userCollection.find(
        createMongoDocument(
            oatpp::Fields<oatpp::Any>({ // map
                { // pair
                    "role", oatpp::Fields<oatpp::String>({ 
                        {"$regex", oatppTutorString}
                         
                    })
                }, // pair
                { "_id", oatppTargetTutorID }
            }) // map
        )
  );
  oatpp::List<oatpp::Object<AvailableTimeDTO>> list({});

  for(auto view : cursor) {
    auto bson = oatpp::String((const char*)view.data(), view.length(), false /* to not copy view data */);
    auto availableTime = m_objectMapper.readFromString<oatpp::Object<AvailableTime>>(bson);
    auto currentAvailableTimeDTO = dtoFromAvailableTime(availableTime); 

    list->push_back(currentAvailableTimeDTO);
  }

  int tutorCount = list->size();

  if (tutorCount == 1) {
      std::cout << "The available time is reference to a correct tutor!" << std::endl;
  } else {
      std::cout << "The available time is reference to an incorrect tutor!" << std::endl;
  }
  
  auto insertedAvailableTime = availableTimeFromDto(availableTimeDto);


  //SECOND CONDITION: CHECK THE DATE, FROM-TIME, AND END-TIME IS CORRECT OR NOT.
  

  // availableTimdCollection.insert_one(createMongoDocument(insertedAvailableTime));
  return dtoFromAvailableTime(insertedAvailableTime);
}


}
