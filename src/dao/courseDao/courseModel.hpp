#ifndef example_oatpp_mongo_CourseModel_hpp
#define example_oatpp_mongo_CourseModel_hpp

#include "dto/courseDTO.hpp"

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace dao {

class Course : public oatpp::DTO {

  DTO_INIT(Course, DTO)

  DTO_FIELD(String, _id);
  DTO_FIELD(String, code);
  DTO_FIELD(String, name);
  DTO_FIELD(Boolean, isActive);

};

}

#include OATPP_CODEGEN_END(DTO)

#endif // example_oatpp_mongo_Model_hpp
