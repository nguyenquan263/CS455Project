
#ifndef example_oatpp_mongo_AppointmentDTOs_hpp
#define example_oatpp_mongo_AppointmentDTOs_hpp

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class AppointmentDto : public oatpp::DTO {

    DTO_INIT(AppointmentDto, DTO)

    DTO_FIELD(String, _id);
    DTO_FIELD(String, tutor_id);
    DTO_FIELD(String, student_id);
    DTO_FIELD(String, course_id);
    DTO_FIELD(String, date);
    DTO_FIELD(String, from_time);
    DTO_FIELD(String, end_time);
    DTO_FIELD(Boolean, isActive);
    DTO_FIELD(Boolean, isCompleted);

};

#include OATPP_CODEGEN_END(DTO)

#endif /* example_oatpp_mongo_AppointmentDTOs_hpp */
