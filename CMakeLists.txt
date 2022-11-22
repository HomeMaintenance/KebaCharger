cmake_minimum_required(VERSION 3.20)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)

project(KebaCharger)

add_subdirectory(test)

include(FetchContent)
FetchContent_Declare(
    PowerManagement
    GIT_REPOSITORY https://github.com/HomeMaintenance/PowerManagement.git
    GIT_TAG main
)
FetchContent_Declare(
    ModbusDevice_Interface
    GIT_REPOSITORY https://github.com/HomeMaintenance/Modbus.git
    GIT_TAG main
)
FetchContent_MakeAvailable(PowerManagement ModbusDevice_Interface)

find_package(jsoncpp CONFIG REQUIRED)

add_library(
    KebaCharger
    KebaCharger.h
    KebaCharger.cpp
)
target_include_directories(KebaCharger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(DEFINED CROSS_COMPILE)
target_link_libraries(KebaCharger PUBLIC
    jsoncpp_object
    jsoncpp_static
    JsonCpp::JsonCpp
)
else()
target_link_libraries(KebaCharger PUBLIC
    jsoncpp_lib
    jsoncpp_object
    JsonCpp::JsonCpp
)
endif()
target_link_libraries(KebaCharger PUBLIC
    PowerManagement
    ModbusDevice)
# target_compile_definitions(KebaCharger PRIVATE KebaCharger_DEBUG=1)

if(PROJECT_IS_TOP_LEVEL)
    find_package(Doxygen
        REQUIRED dot
        OPTIONAL_COMPONENTS mscgen dia
    )
    set(DOXYGEN_EXTRACT_PRIVATE TRUE)
    set(DOXYGEN_WARN_IF_UNDOCUMENTED FALSE)
    set(DOXYGEN_OPTIMIZE_OUTPUT_FOR_C TRUE)
    set(DOXYGEN_EXCLUDE_PATTERNS */build/*)

    doxygen_add_docs(doxygen_target
        ${CMAKE_SOURCE_DIR}
        COMMENT "Run doxygen_target"
    )
endif()
