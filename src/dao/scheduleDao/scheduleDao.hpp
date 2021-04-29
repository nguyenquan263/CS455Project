#ifndef example_oatpp_mongo_ScheduleDao_hpp
#define example_oatpp_mongo_ScheduleDao_hpp

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
            oatpp::Object<Schedule> scheduleFromDto(const oatpp::Object<ScheduleDto>& dto);
            oatpp::Object<ScheduleDto> dtoFromSchedule(const oatpp::Object<Schedule>& schedule);
        private:
            bsoncxx::document::value createMongoDocument(const oatpp::Void &polymorph);
        public:

            scheduleDao(const mongocxx::uri &uri, const std::string &dbName, const std::string &collectionName);

            oatpp::Object<ScheduleDto> createSchedule(const oatpp::Object<ScheduleDto> &scheduleDto);
            oatpp::Object<ScheduleDto> updateSchedule(const oatpp::Object<ScheduleDto> &scheduleDto);
            oatpp::Object<ScheduleDto> getScheduleById(const oatpp::String& _id);
            oatpp::List<oatpp::Object<ScheduleDto>> getScheduleByWeekday(const oatpp::String& weekday);
            oatpp::List<oatpp::Object<ScheduleDto>> getAllSchedules();

            bool deleteSchedule(const oatpp::String& _id);
    };
}

#endif //example_oatpp_mongo_ScheduleDao_hpp
