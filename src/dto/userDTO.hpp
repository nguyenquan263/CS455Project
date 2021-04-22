
#ifndef example_oatpp_mongo_DTOs_hpp
#define example_oatpp_mongo_DTOs_hpp

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class UserDto : public oatpp::DTO {

  DTO_INIT(UserDto, DTO);

  DTO_FIELD(String, _id);
  DTO_FIELD(String, username);
  DTO_FIELD(String, email);
  DTO_FIELD(String, password);
  DTO_FIELD(String, phone);
  DTO_FIELD(String, role);
  DTO_FIELD(String, lastActivityDateTime);
  DTO_FIELD(Boolean, isActive);

};

#include OATPP_CODEGEN_END(DTO)

#endif /* example_oatpp_mongo_DTOs_hpp */
