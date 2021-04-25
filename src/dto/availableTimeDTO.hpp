#ifndef example_oatpp_mongo_availableTimeDTOs_hpp
#define example_oatpp_mongo_availableTimeDTOs_hpp

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class AvailableTimeDTO : public oatpp::DTO {
	DTO_INIT(AvailableTimeDTO, DTO);
    
    DTO_FIELD(String, _id);
    DTO_FIELD(String, tutor_id);
    DTO_FIELD(String, date);
    DTO_FIELD(String, from_time);
    DTO_FIELD(String, to_time);
    DTO_FIELD(Boolean, isActive);
};

#include OATPP_CODEGEN_END(DTO)

#endif /* example_oatpp_mongo_DTOs_hpp */
