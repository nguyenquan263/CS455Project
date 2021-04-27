
#ifndef example_oatpp_mongo_AppointmentController_hpp
#define example_oatpp_mongo_AppointmentController_hpp

#include "dao/appointmentDao/appointmentDao.hpp"
#include "dto/appointmentDTO.hpp"
#include "dto/responseDTO.hpp"

#include "oatpp-swagger/Types.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

#include "oatpp/core/data/stream/BufferStream.hpp"

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"

#include <iostream>

#include OATPP_CODEGEN_BEGIN(ApiController)

class AppointmentController : public oatpp::web::server::api::ApiController {
    private:
        OATPP_COMPONENT(std::shared_ptr<dao::appointmentDao>, m_database);
    
    public:
        AppointmentController(const std::shared_ptr<ObjectMapper>& objectMapper)
            : oatpp::web::server::api::ApiController(objectMapper)
        {}
    public:

        static std::shared_ptr<AppointmentController> createShared(
            OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
            {
                return std::make_shared<AppointmentController>(objectMapper);
            }

        
        ENDPOINT_INFO(createAppointment) {
            info->summary = "Create a new Appointment";
            info->addConsumes<Object<AppointmentDTO>>("application/json");
            info->addResponse<Object<ResponseDTO>>(Status::CODE_200, "application/json");
        }
        ENDPOINT("POST", "/appointment", createAppointment,
                 BODY_DTO(Object<AppointmentDTO>, appointmentDto)) {
                     auto responseDTO = ResponseDTO::createShared();
                 try {
                     auto appointment = m_database->getAppointmentById(appointmentDto->_id);

                     if(appointment) {
                         responseDTO->errorCode = 404;
                         responseDTO->message = "Your appointment is duplicated";
                         responseDTO->data = NULL;
                     } else {
                         responseDTO->errorCode = 200;
                         responseDTO->message = "The appointment is created successfully.";
                         responseDTO->data = m_database->createAppointment(appointmentDto);
                     }
                 } catch(std::runtime_error &rte) {
                     responseDTO->errorCode = 500;
                     responseDTO->message = rte.what();
                     responseDTO->data = NULL;
                 }
                 return createDtoResponse(Status::CODE_200, responseDTO);
        }

        ENDPOINT_INFO(putAppointment) {
            info->summary = "Update Appointment by ID";
            info->addConsumes<Object<AppointmentDTO>>("application/json");
            info->addResponse<Object<AppointmentDTO>>(Status::CODE_200, "application/json");
            info->addResponse<String>(Status::CODE_404, "text/plain");
            info->pathParams["_id"].description = "target _id";
        }
        ENDPOINT("PUT", "appointment/{_id}", putAppointment,
                 PATH(String, _id),
                 BODY_DTO(Object<AppointmentDTO>, appointmentDto)) {
                     auto responseDTO = ResponseDTO::createShared();

                     try {
                        auto appointment = m_database->getAppointmentById(_id);

                        if (appointment) {
                            appointmentDto->_id = _id;
                            responseDTO->errorCode = 200;
                            responseDTO->message = "The course was updated successfully.";
                            responseDTO->data = m_database->updateAppointment(appointmentDto);
                        } else {
                            responseDTO->errorCode = 404;
                            responseDTO->message = "Cannot find the target appointment.";
                            responseDTO->data = NULL;
                        }
                     } catch(std::runtime_error &rte) {
                         responseDTO->errorCode = 500;
                         responseDTO->message = rte.what();
                         responseDTO->data = NULL;
                     }

                     return createDtoResponse(Status::CODE_200, responseDTO);
                 }
        
        ENDPOINT_INFO(getAppointment) {
            info->summary = "Get the appointment by id";
            info->addResponse<Object<AppointmentDTO>>(Status::CODE_200, "application/json");
            info->addResponse<Object<ResponseDTO>>(Status::CODE_404, "application/json");
            
            info->pathParams["_id"].description = "target id";
        }
        ENDPOINT("GET", "appointment/{_id}", getAppointment,
                 PATH(String, _id)) {
                     auto responseDTO = ResponseDTO::createShared();

                     try {
                         auto appointment = m_database->getAppointmentById(_id);
                         if(appointment) {
                             responseDTO->errorCode = 200;
                             responseDTO->message = "Target appointment found";
                             responseDTO->data = appointment;
                         } else {
                             responseDTO->errorCode = 404;
                             responseDTO->message = "Cannot find target appointment";
                             responseDTO->data = NULL;
                         }
                     } catch(std::runtime_error &rte) {
                         responseDTO->errorCode = 500;
                         responseDTO->message = rte.what();
                         responseDTO->data = NULL;
                     }

                     return createDtoResponse(Status::CODE_200, responseDTO);
                 }

        ENDPOINT_INFO(getAllAppointments) {
            info->summary = "Get all Appointments";
            info->addResponse<List<Object<ResponseDTO>>>(Status::CODE_200, "application/json");
        }
        ENDPOINT("GET", "appointment", getAllAppointments) {

            auto responseDTO = ResponseDTO::createShared();

            try {
                auto appointmentList = m_database->getAllAppointments();
                responseDTO->errorCode = 200;
                responseDTO->message = "This is the appointment list.";
                responseDTO->data = appointmentList;
            } catch(std::runtime_error &rte) {
                responseDTO->errorCode = 500;
                responseDTO->message = rte.what();
                responseDTO->data = NULL;
            }

            return createDtoResponse(Status::CODE_200, responseDTO);
        }

    ENDPOINT_INFO(getAppointmentByDate) {
            info->summary = "Get the appointment by date";
            info->addResponse<Object<AppointmentDTO>>(Status::CODE_200, "application/json");
            info->addResponse<Object<ResponseDTO>>(Status::CODE_404, "application/json");
            
            info->pathParams["date"].description = "target date";
        }
    ENDPOINT("GET", "appointment/{date}", getAppointmentByDate,
             PATH(String, date)) {
                 auto responseDTO = ResponseDTO::createShared();

                 try {
                     auto appointment = m_database->getAppointmentByDate(date);
                     if(appointment) {
                         responseDTO->errorCode = 200;
                         responseDTO->message = "Target appointment found";
                         responseDTO->data = appointment;
                     } else {
                         responseDTO->errorCode = 404;
                         responseDTO->message = "Cannot find target appointment";
                         responseDTO->data = NULL;
                     }
                 } catch(std::runtime_error &rte) {
                     responseDTO->errorCode = 500;
                     responseDTO->message = rte.what();
                     responseDTO->data = NULL;
                 }

                 return createDtoResponse(Status::CODE_200, responseDTO);
             }

    ENDPOINT_INFO(deleteAppointment) {
        info->summary = "Delete appointment by ID";
        info->addResponse<String>(Status::CODE_200, "application/json");
        info->pathParams["_id"].description = "target appointment id";
    }
    ENDPOINT("DELETE", "appointment/{_id}", deleteAppointment,
             PATH(String, _id)) {
                 auto responseDTO = ResponseDTO::createShared();

                 try {
                     bool success = m_database->deleteAppointment(_id);
                     if(success) {
                         responseDTO->errorCode = 200;
                         responseDTO->message = "The appointment with _id '" + _id + "' was deleted successfully.";
                         responseDTO->data = NULL;
                     } else {
                         responseDTO->errorCode = 404;
                         responseDTO->message = "Cannot find target appointment.";
                         responseDTO->data = NULL;
                     }
                 } catch(std::runtime_error &rte) {
                     responseDTO->errorCode = 500;
                     responseDTO->message = rte.what();
                     responseDTO->data = NULL;
                 }
                 return createDtoResponse(Status::CODE_200, responseDTO);
             }
};

#include OATPP_CODEGEN_END(ApiController)

#endif /* example_oatpp_mongo_AppointmentController_hpp */