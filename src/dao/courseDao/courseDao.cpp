#include "courseDao.hpp"

#include "dao/courseDao/courseModel.hpp"
#include "oatpp/core/data/stream/BufferStream.hpp"

#include <mongocxx/client.hpp>
#include <mongocxx/options/insert.hpp>
#include <mongocxx/exception/operation_exception.hpp>
#include <iostream>

namespace dao {

courseDao::courseDao(const 
    mongocxx::uri &uri, 
    const std::string &dbName, 
    const std::string &collectionName) 
    : m_pool(std::make_shared<mongocxx::pool>(uri)), 
      m_databaseName(dbName), 
      m_collectionName(collectionName)
{}

oatpp::Object<Course> courseDao::courseFromDto(const oatpp::Object<CourseDTO>& dto) {
  auto course = Course::createShared();
  oatpp::mongo::bson::ObjectId objId = oatpp::mongo::bson::type::ObjectId();
  course->_id = objId->toString();
  course->code = dto->code;
  course->name = dto->name;
  course->isActive = dto->isActive;
  return course;
}

oatpp::Object<CourseDTO> courseDao::dtoFromCourse(const oatpp::Object<Course>& course) {
  auto dto = CourseDTO::createShared();
  dto->_id = course->_id;
  dto->code = course->code;
  dto->name = course->name;
  dto->isActive = course->isActive;
  return dto;
}

bsoncxx::document::value courseDao::createMongoDocument(const oatpp::Void &polymorph) {
  // if you have huge docs, you may want to increase starting BufferOutputStream size.
  // Or you may want to use oatpp::data::stream::ChunkedBuffer instead - for no-copy growth.
  oatpp::data::stream::BufferOutputStream stream;
  m_objectMapper.write(&stream, polymorph);
  bsoncxx::document::view view(stream.getData(), stream.getCurrentPosition());
  return bsoncxx::document::value(view);
}

oatpp::Object<CourseDTO> courseDao::createCourse(const oatpp::Object<CourseDTO> &courseDTO) {
  auto conn = m_pool->acquire();
  auto collection = (*conn)[m_databaseName][m_collectionName];

  auto insertedCourse = courseFromDto(courseDTO);

  collection.insert_one(createMongoDocument(insertedCourse));
  return dtoFromCourse(insertedCourse);
}

oatpp::Object<CourseDTO> courseDao::updateCourse(const oatpp::Object<CourseDTO> &courseDTO) {
  auto conn = m_pool->acquire();
  auto collection = (*conn)[m_databaseName][m_collectionName];

  collection.update_one(
    createMongoDocument( // <-- Filter
      oatpp::Fields<oatpp::String>({
        {"_id", courseDTO->_id}
      })
    ),
    createMongoDocument( // <-- Set
      oatpp::Fields<oatpp::Any>({ // map
        { // pair
          "$set", oatpp::Fields<oatpp::Any>({ // you can also define a "strict" DTO for $set operation.
            {"code", courseDTO->code},
            {"name", courseDTO->name},
            {"isActive", courseDTO->isActive}
          })
        } // pair
      }) // map
    )
  );

  auto result =
    collection.find_one(createMongoDocument( // <-- Filter
      oatpp::Fields<oatpp::String>({
        {"_id", courseDTO->_id}
      })
    ));

  if(result) {
    auto view = result->view();
    auto bson = oatpp::String((const char*)view.data(), view.length(), false /* to not copy view data */);
    auto course = m_objectMapper.readFromString<oatpp::Object<Course>>(bson);
    return dtoFromCourse(course);
  }

  return nullptr;

}

oatpp::Object<CourseDTO> courseDao::getCourseByID(const oatpp::String& courseObjectID) {
  auto conn = m_pool->acquire();
  auto collection = (*conn)[m_databaseName][m_collectionName];

  auto result =
    collection.find_one(createMongoDocument( // <-- Filter
      oatpp::Fields<oatpp::String>({
        {"_id", courseObjectID}
      })
    ));

  if(result) {
    auto view = result->view();
    auto bson = oatpp::String((const char*)view.data(), view.length(), false /* to not copy view data */);
    auto course = m_objectMapper.readFromString<oatpp::Object<Course>>(bson);
    return dtoFromCourse(course);
  }

  return nullptr;
}

oatpp::Object<CourseDTO> courseDao::getCourseByCode(const oatpp::String& code) {
  auto conn = m_pool->acquire();
  auto collection = (*conn)[m_databaseName][m_collectionName];

  auto result =
    collection.find_one(createMongoDocument( // <-- Filter
      oatpp::Fields<oatpp::String>({
        {"code", code}
      })
    ));

  if(result) {
    auto view = result->view();
    auto bson = oatpp::String((const char*)view.data(), view.length(), false /* to not copy view data */);
    auto course = m_objectMapper.readFromString<oatpp::Object<Course>>(bson);
    return dtoFromCourse(course);
  }

  return nullptr;
}


oatpp::List<oatpp::Object<CourseDTO>> courseDao::getAllCourses() {
  auto conn = m_pool->acquire();
  auto collection = (*conn)[m_databaseName][m_collectionName];

  auto cursor = collection.find(
    createMongoDocument(oatpp::Fields<oatpp::String>({})
  ));

  oatpp::List<oatpp::Object<CourseDTO>> list({});

  for(auto view : cursor) {
    auto bson = oatpp::String((const char*)view.data(), view.length(), false /* to not copy view data */);
    auto course = m_objectMapper.readFromString<oatpp::Object<Course>>(bson);
    list->push_back(dtoFromCourse(course));
  }

  return list;

}

bool courseDao::deleteCourse(const oatpp::String& courseObjectID) {
  auto conn = m_pool->acquire();
  auto collection = (*conn)[m_databaseName][m_collectionName];

  auto result =
    collection.delete_one(createMongoDocument( // <-- Filter
      oatpp::Fields<oatpp::String>({
        {"_id", courseObjectID}
      })
    ));

  if(result) {
    return result->deleted_count() == 1;
  }
  return false;
}

}