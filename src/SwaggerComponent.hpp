
#ifndef example_oatpp_mongo_SwaggerComponent_hpp
#define example_oatpp_mongo_SwaggerComponent_hpp

#include "oatpp-swagger/Model.hpp"
#include "oatpp-swagger/Resources.hpp"
#include "oatpp/core/macro/component.hpp"

/**
 *  Swagger ui is served at
 *  http://host:port/swagger/ui
 */
class SwaggerComponent {
public:
  
  /**
   *  General API docs info
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>, swaggerDocumentInfo)([] {
    
    oatpp::swagger::DocumentInfo::Builder builder;
    
    builder
    .setTitle("CS 455 Capstone Project")
    .setDescription("")
    .setVersion("1.0")
    .setContactName("CS 455 Server Gods")
    .setContactUrl("https://oatpp.io/")
    
    .addServer("http://10.205.233.170:8000", "server on remote machine")
    .addServer("http://localhost:8000", "server on localhost")
    .addServer("http://quanthu.life:8000", "server on the cloud");
    
    return builder.build();
    
  }());
  
  
  /**
   *  Swagger-Ui Resources (<oatpp-examples>/lib/oatpp-swagger/res)
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>, swaggerResources)([] {
    // Make sure to specify correct full path to oatpp-swagger/res folder !!!
    return oatpp::swagger::Resources::loadResources(OATPP_SWAGGER_RES_PATH);
  }());
  
};

#endif /* example_oatpp_mongo_SwaggerComponent_hpp */
