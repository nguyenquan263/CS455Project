
#ifndef example_oatpp_mongo_AppComponent_hpp
#define example_oatpp_mongo_AppComponent_hpp

#include "dao/userDao/userDao.hpp"
#include "dao/courseDao/courseDao.hpp"
#include "dao/scheduleDao/scheduleDao.hpp"
#include "SwaggerComponent.hpp"

#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/web/server/HttpRouter.hpp"

#include "oatpp/network/tcp/server/ConnectionProvider.hpp"

#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

#include "oatpp/core/base/CommandLineArguments.hpp"
#include "oatpp/core/macro/component.hpp"

#include <mongocxx/client.hpp>

#include <iostream>

/**
 *  Class which creates and holds Application components and registers components in oatpp::base::Environment
 *  Order of components initialization is from top to bottom
 */
class AppComponent {
private:
  oatpp::base::CommandLineArguments m_cmdArgs;
public:
  AppComponent(const oatpp::base::CommandLineArguments& cmdArgs)
    : m_cmdArgs(cmdArgs)
  {}
public:
  
  /**
   *  Swagger component
   */
  SwaggerComponent swaggerComponent;
  
  /**
   *  Create ConnectionProvider component which listens on the port
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)([] {
    return oatpp::network::tcp::server::ConnectionProvider::createShared({"0.0.0.0", 8000, oatpp::network::Address::IP_4});
  }());
  
  /**
   *  Create Router component
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)([] {
    return oatpp::web::server::HttpRouter::createShared();
  }());
  
  /**
   *  Create ConnectionHandler component which uses Router component to route requests
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler)([] {
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router); // get Router component
    return oatpp::web::server::HttpConnectionHandler::createShared(router);
  }());
  
  /**
   *  Create ObjectMapper component to serialize/deserialize DTOs in Contoller's API
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper)([] {
    auto objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
    objectMapper->getDeserializer()->getConfig()->allowUnknownFields = false;
    objectMapper->getSerializer()->getConfig()->useBeautifier = true;
    return objectMapper;
  }());


  // OATPP Component for managing User Collection.
  OATPP_CREATE_COMPONENT(std::shared_ptr<dao::userDao>, userDatabase)([this] {

    oatpp::String connectionString = std::getenv("DEMO_MONGO_CONN_STR");
    if(!connectionString){
      connectionString = m_cmdArgs.getNamedArgumentValue("--conn-str", "mongodb://qnguyen3:260396@quanthu.life");
      std::cout << "Connected to the database!" << std::endl;
    }

    mongocxx::uri uri(connectionString->std_str());
    return std::make_shared<dao::userDao>(uri, "cs455", "user");

  }());

  // OATPP Component for managing Course Collection.
  OATPP_CREATE_COMPONENT(std::shared_ptr<dao::courseDao>, courseDatabase)([this] {

    oatpp::String connectionString = std::getenv("DEMO_MONGO_CONN_STR");
    if(!connectionString){
      connectionString = m_cmdArgs.getNamedArgumentValue("--conn-str", "mongodb://qnguyen3:260396@quanthu.life");
      std::cout << "Connected to the database!" << std::endl;
    }

    mongocxx::uri uri(connectionString->std_str());
    return std::make_shared<dao::courseDao>(uri, "cs455", "course");

  }());

  OATPP_CREATE_COMPONENT(std::shared_ptr<dao::scheduleDao>, scheduleDatabase)([this] {

    oatpp::String connectionString = std::getenv("DEMO_MONGO_CONN_STR");
    if(!connectionString){
      connectionString = m_cmdArgs.getNamedArgumentValue("--conn-str", "mongodb://qnguyen3:260396@quanthu.life");
      std::cout << "Connected to the database!" << std::endl;
    }

    mongocxx::uri uri(connectionString->std_str());
    return std::make_shared<dao::scheduleDao>(uri, "cs455", "schedule");

  }());

};

#endif /* example_oatpp_mongo_AppComponent_hpp */
