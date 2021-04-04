#ifndef example_oatpp_mongo_CourseDao_hpp
#define example_oatpp_mongo_CourseDao_hpp

#include "dto/courseDTO.hpp"
#include "courseModel.hpp"

#include "oatpp-mongo/bson/mapping/ObjectMapper.hpp"

#include <mongocxx/pool.hpp>
#include <bsoncxx/document/value.hpp>

namespace dao {

class courseDao {
private:
  std::shared_ptr<mongocxx::pool> m_pool;
  std::string m_databaseName;
  std::string m_collectionName;
  oatpp::mongo::bson::mapping::ObjectMapper m_objectMapper;
private:
  oatpp::Object<Course> courseFromDto(const oatpp::Object<CourseDTO>& dto);
  oatpp::Object<CourseDTO> dtoFromCourse(const oatpp::Object<Course>& course);
private:
  bsoncxx::document::value createMongoDocument(const oatpp::Void &polymorph);
public:

  courseDao(const mongocxx::uri &uri, const std::string &dbName, const std::string &collectionName);

  oatpp::Object<CourseDTO> createCourse(const oatpp::Object<CourseDTO> &courseDto);
  oatpp::Object<CourseDTO> updateCourse(const oatpp::Object<CourseDTO> &courseDto);
  oatpp::Object<CourseDTO> getCourseByID(const oatpp::String& courseObjectID);
  oatpp::Object<CourseDTO> getCourseByCode(const oatpp::String& code);
  oatpp::List<oatpp::Object<CourseDTO>> getAllCourses();
  bool deleteCourse(const oatpp::String& courseObjectID);

};

}

#endif //example_oatpp_mongo_Database_hpp
