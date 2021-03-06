
#ifndef example_oatpp_mongo_ScheduleController_hpp
#define example_oatpp_mongo_ScheduleController_hpp

#include "dao/scheduleDao/scheduleDao.hpp"
#include "dto/scheduleDTO.hpp"
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

class ScheduleController : public oatpp::web::server::api::ApiController {
    private:
        OATPP_COMPONENT(std::shared_ptr<dao::scheduleDao>, m_database);
    
    public:
        ScheduleController(const std::shared_ptr<ObjectMapper>& objectMapper)
            : oatpp::web::server::api::ApiController(objectMapper)
        {}
    public:

        static std::shared_ptr<ScheduleController> createShared(
            OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
            {
                return std::make_shared<ScheduleController>(objectMapper);
            }

        
        ENDPOINT_INFO(createSchedule) {
            info->summary = "Create a new Schedule";
            info->addConsumes<Object<ScheduleDto>>("application/json");
            info->addResponse<Object<ResponseDTO>>(Status::CODE_200, "application/json");
        }
        ENDPOINT("POST", "/schedule", createSchedule,
                 BODY_DTO(Object<ScheduleDto>, scheduleDTO)) {
                     auto responseDTO = ResponseDTO::createShared();
                 try {
                     auto schedule = m_database->getScheduleById(scheduleDTO->_id);

                     if(schedule) {
                         responseDTO->errorCode = 404;
                         responseDTO->message = "Your schedule is duplicated";
                         responseDTO->data = NULL;
                     } else {
                         responseDTO->errorCode = 200;
                         responseDTO->message = "The schedule is created successfully.";
                         responseDTO->data = m_database->createSchedule(scheduleDTO);
                     }
                 } catch(std::runtime_error &rte) {
                     responseDTO->errorCode = 500;
                     responseDTO->message = rte.what();
                     responseDTO->data = NULL;
                 }
                 return createDtoResponse(Status::CODE_200, responseDTO);
        }

        ENDPOINT_INFO(putSchedule) {
            info->summary = "Update Schedule by ID";
            info->addConsumes<Object<ScheduleDto>>("application/json");
            info->addResponse<Object<ScheduleDto>>(Status::CODE_200, "application/json");
            info->addResponse<String>(Status::CODE_404, "text/plain");
            info->pathParams["_id"].description = "target _id";
        }
        ENDPOINT("PUT", "schedule/{_id}", putSchedule,
                 PATH(String, _id),
                 BODY_DTO(Object<ScheduleDto>, scheduleDTO)) {
                     auto responseDTO = ResponseDTO::createShared();

                     try {
                        auto schedule = m_database->getScheduleById(_id);

                        if (schedule) {
                            scheduleDTO->_id = _id;
                            responseDTO->errorCode = 200;
                            responseDTO->message = "The course was updated successfully.";
                            responseDTO->data = m_database->updateSchedule(scheduleDTO);
                        } else {
                            responseDTO->errorCode = 404;
                            responseDTO->message = "Cannot find the target schedule.";
                            responseDTO->data = NULL;
                        }
                     } catch(std::runtime_error &rte) {
                         responseDTO->errorCode = 500;
                         responseDTO->message = rte.what();
                         responseDTO->data = NULL;
                     }

                     return createDtoResponse(Status::CODE_200, responseDTO);
                 }
        
        ENDPOINT_INFO(getSchedule) {
            info->summary = "Get the schedule by  id";
            info->addResponse<Object<ScheduleDto>>(Status::CODE_200, "application/json");
            info->addResponse<Object<ResponseDTO>>(Status::CODE_404, "application/json");
            
            info->pathParams["_id"].description = "target  id";
        }
        ENDPOINT("GET", "schedule/{_id}", getSchedule,
                 PATH(String, _id)) {
                     auto responseDTO = ResponseDTO::createShared();

                     try {
                         auto schedule = m_database->getScheduleById(_id);
                         if(schedule) {
                             responseDTO->errorCode = 200;
                             responseDTO->message = "Target schedule found";
                             responseDTO->data = schedule;
                         } else {
                             responseDTO->errorCode = 404;
                             responseDTO->message = "Cannot find target schedule";
                             responseDTO->data = NULL;
                         }
                     } catch(std::runtime_error &rte) {
                         responseDTO->errorCode = 500;
                         responseDTO->message = rte.what();
                         responseDTO->data = NULL;
                     }

                     return createDtoResponse(Status::CODE_200, responseDTO);
                 }

        ENDPOINT_INFO(getAllSchedules) {
            info->summary = "Get all schedules";
            info->addResponse<List<Object<ResponseDTO>>>(Status::CODE_200, "application/json");
        }
        ENDPOINT("GET", "schedule", getAllSchedules) {

            auto responseDTO = ResponseDTO::createShared();

            try {
                auto scheduleList = m_database->getAllSchedules();
                responseDTO->errorCode = 200;
                responseDTO->message = "This is the schedule list.";
                responseDTO->data = scheduleList;
            } catch(std::runtime_error &rte) {
                responseDTO->errorCode = 500;
                responseDTO->message = rte.what();
                responseDTO->data = NULL;
            }

            return createDtoResponse(Status::CODE_200, responseDTO);
        }

    ENDPOINT_INFO(getScheduleByWeekday) {
            info->summary = "Get the schedule by weekday";
            info->addResponse<Object<ScheduleDto>>(Status::CODE_200, "application/json");
            info->addResponse<Object<ResponseDTO>>(Status::CODE_404, "application/json");
            
            info->pathParams["weekday"].description = "target weekday";
        }
    ENDPOINT("GET", "schedule/{weekday}", getScheduleByWeekday,
             PATH(String, weekday)) {
                 auto responseDTO = ResponseDTO::createShared();

                 try {
                     auto schedule = m_database->getScheduleByWeekday(weekday);
                     if(schedule) {
                         responseDTO->errorCode = 200;
                         responseDTO->message = "Target schedule found";
                         responseDTO->data = schedule;
                     } else {
                         responseDTO->errorCode = 404;
                         responseDTO->message = "Cannot find target schedule";
                         responseDTO->data = NULL;
                     }
                 } catch(std::runtime_error &rte) {
                     responseDTO->errorCode = 500;
                     responseDTO->message = rte.what();
                     responseDTO->data = NULL;
                 }

                 return createDtoResponse(Status::CODE_200, responseDTO);
             }

    ENDPOINT_INFO(getScheduleByTutorId) {
            info->summary = "Get the schedule by Tutor ID";
            info->addResponse<Object<ScheduleDto>>(Status::CODE_200, "application/json");
            info->addResponse<Object<ResponseDTO>>(Status::CODE_404, "application/json");
            
            info->pathParams["tutor_id"].description = "target tutor_id";
        }
    ENDPOINT("GET", "schedule/{tutor_id}", getScheduleByTutorId,
             PATH(String, tutor_id)) {
                 auto responseDTO = ResponseDTO::createShared();

                 try {
                     auto schedule = m_database->getScheduleByTutorId(tutor_id);
                     if(schedule) {
                         responseDTO->errorCode = 200;
                         responseDTO->message = "Target schedule found";
                         responseDTO->data = schedule;
                     } else {
                         responseDTO->errorCode = 404;
                         responseDTO->message = "Cannot find target schedule";
                         responseDTO->data = NULL;
                     }
                 } catch(std::runtime_error &rte) {
                     responseDTO->errorCode = 500;
                     responseDTO->message = rte.what();
                     responseDTO->data = NULL;
                 }

                 return createDtoResponse(Status::CODE_200, responseDTO);
             }

             
    ENDPOINT_INFO(deleteSchedule) {
        info->summary = "Delete Schedule by ID";
        info->addResponse<String>(Status::CODE_200, "application/json");
        info->pathParams["_id"].description = "target schedule _id";
    }
    ENDPOINT("DELETE", "schedule/{_id}", deleteSchedule,
             PATH(String, _id)) {
                 auto responseDTO = ResponseDTO::createShared();

                 try {
                     bool success = m_database->deleteSchedule(_id);
                     if(success) {
                         responseDTO->errorCode = 200;
                         responseDTO->message = "The schedule with _id '" + _id + "' was deleted successfully.";
                         responseDTO->data = NULL;
                     } else {
                         responseDTO->errorCode = 404;
                         responseDTO->message = "Cannot find target schedule.";
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

#endif /* example_oatpp_mongo_ScheduleController_hpp */
