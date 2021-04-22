#include "controller/UserController.hpp"
#include "controller/CourseController.hpp"
#include "controller/ScheduleController.hpp"
#include "./AppComponent.hpp"
#include "oatpp-swagger/Controller.hpp"
#include "oatpp/network/Server.hpp"
#include <mongocxx/instance.hpp>
#include <iostream>
#include <string>
#include "boost/date_time/posix_time/posix_time.hpp"
#include <iostream>


void run(const oatpp::base::CommandLineArguments& args) {

  mongocxx::instance instance{};

  AppComponent components(args);
  
  /* create ApiControllers and add endpoints to router */
  
  auto router = components.httpRouter.getObject();
  auto docEndpoints = oatpp::swagger::Controller::Endpoints::createShared();
  
  auto userController = UserController::createShared();
  userController->addEndpointsToRouter(router);

  auto courseController = CourseController::createShared();
  courseController->addEndpointsToRouter(router);

  auto scheduleController = ScheduleController::createShared();
  scheduleController->addEndpointsToRouter(router);
  
  docEndpoints->pushBackAll(userController->getEndpoints());
  docEndpoints->pushBackAll(courseController->getEndpoints());
  docEndpoints->pushBackAll(scheduleController->getEndpoints());
  
  auto swaggerController = oatpp::swagger::Controller::createShared(docEndpoints);
  swaggerController->addEndpointsToRouter(router);
  
  /* create server */
  
  oatpp::network::Server server(components.serverConnectionProvider.getObject(),
                                components.serverConnectionHandler.getObject());
  
  OATPP_LOGD("Server", "Running on port %s...", components.serverConnectionProvider.getObject()->getProperty("port").toString()->c_str());
  
  server.run();
  
}

/**
 *  main
 */
int main(int argc, const char * argv[]) {


  // std::string testPassword = "quannguyen";
  // std::string testKey = "thisiskey";

  // std::string encryptedPass = encrypt(testPassword, testKey);

  // std::cout << encryptedPass << std::endl;
  // std::cout << decrypt(encryptedPass, testKey) << std::endl;
  




  oatpp::base::Environment::init();


  // std::string ts1("2002-01-20 20:59:59");
  // boost::posix_time::ptime t1(boost::posix_time::time_from_string(ts1));


  // std::string ts2("2002-01-20 23:59:59");
  // boost::posix_time::ptime t2(boost::posix_time::time_from_string(ts2));

  // if (t1 < t2) {
  //   std::cout << "be hon";
  // } else {
  //   std::cout << "lon hon";
  // }

  // std::cout << std::endl;

  // boost::posix_time::ptime t(boost::posix_time::second_clock::local_time());

  // std::cout << to_simple_string(t);





  run(oatpp::base::CommandLineArguments(argc, argv));
  
  /* Print how much objects were created during app running, and what have left-probably leaked */
  /* Disable object counting for release builds using '-D OATPP_DISABLE_ENV_OBJECT_COUNTERS' flag for better performance */
  std::cout << "\nEnvironment:\n";
  std::cout << "objectsCount = " << oatpp::base::Environment::getObjectsCount() << "\n";
  std::cout << "objectsCreated = " << oatpp::base::Environment::getObjectsCreated() << "\n\n";
  
  oatpp::base::Environment::destroy();

  
  
  return 0;
}
