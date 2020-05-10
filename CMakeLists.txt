cmake_minimum_required(VERSION 3.8.2)
project(json VERSION 0.1.4 LANGUAGES C)

set(C_STANDARD 11)
set(C_STANDARD_REQUIRED YES)

set(JSON_BUILD)
option(JSON_SHARED "Shared build" ON)
option(JSON_STATIC "Static build" OFF)
option(JSON_USE_C99 "" OFF)

if(NOT JSON_STATIC AND JSON_SHARED)
  set(JSON_BUILD SHARED)
else(JSON_STATIC)
  set(JSON_BUILD STATIC)
endif()

if(JSON_USE_C99)
  set(C_STANDARD 99)
endif()

if(MSVC)
	add_definitions(-D_WINDOWS -D_USRDLL -DJSON_EXPORTS -DJSON_DLL)
	add_compile_options("/W3" "/Ox" "/Gy" "/Oi" "/TC" "/analyze")
else()
  add_compile_options("-Wall" "-Werror" "-std=gnu11" "-O3")
endif()

add_library(json
  ${JSON_BUILD}
  src/json.c
)

set_target_properties(json PROPERTIES 
                              VERSION ${PROJECT_VERSION}
                            SOVERSION ${PROJECT_VERSION_MAJOR})

target_include_directories(json PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

install(
	TARGETS	json
	EXPORT json
	ARCHIVE DESTINATION lib/ COMPONENT development
	LIBRARY DESTINATION lib/ COMPONENT runtime
	RUNTIME DESTINATION ${CMAKE_INSTALL_NAME_DIR} COMPONENT runtime
)

if(JSON_SHARED)
	install(
		TARGETS	json
		EXPORT json
		LIBRARY DESTINATION include/ COMPONENT development 
	)
endif()

INSTALL(DIRECTORY include/ DESTINATION include)

# Test Configuration
if(JSON_USE_TEST)
  enable_testing()
  add_subdirectory(test)
endif()
