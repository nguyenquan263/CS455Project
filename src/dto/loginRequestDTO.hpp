
#ifndef example_oatpp_mongo_loginRequestDTOs_hpp
#define example_oatpp_mongo_loginRequestDTOs_hpp

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class loginRequestDTO : public oatpp::DTO {
	DTO_INIT(loginRequestDTO, DTO);

	DTO_FIELD(String, username);
	DTO_FIELD(String, password);
};

#include OATPP_CODEGEN_END(DTO)

#endif /* example_oatpp_mongo_DTOs_hpp */
