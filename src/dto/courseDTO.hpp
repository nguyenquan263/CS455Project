
#ifndef example_oatpp_mongo_courseDTOs_hpp
#define example_oatpp_mongo_courseDTOs_hpp

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class CourseDTO : public oatpp::DTO {
	DTO_INIT(CourseDTO, DTO);
    
    DTO_FIELD(String, _id);
    DTO_FIELD(String, code);
    DTO_FIELD(String, name);
    DTO_FIELD(Boolean, isActive);
};

#include OATPP_CODEGEN_END(DTO)

#endif /* example_oatpp_mongo_DTOs_hpp */
