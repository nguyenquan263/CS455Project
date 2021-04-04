#ifndef example_oatpp_mongo_UserDao_hpp
#define example_oatpp_mongo_UserDao_hpp

#include "dto/scheduleDTO.hpp"
#include "scheduleModel.hpp"

#include "oatpp-mongo/bson/mapping/ObjectMapper.hpp"

#include <mongocxx/pool.hpp>
#include <bsoncxx/document/value.hpp>

namespace dao {

class scheduleDao {
private:
  std::shared_ptr<mongocxx::pool> m_pool;
  std::string m_databaseName;
  std::string m_collectionName;
  oatpp::mongo::bson::mapping::ObjectMapper m_objectMapper;
private:
  oatpp::Object<Schedule> scheduleFromDto(const oatpp::Object<ScheduleDTO>& dto);
  oatpp::Object<ScheduleDTO> dtoFromSchedule(const oatpp::Object<Schedule>& schedule);
private:
  bsoncxx::document::value createMongoDocument(const oatpp::Void &polymorph);
public:

  scheduleDao(const mongocxx::uri &uri, const std::string &dbName, const std::string &collectionName);

  oatpp::Object<ScheduleDTO> createSchedule(const oatpp::Object<ScheduleDTO> &scheduleDto);
  oatpp::Object<ScheduleDTO> updateSchedule(const oatpp::Object<ScheduleDTO> &scheduleDto);
  oatpp::List<oatpp::Object<ScheduleDto>> getAllSchedules();
  oatpp::List<oatpp::Object<ScheduleDto>> getAllSchedulesByTutorID(const oatpp::String& tutorID);
  bool deleteSchedule(const oatpp::String& scheduleID);

};

}

#endif //example_oatpp_mongo_Database_hpp
