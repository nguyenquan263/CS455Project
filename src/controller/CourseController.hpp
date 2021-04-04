
#ifndef example_oatpp_mongo_CourseController_hpp
#define example_oatpp_mongo_CourseController_hpp

#include "dao/courseDao/courseDao.hpp"
#include "dto/courseDTO.hpp"
#include "dto/responseDTO.hpp"

#include "oatpp-swagger/Types.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

#include "oatpp/core/data/stream/BufferStream.hpp"

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"

#include <iostream>


#include OATPP_CODEGEN_BEGIN(ApiController)

class CourseController : public oatpp::web::server::api::ApiController {
private:
  OATPP_COMPONENT(std::shared_ptr<dao::courseDao>, m_database);
public:
  CourseController(const std::shared_ptr<ObjectMapper>& objectMapper)
    : oatpp::web::server::api::ApiController(objectMapper)
  {}
public:

  static std::shared_ptr<CourseController> createShared(
    OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
  {
    return std::make_shared<CourseController>(objectMapper);
  }



  ENDPOINT_INFO(createCourse) {
    info->summary = "Create a new Course";
    info->addConsumes<Object<CourseDTO>>("application/json");
    info->addResponse<Object<ResponseDTO>>(Status::CODE_200, "application/json");
  }
  ENDPOINT("POST", "/courses", createCourse,
           BODY_DTO(Object<CourseDTO>, courseDTO)) {
    auto responseDTO = ResponseDTO::createShared();
    try {
      //check the user is existed or not
      auto course = m_database->getCourseByCode(courseDTO->code);

      if (course) {
        // OATPP_LOGD("Server", "found!");
        responseDTO->errorCode = 404;
        responseDTO->message = "Your course's code is duplicated.";
        responseDTO->data = NULL;
      } else {
        // OATPP_LOGD("Server", "not found!");
        responseDTO->errorCode = 200;
        responseDTO->message = "The course is created successfully.";
        responseDTO->data = m_database->createCourse(courseDTO);
      }
    } catch(std::runtime_error &rte) {
      responseDTO->errorCode = 500;
      responseDTO->message = rte.what();
      responseDTO->data = NULL;
    }
    return createDtoResponse(Status::CODE_200, responseDTO);
  }


  ENDPOINT_INFO(putCourse) {
    // general
    info->summary = "Update Course by courseObjectID";
    info->addConsumes<Object<CourseDTO>>("application/json");
    info->addResponse<Object<CourseDTO>>(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_404, "text/plain");
    // params specific
    info->pathParams["courseObjectID"].description = "target courseObjectID";
  }
  ENDPOINT("PUT", "courses/{courseObjectID}", putCourse,
           PATH(String, courseObjectID),
           BODY_DTO(Object<CourseDTO>, courseDTO)) {
    auto responseDTO = ResponseDTO::createShared();

    try {
      //check the user is existed or not
      auto course = m_database->getCourseByID(courseObjectID);

      if (course) {
        courseDTO->_id = courseObjectID;
        responseDTO->errorCode = 200;
        responseDTO->message = "The course is updated successfully.";
        responseDTO->data = m_database->updateCourse(courseDTO);
      } else {
        responseDTO->errorCode = 404;
        responseDTO->message = "Cannot found the target course.";
        responseDTO->data = NULL;  
      }
    } catch(std::runtime_error &rte) {
      responseDTO->errorCode = 500;
      responseDTO->message = rte.what();
      responseDTO->data = NULL;
    }  
    
    return createDtoResponse(Status::CODE_200, responseDTO);
  }


  ENDPOINT_INFO(getCourse) {
    // general
    info->summary = "Get the course by courseObjectID";
    info->addResponse<Object<CourseDTO>>(Status::CODE_200, "application/json");
    info->addResponse<Object<ResponseDTO>>(Status::CODE_404, "application/json");
    // params specific
    info->pathParams["courseObjectID"].description = "target courseObjectID";
  }
  ENDPOINT("GET", "courses/{courseObjectID}", getCourse,
           PATH(String, courseObjectID)) {

    auto responseDTO = ResponseDTO::createShared();

    try {
      auto course = m_database->getCourseByID(courseObjectID);
      if (course) {
        responseDTO->errorCode = 200;
        responseDTO->message = "The target course is found.";
        responseDTO->data = course;
      } else {
        responseDTO->errorCode = 404;
        responseDTO->message = "Cannot found the target course.";
        responseDTO->data = NULL;
      }
    } catch(std::runtime_error &rte) {
      responseDTO->errorCode = 500;
      responseDTO->message = rte.what();
      responseDTO->data = NULL;
    }

    return createDtoResponse(Status::CODE_200, responseDTO);
  }

  


  ENDPOINT_INFO(getAllCourses) {
    info->summary = "Get all stored courses";
    info->addResponse<List<Object<ResponseDTO>>>(Status::CODE_200, "application/json");
  }
  ENDPOINT("GET", "courses", getAllCourses) {
    

    auto responseDTO = ResponseDTO::createShared();
    
    try {
      auto courseList = m_database->getAllCourses();
      responseDTO->errorCode = 200;
      responseDTO->message = "This is the course list.";
      responseDTO->data = courseList;
    } catch(std::runtime_error &rte) {
      responseDTO->errorCode = 500;
      responseDTO->message = rte.what();
      responseDTO->data = NULL;
    }    

    return createDtoResponse(Status::CODE_200, responseDTO);
  }


  ENDPOINT_INFO(deleteCourse) {
    // general
    info->summary = "Delete Course by courseObjectID.";
    info->addResponse<String>(Status::CODE_200, "application/json");
    // params specific
    info->pathParams["codeObjectID"].description = "target courseObjectID";
  }
  ENDPOINT("DELETE", "courses/{courseObjectID}", deleteCourse,
           PATH(String, courseObjectID)) {
    auto responseDTO = ResponseDTO::createShared();

    try {
      bool success = m_database->deleteCourse(courseObjectID);
      if (success) {
        responseDTO->errorCode = 200;
        responseDTO->message = "The course with code equal '" + courseObjectID + "' is deleted successfully.";
        responseDTO->data = NULL;
      } else {
        responseDTO->errorCode = 404;
        responseDTO->message = "Cannot found the target course.";
        responseDTO->data = NULL;
      }

    } catch(std::runtime_error &rte) {
      responseDTO->errorCode = 500;
      responseDTO->message = rte.what();
      responseDTO->data = NULL;
    }
    return createDtoResponse(Status::CODE_200, responseDTO);
  } 
};

#include OATPP_CODEGEN_END(ApiController)

#endif /* example_oatpp_mongo_UserController_hpp */
