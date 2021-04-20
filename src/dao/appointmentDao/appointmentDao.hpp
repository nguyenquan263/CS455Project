
#ifndef example_oatpp_mongo_AppointmentDao_hpp
#define example_oatpp_mongo_AppointmentDao_hpp

#include "dto/appointmentDTO.hpp"
#include "appointmentModel.hpp"

#include "oatpp-mongo/bson/mapping/ObjectMapper.hpp"

#include <mongocxx/pool.hpp>
#include <bsoncxx/document/value.hpp>

namespace dao {

    class appointmentDao {
        private:
            std::shared_ptr<mongocxx::pool> m_pool;
            std::string m_databaseName;
            std::string m_collectionName;
            oatpp::mongo::bson::mapping::ObjectMapper m_objectMapper;
        private:
            oatpp::Object<Appointment> appointmentFromDto(const oatpp::Object<AppointmentDto>& dto);
            oatpp::Object<AppointmentDto> dtoFromAppointment(const oatpp::Object<Appointment>& appointment);
        private:
            bsoncxx::document::value createMongoDocument(const oatpp::Void &polymorph);
        public:

            appointmentDao(const mongocxx::uri &uri, const std::string &dbName, const std::string &collectionName);

            oatpp::Object<AppointmentDto> createAppointment(const oatpp::Object<AppointmentDto> &appointmentDto);
            oatpp::Object<AppointmentDto> updateAppointment(const oatpp::Object<AppointmentDto> &appointmentDto);
            oatpp::Object<AppointmentDto> getAppointmentByTutorId(const oatpp::String& tutor_id);
            oatpp::List<oatpp::Object<AppointmentDto>> getAppointmentByDate(const oatpp::String& date);
            oatpp::List<oatpp::Object<AppointmentDto>> getAllAppointments();

            bool deleteAppointment(const oatpp::String& tutor_id);
    };
}

#endif //example_oatpp_mongo_AppointmentDao_hpp
