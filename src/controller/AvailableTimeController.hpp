#ifndef example_oatpp_mongo_AvailableTimeController_hpp
#define example_oatpp_mongo_AvailableTimeController_hpp

#include "dao/availableTimeDao/availableTimeDao.hpp"
#include "dto/availableTimeDTO.hpp"

#include "oatpp-swagger/Types.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

#include "oatpp/core/data/stream/BufferStream.hpp"

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"
#include <iostream>


#include OATPP_CODEGEN_BEGIN(ApiController)

class AvailableTimeController : public oatpp::web::server::api::ApiController {
    private:
        OATPP_COMPONENT(std::shared_ptr<dao::availableTimeDao>, m_database);
    public:
        AvailableTimeController(const std::shared_ptr<ObjectMapper> &objectMapper)
            : oatpp::web::server::api::ApiController(objectMapper)
        {}

    public:
        static std::shared_ptr<AvailableTimeController> createShared(
            OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)
        ) {
            return std::make_shared<AvailableTimeController>(objectMapper);
        }

        ENDPOINT_INFO(createAvailableTime) {
            info->summary = "Create a new Available Time";
            info->addConsumes<Object<AvailableTimeDTO>>("application/json");
            info->addResponse<Object<ResponseDTO>>(Status::CODE_200, "application/json");
        }
        ADD_CORS(createAvailableTime)
        ENDPOINT("POST", "/availableTime", createAvailableTime,
                BODY_DTO(Object<AvailableTimeDTO>, availableTimeDto)) {
            auto responseDTO = ResponseDTO::createShared();
            try {
                //check the user is existed or not       
                responseDTO->errorCode = 200;
                responseDTO->message = "The available time is created successfully.";
                responseDTO->data = m_database->createAvailableTime(availableTimeDto);

            } catch(std::runtime_error &rte) {
                responseDTO->errorCode = 500;
                responseDTO->message = rte.what();
                responseDTO->data = NULL;
            }
            return createDtoResponse(Status::CODE_200, responseDTO);
        }
};

#include OATPP_CODEGEN_END(ApiController)

#endif /* example_oatpp_mongo_UserController_hpp */