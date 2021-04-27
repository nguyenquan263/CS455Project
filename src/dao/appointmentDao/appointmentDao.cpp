#include "appointmentDao.hpp"

#include "dao/appointmentDao/appointmentModel.hpp"
#include "oatpp/core/data/stream/BufferStream.hpp"

#include <mongocxx/client.hpp>
#include <mongocxx/options/insert.hpp>
#include <mongocxx/exception/operation_exception.hpp>

namespace dao {

    appointmentDao::appointmentDao(const 
    mongocxx::uri &uri,
    const std::string &dbName,
    const std::string &collectionName)
    : m_pool(std::make_shared<mongocxx::pool>(uri)),
      m_databaseName(dbName),
      m_collectionName(collectionName)
    
{}
    oatpp::Object<Appointment> appointmentDao::appointmentFromDto(const oatpp::Object<AppointmentDTO>& dto) {
        auto appointment = Appointment::createShared();
        oatpp::mongo::bson::ObjectId objId = oatpp::mongo::bson::type::ObjectId();
        appointment->_id = objId->toString();
        appointment->tutor_id = dto->tutor_id;
        appointment->student_id = dto->student_id;
        appointment->date = dto->date;
        appointment->from_time = dto->from_time;
        appointment->end_time = dto->end_time;
        appointment->isCompleted = dto->isCompleted;
        appointment->isActive = dto->isActive;
        return appointment;
    }

    oatpp::Object<AppointmentDTO> appointmentDao::dtoFromAppointment(const oatpp::Object<Appointment>& appointment) {
        auto dto = AppointmentDTO::createShared();
        dto->tutor_id = appointment->tutor_id;
        dto->student_id = appointment->student_id;
        dto->date = appointment->date;
        dto->from_time = appointment->from_time;
        dto->end_time = appointment->end_time;
        dto->isCompleted = appointment->isCompleted;
        dto->isActive = appointment->isActive;
        return dto;
    }

    bsoncxx::document::value appointmentDao::createMongoDocument(const oatpp::Void &polymorph) {
        oatpp::data::stream::BufferOutputStream stream;
        m_objectMapper.write(&stream, polymorph);
        bsoncxx::document::view view(stream.getData(), stream.getCurrentPosition());
        return bsoncxx::document::value(view);
    }
    
    oatpp::Object<AppointmentDTO> appointmentDao::createAppointment(const oatpp::Object<AppointmentDTO> &appointmentDto) {
        auto conn = m_pool->acquire();
        auto collection = (*conn)[m_databaseName][m_collectionName];
        collection.insert_one(createMongoDocument(appointmentFromDto(appointmentDto)));
        return appointmentDto;
    }

    oatpp::Object<AppointmentDTO> appointmentDao::updateAppointment(const oatpp::Object<AppointmentDTO> &appointmentDto) {
        auto conn = m_pool->acquire();
        auto collection = (*conn)[m_databaseName][m_collectionName];

        collection.update_one(
            createMongoDocument(
                oatpp::Fields<oatpp::String>({
                    {"_id", appointmentDto->_id}
                })
            ),
            createMongoDocument(
                oatpp::Fields<oatpp::Any>({
                    {
                        "$set", oatpp::Fields<oatpp::Any>({
                            {"tutor_id", appointmentDto->tutor_id},
                            {"student_id", appointmentDto->student_id},
                            {"date", appointmentDto->date},
                            {"from_time", appointmentDto->from_time},
                            {"end_time", appointmentDto->end_time},
                            {"isCompleted", appointmentDto->isCompleted},
                            {"isActive", appointmentDto->isActive}
                        })
                    }
                })
            )
        );

        return appointmentDto;
    }

    oatpp::Object<AppointmentDTO> appointmentDao::getAppointmentById(const oatpp::String& _id) {
        auto conn = m_pool->acquire();
        auto collection = (*conn)[m_databaseName][m_collectionName];

        auto result = 
        collection.find_one(
            createMongoDocument(oatpp::Fields<oatpp::String>({
                {"_id", _id}
            }))
        );

        if(result) {
            auto view = result->view();
            auto bson = oatpp::String((const char*)view.data(), view.length(), false /* to not copy view data */);
            auto appointment = m_objectMapper.readFromString<oatpp::Object<Appointment>>(bson);
            return dtoFromAppointment(appointment);
        }

        return nullptr;
    }

    oatpp::List<oatpp::Object<AppointmentDTO>> appointmentDao::getAppointmentByDate(const oatpp::String& date) {
        auto conn = m_pool->acquire();
        auto collection = (*conn)[m_databaseName][m_collectionName];

        auto cursor = collection.find(
            createMongoDocument(oatpp::Fields<oatpp::String>({
                {"date", date}
            }))
        );

        oatpp::List<oatpp::Object<AppointmentDTO>> list({});

        for(auto view : cursor) {
            auto bson = oatpp::String((const char*)view.data(), view.length(), false);
            auto appointment = m_objectMapper.readFromString<oatpp::Object<Appointment>>(bson);
            list->push_back(dtoFromAppointment(appointment));
        }

        return list;
    }

    oatpp::List<oatpp::Object<AppointmentDTO>> appointmentDao::getAllAppointments() {
        auto conn = m_pool->acquire();
        auto collection = (*conn)[m_databaseName][m_collectionName];

        auto cursor = collection.find(
            createMongoDocument(oatpp::Fields<oatpp::String>({})
        ));

        oatpp::List<oatpp::Object<AppointmentDTO>> list({});

        for(auto view : cursor) {
            auto bson = oatpp::String((const char*)view.data(), view.length(), false /* to not copy view data */);
            auto appointment = m_objectMapper.readFromString<oatpp::Object<Appointment>>(bson);
            list->push_back(dtoFromAppointment(appointment));
        }

        return list;
    }

    bool appointmentDao::deleteAppointment(const oatpp::String& _id) {
        auto conn = m_pool->acquire();
        auto collection = (*conn)[m_databaseName][m_collectionName];

        auto result = 
            collection.delete_one(createMongoDocument(
                oatpp::Fields<oatpp::String>({
                    {"_id", _id}
                })
            ));

            if(result)
                return result->deleted_count() == 1;
            else
                return false;
    }
    

}