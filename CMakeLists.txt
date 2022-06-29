message(STATUS  "===========nertc-electron-sdk============")
message(STATUS "======================================= ${CMAKE_JS_LIB}")

cmake_minimum_required(VERSION 3.10)
set(TARGET_NAME nertc-electron-sdk)
project(${TARGET_NAME})
set(CMAKE_CXX_STANDARD 11)

# get NODE_ADDON_API_DIR
add_definitions(-DNAPI_VERSION=4)
execute_process(COMMAND node -p "require('node-addon-api').include"
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        OUTPUT_VARIABLE NODE_ADDON_API_DIR)
string(REPLACE "\n" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
string(REPLACE "\"" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
message(STATUS "NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR}")


include_directories(${CMAKE_JS_INC}
${NODE_ADDON_API_DIR}
${CMAKE_CURRENT_LIST_DIR}/nertc_sdk
${CMAKE_CURRENT_LIST_DIR}/nertc_sdk/api
${CMAKE_CURRENT_LIST_DIR}/shared
${CMAKE_CURRENT_LIST_DIR}/nertc_sdk/nertc_sdk_Mac.framework/Headers
${CMAKE_CURRENT_LIST_DIR}/shared/libyuv/include
${CMAKE_CURRENT_LIST_DIR}/shared/libyuv/include/libyuv
)

message(STATUS  "=====================111111=====================")
message(STATUS  ${CMAKE_JS_INC})

file(GLOB_RECURSE NERTC_NODE_SOURCE 
${CMAKE_CURRENT_LIST_DIR}/nertc_sdk_node/*.cc
${CMAKE_CURRENT_LIST_DIR}/nertc_sdk_node/*.h
${CMAKE_CURRENT_LIST_DIR}/nertc_sdk_node/*.cpp
${CMAKE_CURRENT_LIST_DIR}/shared/sdk_helper/*.h
${CMAKE_CURRENT_LIST_DIR}/shared/sdk_helper/*.cpp
${CMAKE_CURRENT_LIST_DIR}/shared/libyuv/source/*.cc
${CMAKE_CURRENT_LIST_DIR}/shared/util/logger.h
${CMAKE_CURRENT_LIST_DIR}/shared/util/logger.cpp
#${CMAKE_CURRENT_LIST_DIR}/shared/util/*.cpp  
#${CMAKE_CURRENT_LIST_DIR}/shared/util/*.c
)


link_directories(${TARGET_NAME}  PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/nertc_sdk/bin/darwin
        ${PROJECT_SOURCE_DIR}/nertc_sdk/)    # 

#生成链接文件(.node)
add_library(${TARGET_NAME} SHARED ${NERTC_NODE_SOURCE} ${CMAKE_JS_SRC})




#目标文件属性设置
set_target_properties(${TARGET_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

#设置链接库目录
# link_directories(${TARGET_NAME}  
# ${CMAKE_CURRENT_LIST_DIR}/nertc_sdk/lib
# )

message("===============  ${CMAKE_JS_LIB}")

if(APPLE)
    set_target_properties( ${TARGET_NAME}
        PROPERTIES
        BUILD_WITH_INSTALL_RPATH 1
        INSTALL_RPATH "@loader_path")
endif ()





if (WIN32)
      target_link_libraries(${TARGET_NAME} ${CMAKE_JS_LIB}  ${CMAKE_CURRENT_LIST_DIR}/nertc_sdk/lib/nertc_sdk.lib Ws2_32)
elseif(APPLE)
    target_link_libraries(${TARGET_NAME}   
                      ${CMAKE_JS_LIB}
                      )
else()
target_link_libraries(
        ${TARGET_NAME}
        ${CMAKE_JS_LIB}
        nertc_sdk
        protoopp)
       
endif()

if (APPLE)
      find_library(NERTC_FRAMEWORK nertc_sdk_Mac ${CMAKE_CURRENT_LIST_DIR}/nertc_sdk)
      if (NOT ${NERTC_FRAMEWORK} STREQUAL "NERTC_FRAMEWORK-NOTFOUND")
              target_link_libraries(${PROJECT_NAME} ${NERTC_FRAMEWORK})
      endif ()
            find_library(NEFUNDATION_FRAMEWORK NEFundation_Mac ${CMAKE_CURRENT_LIST_DIR}/nertc_sdk)
            message(STATUS "============================= nefundation: ${NEFUNDATION_FRAMEWORK}")
        if (NOT ${NEFUNDATION_FRAMEWORK} STREQUAL "NEFUNDATION_FRAMEWORK-NOTFOUND")
             target_link_libraries(${PROJECT_NAME} ${NEFUNDATION_FRAMEWORK})
         endif ()
endif()


