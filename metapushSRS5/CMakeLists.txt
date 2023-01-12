cmake_minimum_required(VERSION 2.8)
project(metapushSRS5)
# add_definitions(-D__STDC_FORMAT_MACROS)

set(CMAKE_BUILD_TYPE "RELEASE")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -std=gnu11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=gnu++11")
set(HOME_BASE "../")

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)
# 头文件目录
include_directories(${HOME_BASE}/include
                    ${HOME_BASE}/libmetartc5/src/
                    )

#添加ffmpeg
find_package(FFMPEG REQUIRED)
if(FFMPEG_FOUND)
    message( STATUS "Found FFmpeg ${FFMPEG_VERSION} installed in the system.")
    message( STATUS " - Includes: ${FFMPEG_INCLUDE_DIRS}")
    message( STATUS " - Libraries: ${FFMPEG_LIBRARIES}")
else()
    message("Cannot Find FFMPEG")
endif(FFMPEG_FOUND)
include_directories(${FFMPEG_INCLUDE_DIR})


set(METARTC_LIB ${CMAKE_CURRENT_SOURCE_DIR}/libmetartc5.a)
set(METARTCCORE_LIB ${CMAKE_CURRENT_SOURCE_DIR}/libmetartccore5.a)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} 
                      ${METARTC_LIB}
                      ${METARTCCORE_LIB}
                      -lpthread
                      -lssl
                      -lcrypto
                      -lopenh264
                      -lyuv
                      -ljpeg
                      -lspeexdsp
                      -lopus
                      -lX11
                      -ldl
                      -lsrtp2
                      -lmbedtls
                      -lmbedx509
                      -lmbedcrypto
                      -lusrsctp
                      -lasound
                      )


add_executable(metartc_zlm metartc_zlm.cpp)
target_link_libraries(metartc_zlm
                      ${METARTC_LIB}
                      ${METARTCCORE_LIB}
                      -lpthread
                      -lssl
                      -lcrypto
                      -lopenh264
                      -lyuv
                      -ljpeg
                      -lspeexdsp
                      -lopus
                      -lX11
                      -ldl
                      -lsrtp2
                      -lmbedtls
                      -lmbedx509
                      -lmbedcrypto
                      -lusrsctp
                      -lasound
                      )


add_executable(peerconnection peerconnection.cpp)
target_link_libraries(peerconnection
                      ${METARTC_LIB}
                      ${METARTCCORE_LIB}
                      -lpthread
                      -lssl
                      -lcrypto
                      -lopenh264
                      -lyuv
                      -ljpeg
                      -lspeexdsp
                      -lopus
                      -lX11
                      -ldl
                      -lsrtp2
                      -lmbedtls
                      -lmbedx509
                      -lmbedcrypto
                      -lusrsctp
                      -lasound
                      ${FFMPEG_LIBRARIES}
                      )
