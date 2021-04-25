#ifndef example_oatpp_mongo_AvailableTimeModel_hpp
#define example_oatpp_mongo_AvailableTimeModel_hpp

#include "dto/availableTimeDTO.hpp"

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace dao {

class AvailableTime : public oatpp::DTO {

  DTO_INIT(AvailableTime, DTO);

  DTO_FIELD(String, _id);
  DTO_FIELD(String, tutor_id);
  DTO_FIELD(String, date);
  DTO_FIELD(String, from_time);
  DTO_FIELD(String, to_time);
  DTO_FIELD(Boolean, isActive);

};

}

#include OATPP_CODEGEN_END(DTO)

#endif // example_oatpp_mongo_Model_hpp
