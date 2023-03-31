cmake_minimum_required(VERSION 2.8)
project(metapushSRS5)
# add_definitions(-D__STDC_FORMAT_MACROS)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

set(CMAKE_BUILD_TYPE "RELEASE")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -std=gnu11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=gnu++11")
set(HOME_BASE "../../")

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)
# 头文件目录
include_directories(${HOME_BASE}/include/
                    ${HOME_BASE}/libmetartc6/src/
                    ${HOME_BASE}/demo/libmetaApp/src/
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


set(METARTC_LIB ${CMAKE_CURRENT_SOURCE_DIR}/libmetartc6.a)
set(METARTCCORE_LIB ${CMAKE_CURRENT_SOURCE_DIR}/libmetartccore6.a)
set(METAAPP_LIB ${CMAKE_CURRENT_SOURCE_DIR}/libmetaApp.a)

add_executable(metartc_srs metartc_srs.cpp)
target_link_libraries(metartc_srs 
                      ${METAAPP_LIB}
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
                      ${METAAPP_LIB}
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


add_executable(peerconnection_zlm peerconnection_zlm.cpp)
target_link_libraries(peerconnection_zlm
                      ${METAAPP_LIB}
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

add_executable(peerconnection_srs peerconnection_srs.cpp)
target_link_libraries(peerconnection_srs
                    ${METAAPP_LIB}
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

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../../yang_config.ini DESTINATION ${EXECUTABLE_OUTPUT_PATH})                
                    