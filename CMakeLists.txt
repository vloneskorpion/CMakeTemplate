cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 17)

project(greeter)

option(GREETER_EXAMPLE "Build an example" OFF)
option(GREETER_INSTALL "Install greeter" OFF)
option(GREETER_SHARED "Build greeter as shared_library" BUILD_SHARED_LIBS)
if(GREETER_SHARED)
	set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

include(FetchContent)
FetchContent_Declare(fmt
	GIT_REPOSITORY https://github.com/fmtlib/fmt.git
	GIT_TAG 9.1.0
)
FetchContent_MakeAvailable(fmt)

set(GREETER_SOURCES
		include/greeter/config.hpp
		include/greeter/greeter.hpp
		src/greeter.cpp
)

if(GREETER_SHARED)
	add_library(greeter_lib SHARED ${GREETER_SOURCES})
else()
	add_library(greeter_lib STATIC ${GREETER_SOURCES})
endif()
add_library(sample::greeter_lib ALIAS greeter_lib)

target_include_directories(greeter_lib PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
)
target_link_libraries(greeter_lib PRIVATE fmt::fmt)

add_executable(main main.cpp)
target_link_libraries(main greeter_lib)

if(GREETER_SHARED)
	target_compile_definitions(greeter_lib PRIVATE GREETER_EXPORTS)
	target_compile_definitions(greeter_lib INTERFACE GREETER_IMPORTS)
endif()

if(GREETER_INSTALL)
	# INSTALLATION TARGET
	install(TARGETS greeter_lib EXPORT greeter_export
				RUNTIME DESTINATION bin
				ARCHIVE DESTINATION lib
				LIBRARY DESTINATION lib
				INCLUDES DESTINATION include
	)

	# COPY PUBLIC INCLUDE FILES
	install(FILES
				include/greeter/config.hpp
				include/greeter/greeter.hpp
				DESTINATION include/greeter
	)
	
	# create export file which can be imported by other cmake projects
	install(EXPORT greeter_export
			FILE greeter_lib-config.cmake
			NAMESPACE sample::
			DESTINATION lib/cmake/greeter_lib
	)

endif()

target_compile_options(main PRIVATE -Wall -Wextra -pedantic)