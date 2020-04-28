cmake_minimum_required (VERSION 2.8.11)

project (write_test)

include (FindPkgConfig)

if (NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE "Release")
endif ()

if (NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
  set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/bin")
  set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/bin")
endif ()

if (NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
  set (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
  set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/bin")
  set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/bin")
endif ()

if (NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
  set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
  set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/bin")
  set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/bin")
endif ()

#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")

find_package(Qt5 COMPONENTS Network Core Test REQUIRED) #SerialBus

#SET(Qt5SerialBus_DIR QT_PATH/lib/cmake/Qt5SerialBus)
SET(Qt5Core_DIR QT_PATH/lib/cmake/Qt5Core)
SET(Qt5Test_DIR QT_PATH/lib/cmake/Qt5Core)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/../src")
include_directories ("${QT_PATH}/include/QtCore/5.12.6/QtCore/private")
include_directories ("${QT_PATH}/include/QtCore/5.12.6/QtCore")
include_directories (${Qt5Core_PRIVATE_INCLUDE_DIRS})

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable (write_test
                #write_test.cpp
                main.cpp
                ../src/socketcanbackend_v2.cpp
                ../src/qcanbusdevice_v2.cpp
                ../src/qcanbusframe_v2.cpp
                ../src/qcanbus_v2.cpp
                ../src/qcanbusdeviceinfo_v2.cpp
                ../src/qcanbusfactory_v2.cpp
                ${MOCSrcs})

target_link_libraries(write_test Qt5::Core Qt5::Network Qt5::Test) #Qt5::SerialBus

