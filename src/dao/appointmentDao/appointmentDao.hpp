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
            oatpp::Object<Appointment> appointmentFromDto(const oatpp::Object<AppointmentDTO>& dto);
            oatpp::Object<AppointmentDTO> dtoFromAppointment(const oatpp::Object<Appointment>& appointment);
        private:
            bsoncxx::document::value createMongoDocument(const oatpp::Void &polymorph);
        public:

            appointmentDao(const mongocxx::uri &uri, const std::string &dbName, const std::string &collectionName);

            oatpp::Object<AppointmentDTO> createAppointment(const oatpp::Object<AppointmentDTO> &appointmentDto);
            oatpp::Object<AppointmentDTO> updateAppointment(const oatpp::Object<AppointmentDTO> &appointmentDto);
            oatpp::Object<AppointmentDTO> getAppointmentById(const oatpp::String& _id);
            oatpp::List<oatpp::Object<AppointmentDTO>> getAppointmentByDate(const oatpp::String& date);
            oatpp::List<oatpp::Object<AppointmentDTO>> getAllAppointments();

            bool deleteAppointment(const oatpp::String& _id);
    };
}

#endif //example_oatpp_mongo_AppointmentDao_hpp