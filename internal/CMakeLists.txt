CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(context-internal)

# Targets
SET(target_internal "context-internal")

# Source Lists
FILE(GLOB_RECURSE SRCS_INTERNAL *.cpp)

# Dependencies
SET(DEPS_INTERNAL "context-common")

# Building Library
pkg_check_modules(api_internal_pkg REQUIRED ${DEPS_INTERNAL})

FOREACH(flag ${api_internal_pkg_CFLAGS})
   SET(API_INTERNAL_EXTRA_CFLAGS "${API_INTERNAL_EXTRA_CFLAGS} ${flag}")
ENDFOREACH(flag)

ADD_LIBRARY(${target_internal} SHARED ${SRCS_INTERNAL})
TARGET_LINK_LIBRARIES(${target_internal} ${api_internal_pkg_LDFLAGS})
SET_TARGET_PROPERTIES(${target_internal} PROPERTIES COMPILE_FLAGS ${API_INTERNAL_EXTRA_CFLAGS})
SET_TARGET_PROPERTIES(${target_internal} PROPERTIES COMPILE_DEFINITIONS "LOG_TAG=\"CONTEXT-API\"")
SET_TARGET_PROPERTIES(${target_internal} PROPERTIES SOVERSION ${MAJORVER})
SET_TARGET_PROPERTIES(${target_internal} PROPERTIES VERSION ${FULLVER})

# Installing
INSTALL(TARGETS ${target_internal} DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT RuntimeLibraries)
INSTALL(
	DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/context-service/internal
	FILES_MATCHING PATTERN "*_internal.h"
)

SET(VERSION ${FULLVER})
SET(PREFIX ${CMAKE_INSTALL_PREFIX})
SET(PC_NAME ${target_internal})
SET(PC_INCLUDE "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}/context-service/internal")
SET(PC_LIBDIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
SET(PC_DESCRIPTION "Tizen Context Framework Native Test API")
SET(PC_REQUIRED ${DEPS_INTERNAL})
SET(PC_LDFLAGS -l${target_internal})
SET(PC_CFLAGS -I\${includedir}/context-service/internal)

CONFIGURE_FILE(
	${CMAKE_SOURCE_DIR}/context.pc.in
	${CMAKE_SOURCE_DIR}/${target_internal}.pc
	@ONLY
)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/${target_internal}.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
