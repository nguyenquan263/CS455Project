
#ifndef example_oatpp_mongo_ScheduleModel_hpp
#define example_oatpp_mongo_ScheduleModel_hpp

#include "dto/scheduleDTO.hpp"

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace dao {

class Schedule : public oatpp::DTO {

    DTO_INIT(Schedule, DTO);

    DTO_FIELD(String, _id);
    DTO_FIELD(String, tutorID);
    DTO_FIELD(String, fromDateTime);
    DTO_FIELD(String, toDateTime);
    DTO_FIELD(Boolean, isActive);

};

}

#include OATPP_CODEGEN_END(DTO)

#endif // example_oatpp_mongo_Model_hpp
