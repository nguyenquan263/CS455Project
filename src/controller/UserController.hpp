
#ifndef example_oatpp_mongo_UserController_hpp
#define example_oatpp_mongo_UserController_hpp

#include "dao/userDao/userDao.hpp"
#include "dto/userDTO.hpp"
#include "dto/responseDTO.hpp"
#include "dto/loginRequestDTO.hpp"

#include "oatpp-swagger/Types.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

#include "oatpp/core/data/stream/BufferStream.hpp"

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"

#include <iostream>


#include OATPP_CODEGEN_BEGIN(ApiController)

class UserController : public oatpp::web::server::api::ApiController {
private:
  OATPP_COMPONENT(std::shared_ptr<dao::userDao>, m_database);
public:
  UserController(const std::shared_ptr<ObjectMapper>& objectMapper)
    : oatpp::web::server::api::ApiController(objectMapper)
  {}
public:

  static std::shared_ptr<UserController> createShared(
    OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
  {
    return std::make_shared<UserController>(objectMapper);
  }

  ENDPOINT_INFO(greeting) {
    info->summary = "Greeting from CS455 Server Gods.";
    info->addResponse<Object<ResponseDTO>>(Status::CODE_200, "application/json");
  }
  ENDPOINT("GET", "/", greeting) {
    auto responseDTO = ResponseDTO::createShared();

    try{
      responseDTO->errorCode = 200;
      responseDTO->message = "Hi there, this is CS 455 Server Gods!";
      responseDTO->data = NULL;
    } catch(std::runtime_error &rte) {
      responseDTO->errorCode = 500;
      responseDTO->message = rte.what();
      responseDTO->data = NULL;
    }
    
    boost::gregorian::date d(boost::gregorian::day_clock::local_day());
    std::cout << boost::gregorian::to_simple_string(d) << std::endl;

    return createDtoResponse(Status::CODE_200, responseDTO);
  }


  ENDPOINT_INFO(createUser) {
    info->summary = "Create new User";
    info->addConsumes<Object<UserDto>>("application/json");
    info->addResponse<Object<ResponseDTO>>(Status::CODE_200, "application/json");
  }
  ENDPOINT("POST", "/users", createUser,
           BODY_DTO(Object<UserDto>, userDto)) {
    auto responseDTO = ResponseDTO::createShared();
    try {
      //check the user is existed or not
      auto user = m_database->getUser(userDto->username);

      if (user) {
        // OATPP_LOGD("Server", "found!");
        responseDTO->errorCode = 404;
        responseDTO->message = "Your username is duplicated.";
        responseDTO->data = NULL;
      } else {
        // OATPP_LOGD("Server", "not found!");
        responseDTO->errorCode = 200;
        responseDTO->message = "The user is created successfully.";
        responseDTO->data = m_database->createUser(userDto);
      }
    } catch(std::runtime_error &rte) {
      responseDTO->errorCode = 500;
      responseDTO->message = rte.what();
      responseDTO->data = NULL;
    }
    return createDtoResponse(Status::CODE_200, responseDTO);
  }


  ENDPOINT_INFO(putUser) {
    // general
    info->summary = "Update User by username";
    info->addConsumes<Object<UserDto>>("application/json");
    info->addResponse<Object<UserDto>>(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_404, "text/plain");
    // params specific
    info->pathParams["username"].description = "username/login";
  }
  ENDPOINT("PUT", "users/{username}", putUser,
           PATH(String, username),
           BODY_DTO(Object<UserDto>, userDto)) {
    auto responseDTO = ResponseDTO::createShared();

    try {
      //check the user is existed or not
      auto user = m_database->getUser(username);

      if (user) {
        userDto->username = username;
        responseDTO->errorCode = 200;
        responseDTO->message = "The user is updated successfully.";
        responseDTO->data = m_database->updateUser(userDto);
      } else {
        responseDTO->errorCode = 404;
        responseDTO->message = "Cannot found the target user.";
        responseDTO->data = NULL;  
      }
    } catch(std::runtime_error &rte) {
      responseDTO->errorCode = 500;
      responseDTO->message = rte.what();
      responseDTO->data = NULL;
    }  
    
    return createDtoResponse(Status::CODE_200, responseDTO);
  }


  ENDPOINT_INFO(getUser) {
    // general
    info->summary = "Get one User by username";
    info->addResponse<Object<UserDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<ResponseDTO>>(Status::CODE_404, "application/json");
    // params specific
    info->pathParams["username"].description = "username/login";
  }
  ENDPOINT("GET", "users/{username}", getUser,
           PATH(String, username)) {

    auto responseDTO = ResponseDTO::createShared();

    try {
      auto user = m_database->getUser(username);
      if (user) {
        responseDTO->errorCode = 200;
        responseDTO->message = "The target user is found.";
        responseDTO->data = user;
      } else {
        responseDTO->errorCode = 404;
        responseDTO->message = "Cannot found the target user.";
        responseDTO->data = NULL;
      }
    } catch(std::runtime_error &rte) {
      responseDTO->errorCode = 500;
      responseDTO->message = rte.what();
      responseDTO->data = NULL;
    }

    return createDtoResponse(Status::CODE_200, responseDTO);
  }

  //LOGIN----------------------------
  ENDPOINT_INFO(login) {
    info->summary = "Login to the application";
    info->addConsumes<Object<loginRequestDTO>>("application/json");
    info->addResponse<Object<ResponseDTO>>(Status::CODE_200, "application/json");
  }
  ENDPOINT("POST", "users/login", login,
           BODY_DTO(Object<loginRequestDTO>, logDTO)) {

    auto responseDTO = ResponseDTO::createShared();

    try {
      auto user = m_database->login(logDTO->username, logDTO->password);
      if (user) {
        responseDTO->errorCode = 200;
        responseDTO->message = "Login successfully.";
        responseDTO->data = user;

        //update the last activity period
      } else {
        responseDTO->errorCode = 404;
        responseDTO->message = "Your username or password is incorrect.";
        responseDTO->data = NULL;
      }
    } catch(std::runtime_error &rte) {
      responseDTO->errorCode = 500;
      responseDTO->message = rte.what();
      responseDTO->data = NULL;
    }

    return createDtoResponse(Status::CODE_200, responseDTO);
  }
  //---------------------------------


  ENDPOINT_INFO(getAllUsers) {
    info->summary = "Get all stored users";
    info->addResponse<List<Object<ResponseDTO>>>(Status::CODE_200, "application/json");
  }
  ENDPOINT("GET", "users", getAllUsers) {
    

    auto responseDTO = ResponseDTO::createShared();
    
    try {
      auto userList = m_database->getAllUsers();
      responseDTO->errorCode = 200;
      responseDTO->message = "This is the user list.";
      responseDTO->data = userList;
    } catch(std::runtime_error &rte) {
      responseDTO->errorCode = 500;
      responseDTO->message = rte.what();
      responseDTO->data = NULL;
    }    

    return createDtoResponse(Status::CODE_200, responseDTO);
  }


  ENDPOINT_INFO(deleteUser) {
    // general
    info->summary = "Delete User by username";
    info->addResponse<String>(Status::CODE_200, "application/json");
    // params specific
    info->pathParams["username"].description = "username/login";
  }
  ENDPOINT("DELETE", "users/{username}", deleteUser,
           PATH(String, username)) {
    auto responseDTO = ResponseDTO::createShared();

    try {
      bool success = m_database->deleteUser(username);
      if (success) {
        responseDTO->errorCode = 200;
        responseDTO->message = "The user with username equal '" + username + "' is deleted successfully.";
        responseDTO->data = NULL;
      } else {
        responseDTO->errorCode = 404;
        responseDTO->message = "Cannot found the target user.";
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
