#ifndef example_oatpp_mongo_AvailableTimeDao_hpp
#define example_oatpp_mongo_AvailableTimeDao_hpp

#include "dto/availableTimeDTO.hpp"
#include "availableTimeModel.hpp"
#include "oatpp-mongo/bson/mapping/ObjectMapper.hpp"
#include <mongocxx/pool.hpp>
#include <bsoncxx/document/value.hpp>

namespace dao {

class availableTimeDao {
private:
  std::shared_ptr<mongocxx::pool> m_pool;
  std::string m_databaseName;
  std::string m_collectionName;
  oatpp::mongo::bson::mapping::ObjectMapper m_objectMapper;
private:
  oatpp::Object<AvailableTime> availableTimeFromDto(const oatpp::Object<AvailableTimeDTO>& dto);
  oatpp::Object<AvailableTimeDTO> dtoFromAvailableTime(const oatpp::Object<AvailableTime>& availableTime);
private:
  bsoncxx::document::value createMongoDocument(const oatpp::Void &polymorph);
public:

  availableTimeDao(const mongocxx::uri &uri, const std::string &dbName, const std::string &collectionName);

  oatpp::Object<AvailableTimeDTO> createAvailableTime(const oatpp::Object<AvailableTimeDTO> &availableTimeDto);
  oatpp::Object<AvailableTimeDTO> updateAvailableTime(const oatpp::Object<AvailableTimeDTO> &availableTimeDto);
  oatpp::Object<AvailableTimeDTO> getAvailableTimeByID(const oatpp::String& availableTimeObjectID);
  oatpp::List<oatpp::Object<AvailableTimeDTO>> getAvailableTime();
  oatpp::List<oatpp::Object<AvailableTimeDTO>> getAvailableTimeByTutorID(const oatpp::String& tutorID);
  bool deleteAvailableTime(const oatpp::String& availableTimeObjectID);

};

}

#endif //example_oatpp_mongo_Database_hpp
