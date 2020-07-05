cmake_minimum_required(VERSION 3.0.0)
project(boost-tutorial-3 VERSION 0.1.0)


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)
enable_testing()

# find_package(Boost 1.73 REQUIRED COMPONENTS coroutine program_options)
# include_directories(/usr/local/include)
# link_directories(/usr/local/lib)
# include_directories(/usr/local/gcc/include)
# link_directories(/usr/local/gcc/lib64)

list(APPEND CMAKE_MODULE_PATH "/usr/local/lib/cmake")


message(${CMAKE_MODULE_PATH})
find_package(Boost)
find_package(Boost 1.62 REQUIRED COMPONENTS program_options system thread )
if (Boost_FOUND)

    # find_package( Boost REQUIRED COMPONENTS system date_time thread )
    include_directories(${Boost_INCLUDE_DIR})   

    include_directories(/usr/local/include)
    link_directories(/usr/local/lib)
    # include_directories(/usr/local/gcc/include)
    # link_directories(/usr/local/gcc/lib64)

    message(Boost_FOUND: ${Boost_FOUND} )
    message(Boost_INCLUDE_DIR: ${Boost_INCLUDE_DIR} )
    message(Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS} )
    message(Boost_LIBRARIES: ${Boost_LIBRARIES} )
    message(Boost_SYSTEM_FOUND: ${Boost_SYSTEM_FOUND} )
    message(Boost_SYSTEM_LIBRARY: ${Boost_SYSTEM_LIBRARY} )
    message(Boost_DATE_TIME_FOUND: ${Boost_DATE_TIME_FOUND} )
    message(Boost_DATE_TIME_LIBRARY: ${Boost_DATE_TIME_LIBRARY} )
    add_definitions( "-DHAS_BOOST" )
else()
    message(boost no found..)
endif()

add_executable(boost-option-1 boost-option-1.cpp)
target_link_libraries(boost-option-1 ${Boost_LIBRARIES})

add_executable(boost-option-2 boost-option-2.cpp)
target_link_libraries(boost-option-2 ${Boost_LIBRARIES})

add_executable(boost-any-1 boost-any-1.cpp)

add_executable(boost-any-2 boost-any-2.cpp)

add_executable(boost-tuple-1 boost-tuple-1.cpp)

add_executable(boost-bind-1 boost-bind-1.cpp)

add_executable(boost-thread-1 boost-thread-1.cpp)
target_link_libraries(boost-thread-1 ${Boost_LIBRARIES})

add_executable(boost-thread-2 boost-thread-2.cpp)
target_link_libraries(boost-thread-2 ${Boost_LIBRARIES})

add_executable(boost-thread-3 boost-thread-3.cpp)
target_link_libraries(boost-thread-3 ${Boost_LIBRARIES})

add_executable(boost-thread-4 boost-thread-4.cpp)
target_link_libraries(boost-thread-4 ${Boost_LIBRARIES})

# add_executable(swap-1 swap-1.cpp)
# target_link_libraries(swap-1 ${Boost_LIBRARIES})

add_executable(condition-variable-1 condition-variable-1.cpp)
target_link_libraries(condition-variable-1 ${Boost_LIBRARIES})

add_executable(http-client-async-1 http-client-async-1.cpp)
target_link_libraries(http-client-async-1 ${Boost_LIBRARIES})

add_executable(client-async-2 client-async-2.cpp)
target_link_libraries(client-async-2 ${Boost_LIBRARIES})

add_executable(client-async-3 client-async-3.cpp)
target_link_libraries(client-async-3 ${Boost_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

