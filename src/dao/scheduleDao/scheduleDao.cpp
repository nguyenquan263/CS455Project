#include "scheduleDao.hpp"

#include "dao/scheduleDao/scheduleModel.hpp"
#include "oatpp/core/data/stream/BufferStream.hpp"

#include <mongocxx/client.hpp>
#include <mongocxx/options/insert.hpp>
#include <mongocxx/exception/operation_exception.hpp>

namespace dao {

    scheduleDao::scheduleDao(const 
    mongocxx::uri &uri,
    const std::string &dbName,
    const std::string &collectionName)
    : m_pool(std::make_shared<mongocxx::pool>(uri)),
      m_databaseName(dbName),
      m_collectionName(collectionName)
    
{}
    oatpp::Object<Schedule> scheduleDao::scheduleFromDto(const oatpp::Object<ScheduleDto>& dto) {
        auto schedule = Schedule::createShared();
        oatpp::mongo::bson::ObjectId objId = oatpp::mongo::bson::type::ObjectId();
        schedule->_id = objId->toString();
        schedule->tutor_id = dto->tutor_id;
        schedule->date = dto->date;
        schedule->from_time = dto->from_time;
        schedule->end_time = dto->end_time;
        schedule->isActive = dto->isActive;
        return schedule;
    }

    oatpp::Object<ScheduleDto> scheduleDao::dtoFromSchedule(const oatpp::Object<Schedule>& schedule) {
        auto dto = ScheduleDto::createShared();
        dto->tutor_id = schedule->tutor_id;
        dto->date = schedule->date;
        dto->from_time = schedule->from_time;
        dto->end_time = schedule->end_time;
        dto->isActive = schedule->isActive;
        return dto;
    }

    bsoncxx::document::value scheduleDao::createMongoDocument(const oatpp::Void &polymorph) {
        oatpp::data::stream::BufferOutputStream stream;
        m_objectMapper.write(&stream, polymorph);
        bsoncxx::document::view view(stream.getData(), stream.getCurrentPosition());
        return bsoncxx::document::value(view);
    }
    
    oatpp::Object<ScheduleDto> scheduleDao::createSchedule(const oatpp::Object<ScheduleDto> &scheduleDto) {
        auto conn = m_pool->acquire();
        auto collection = (*conn)[m_databaseName][m_collectionName];
        collection.insert_one(createMongoDocument(scheduleFromDto(scheduleDto)));
        return scheduleDto;
    }

    oatpp::Object<ScheduleDto> scheduleDao::updateSchedule(const oatpp::Object<ScheduleDto> &scheduleDto) {
        auto conn = m_pool->acquire();
        auto collection = (*conn)[m_databaseName][m_collectionName];

        collection.update_one(
            createMongoDocument(
                oatpp::Fields<oatpp::String>({
                    {"tutor_id", scheduleDto->tutor_id}
                })
            ),
            createMongoDocument(
                oatpp::Fields<oatpp::Any>({
                    {
                        "$set", oatpp::Fields<oatpp::Any>({
                            {"tutor_id", scheduleDto->tutor_id},
                            {"date", scheduleDto->date},
                            {"from_time", scheduleDto->from_time},
                            {"end_time", scheduleDto->end_time},
                            {"isActive", scheduleDto->isActive}
                        })
                    }
                })
            )
        );

        return scheduleDto;
    }

    oatpp::Object<ScheduleDto> scheduleDao::getScheduleByTutorId(const oatpp::String& tutor_id) {
        auto conn = m_pool->acquire();
        auto collection = (*conn)[m_databaseName][m_collectionName];

        auto result = 
        collection.find_one(
            createMongoDocument(oatpp::Fields<oatpp::String>({
                {"tutor_id", tutor_id}
            }))
        );

        if(result) {
            auto view = result->view();
            auto bson = oatpp::String((const char*)view.data(), view.length(), false /* to not copy view data */);
            auto schedule = m_objectMapper.readFromString<oatpp::Object<Schedule>>(bson);
            return dtoFromSchedule(schedule);
        }

        return nullptr;
    }

    oatpp::List<oatpp::Object<ScheduleDto>> scheduleDao::getScheduleByDate(const oatpp::String& date) {
        auto conn = m_pool->acquire();
        auto collection = (*conn)[m_databaseName][m_collectionName];

        auto cursor = collection.find(
            createMongoDocument(oatpp::Fields<oatpp::String>({
                {"date", date}
            }))
        );

        oatpp::List<oatpp::Object<ScheduleDto>> list({});

        for(auto view : cursor) {
            auto bson = oatpp::String((const char*)view.data(), view.length(), false);
            auto schedule = m_objectMapper.readFromString<oatpp::Object<Schedule>>(bson);
            list->push_back(dtoFromSchedule(schedule));
        }

        return list;
    }

    oatpp::List<oatpp::Object<ScheduleDto>> scheduleDao::getAllSchedules() {
        auto conn = m_pool->acquire();
        auto collection = (*conn)[m_databaseName][m_collectionName];

        auto cursor = collection.find(
            createMongoDocument(oatpp::Fields<oatpp::String>({})
        ));

        oatpp::List<oatpp::Object<ScheduleDto>> list({});

        for(auto view : cursor) {
            auto bson = oatpp::String((const char*)view.data(), view.length(), false /* to not copy view data */);
            auto schedule = m_objectMapper.readFromString<oatpp::Object<Schedule>>(bson);
            list->push_back(dtoFromSchedule(schedule));
        }

        return list;
    }

    bool scheduleDao::deleteSchedule(const oatpp::String& tutor_id) {
        auto conn = m_pool->acquire();
        auto collection = (*conn)[m_databaseName][m_collectionName];

        auto result = 
            collection.delete_one(createMongoDocument(
                oatpp::Fields<oatpp::String>({
                    {"tutor_id", tutor_id}
                })
            ));

            if(result)
                return result->deleted_count() == 1;
            else
                return false;
    }
    

}