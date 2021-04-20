cmake_minimum_required(VERSION 3.1)

set(project_name cs455)

project(${project_name})

set(CMAKE_CXX_STANDARD 11)

add_library(${project_name}-lib
        src/dao/userDao/userDao.cpp
        src/dao/userDao/userDao.hpp
        src/dao/courseDao/courseDao.cpp
        src/dao/courseDao/courseDao.hpp
        src/dao/scheduleDao/scheduleDao.hpp
        src/dao/scheduleDao/scheduleDao.cpp
        src/dao/appointmentDao/appointmentDao.hpp
        src/dao/appointmentDao/appointmentDao.cpp

        src/AppComponent.hpp
        src/SwaggerComponent.hpp

        src/controller/UserController.hpp
        src/controller/CourseController.hpp
        src/controller/ScheduleController.hpp
	src/controller/AppointmentController.hpp

        src/dto/userDTO.hpp
        src/dto/responseDTO.hpp
        src/dto/loginRequestDTO.hpp
        src/dto/courseDTO.hpp
        src/dto/scheduleDTO.hpp
        src/dto/appointmentDTO.hpp

        src/dao/userDao/userModel.hpp
        src/dao/courseDao/courseModel.hpp
        src/dao/scheduleDao/scheduleModel.hpp
        src/dao/appointmentDao/appointmentModel.hpp
)

###################################################################
# Find mongocxx

find_package(mongocxx REQUIRED)

message("LIBBSONCXX_INCLUDE_DIRS=${LIBBSONCXX_INCLUDE_DIRS}")
message("LIBBSONCXX_LIBRARIES=${LIBBSONCXX_LIBRARIES}")

message("LIBMONGOCXX_INCLUDE_DIRS=${LIBMONGOCXX_INCLUDE_DIRS}")
message("LIBMONGOCXX_LIBRARIES=${LIBMONGOCXX_LIBRARIES}")

###################################################################

find_package(oatpp          1.2.5 REQUIRED)
find_package(oatpp-swagger  1.2.5 REQUIRED)
find_package(oatpp-mongo    1.2.5 REQUIRED)
find_package(Boost          1.75.0 REQUIRED)

## include directories

target_include_directories(${project_name}-lib
        PUBLIC src
)

## link libs

target_link_libraries(${project_name}-lib
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-swagger
        PUBLIC oatpp::oatpp-mongo
        PUBLIC Boost::boost

)

if (TARGET mongo::mongocxx_shared)
    target_link_libraries(${project_name}-lib
            PUBLIC mongo::mongocxx_shared
    )
    message("mongo::mongocxx_shared is used")
elseif(TARGET mongo::mongocxx_static)
    target_link_libraries(${project_name}-lib
            PUBLIC mongo::mongocxx_static
    )
endif()

## define path to swagger-ui res folder

add_definitions(
        -DOATPP_SWAGGER_RES_PATH="${oatpp-swagger_INCLUDE_DIRS}/../bin/oatpp-swagger/res"
)

#################################################################
## add executables

add_executable(${project_name}
        src/App.cpp
)

target_link_libraries(${project_name} ${project_name}-lib)
add_dependencies(${project_name} ${project_name}-lib)

add_executable(${project_name}-test
        test/tests.cpp
)

target_link_libraries(${project_name}-test ${project_name}-lib)
add_dependencies(${project_name}-test ${project_name}-lib)

enable_testing()
add_test(project-tests ${project_name}-test)
