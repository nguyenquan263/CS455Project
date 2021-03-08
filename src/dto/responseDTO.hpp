
#ifndef example_oatpp_mongo_responseDTOs_hpp
#define example_oatpp_mongo_responseDTOs_hpp

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class ResponseDTO : public oatpp::DTO {
	DTO_INIT(ResponseDTO, DTO);

	DTO_FIELD(Int16, errorCode);
	DTO_FIELD(String, message);
	DTO_FIELD(Any, data);
};

#include OATPP_CODEGEN_END(DTO)

#endif /* example_oatpp_mongo_DTOs_hpp */
