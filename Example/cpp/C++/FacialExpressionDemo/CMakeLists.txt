cmake_minimum_required(VERSION 2.8.0)

project (faceexp)

set ( EXAMPLES_DIR ../../../ )

if (WIN32)
        set ( PROJECT_LINK_LIBS edk.lib)
        if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "(amd64)|(AMD64)")
           link_directories( ${EXAMPLES_DIR}/../bin/win64 )
        elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "(x86)|(X86)")
           link_directories( ${EXAMPLES_DIR}/../bin/win32 )
        else()
           link_directories( ${EXAMPLES_DIR}/../bin/winarm )
        endif()
elseif(APPLE)

elseif(UNIX)
        set ( PROJECT_LINK_LIBS libedk.so )
        if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
           link_directories( ${EXAMPLES_DIR}/../bin/linux64 )
        elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86")
           link_directories( ${EXAMPLES_DIR}/../bin/linux32 )
        else()
           link_directories( ${EXAMPLES_DIR}/../bin/armhf )
        endif()
endif()

include_directories(${EXAMPLES_DIR}/include)
 
add_executable(faceexp Socket.cpp FacialExpressionControl.cpp FacialExpressionDemo.cpp )
target_link_libraries(faceexp ${PROJECT_LINK_LIBS} )



