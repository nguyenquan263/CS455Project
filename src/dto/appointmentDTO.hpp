#ifndef example_oatpp_mongo_AppointmentDTOs_hpp
#define example_oatpp_mongo_AppointmentDTOs_hpp

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class AppointmentDTO : public oatpp::DTO {

    DTO_INIT(AppointmentDTO, DTO)

    DTO_FIELD(String, _id);
    DTO_FIELD(String, tutor_id);
    DTO_FIELD(String, student_id);
    DTO_FIELD(String, date);
    DTO_FIELD(String, from_time);
    DTO_FIELD(String, end_time);
    DTO_FIELD(Boolean, isCompleted);
    DTO_FIELD(Boolean, isActive);

};

#include OATPP_CODEGEN_END(DTO)

#endif /* example_oatpp_mongo_AppointmentDTOs_hpp */