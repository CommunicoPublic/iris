#
# IRIS - C++ TCP server/framework
#
PROJECT(IRIS)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0 FATAL_ERROR)

# ################################################################################
# Defaults
#
OPTION(DEBUG_MODE           "Enable debug output [default: OFF]" ON)
OPTION(ENABLE_OPTIMIZATION  "Enable optimization [default: OFF]" ON)
OPTION(SKIP_RELINK_RPATH    "Skip relinking and full RPATH for the install tree [default: OFF]" OFF)
OPTION(ENABLE_TEST_COVERAGE "Enable test coverage [default: OFF]" OFF)

# ################################################################################
# Version
#
SET(IRIS_VERSION_MAJOR 0)
SET(IRIS_VERSION_MINOR 1)
SET(IRIS_VERSION_PATCH 17)

SET(IRIS_VERSION         "${IRIS_VERSION_MAJOR}.${IRIS_VERSION_MINOR}.${IRIS_VERSION_PATCH}")
SET(IRIS_MASTER_SITE_URL "http://www.communico.pro/iris")

# Iris share dir
SET(IRIS_SHAREDIR "${CMAKE_INSTALL_PREFIX}/share/iris")

# Iris configuration file
SET(IRIS_CONFIG_FILE "iris.conf")

# Allow TLS sockets
SET(IRIS_TLS_SUPPORT ON)

# Allow run Iris from super-user
SET(IRIS_BIG_SECURITY_HOLE OFF)

# Die if unrecoverable error occured
SET(IRIS_DIE_ON_UNRECOVERABLE ON)

# ################################################################################
# Macroses
#

# Standard CMake macroses
INCLUDE(CheckIncludeFiles)
INCLUDE(CheckCXXSourceCompiles)
INCLUDE(CheckSymbolExists)
INCLUDE(CheckLibraryExists)
INCLUDE(FindThreads)

# User-defined macroses
INCLUDE(cmake/ConfigureLibrary.cmake)
INCLUDE(cmake/RenameModule.cmake)
INCLUDE(cmake/ConfigureMD5Support.cmake)
INCLUDE(cmake/RagelGenerate.cmake)

# ################################################################################
# Setup libraries & headers
#
CONFIGURE_LIBRARY(ev.h "/usr/include /usr/local/include /opt/include /usr/pkg/include/ev"
                  ev   "/lib /usr/lib /usr/local/lib /opt/lib /usr/pkg/lib/ev")

CONFIGURE_LIBRARY(uuid/uuid.h "/usr/include /usr/local/include /opt/include /usr/pkg/include/uuid"
                  uuid        "/lib /usr/lib /usr/local/lib /opt/lib /usr/pkg/lib/ev")

FIND_PACKAGE(OpenSSL REQUIRED)

# ################################################################################
# Header files
#
INCLUDE_DIRECTORIES(include)
INCLUDE_DIRECTORIES(include/conf)
INCLUDE_DIRECTORIES(include/core)
INCLUDE_DIRECTORIES(include/ev)
INCLUDE_DIRECTORIES(include/mt)
INCLUDE_DIRECTORIES(include/net)
INCLUDE_DIRECTORIES(include/util)
INCLUDE_DIRECTORIES(${OPENSSL_INCLUDE_DIR})

# ################################################################################
# API Headers
#
SET(IRIS_CONF_HEADERS include/conf/ConfigHelper.hpp
                      include/conf/HandlerConfig.hpp
                      include/conf/MainConfig.hpp
                      include/conf/ModuleConfig.hpp
                      include/conf/NetworkThreadWorker.hpp
                      include/conf/ServiceConfig.hpp
    )

SET(IRIS_CORE_HEADERS include/core/AcceptLoopContext.hpp
                      include/core/AcceptLoopStopWatcher.hpp
                      include/core/AcceptLoopWorker.hpp
                      include/core/AcceptSignalWatcher.hpp
                      include/core/GlobalContext.hpp
                      include/core/LoggerContext.hpp
                      include/core/NetworkLoggerContext.hpp
                      include/core/MainLoopContext.hpp
                      include/core/MainLoopEventWatcher.hpp
                      include/core/MainLoopSignalHandler.hpp
                      include/core/MainLoopStopWatcher.hpp
                      include/core/NetworkEventTimer.hpp
                      include/core/NetworkEventWatcher.hpp
                      include/core/NetworkHandler.hpp
                      include/core/NetworkWatcherList.hpp
                      include/core/SSL.hpp
                      include/core/SocketChangeSignalWatcher.hpp
    )

SET(IRIS_EV_HEADERS include/ev/EventLoop.hpp
                    include/ev/EventTimer.hpp
                    include/ev/IOAsyncWatcher.hpp
                    include/ev/IOWatcher.hpp
    )

SET(IRIS_MT_HEADERS include/mt/ConditionVariable.hpp
                    include/mt/Mutex.hpp
                    include/mt/MutexMultiLocker.hpp
                    include/mt/RWLock.hpp
                    include/mt/Semaphore.hpp
                    include/mt/ThreadGroup.hpp
   )

SET(IRIS_NET_HEADERS include/net/ClientTCP4Socket.hpp
                     include/net/ClientTCP6Socket.hpp
                     include/net/ClientTCPUnixSocket.hpp
                     include/net/ClientTCPSocket.hpp
                     include/net/Resolver.hpp
                     include/net/ServerClientTCP4Socket.hpp
                     include/net/ServerClientTCP6Socket.hpp
                     include/net/ServerClientTCPUnixSocket.hpp
                     include/net/ServerTCP4Socket.hpp
                     include/net/ServerTCP6Socket.hpp
                     include/net/ServerTCPUnixSocket.hpp
                     include/net/ServerTCPSocket.hpp
                     include/net/Socket.hpp
                     include/net/SocketPoller.hpp
                     include/net/SocketPoller.tcc
                     include/net/TCPSocket.hpp
    )

IF (IRIS_TLS_SUPPORT MATCHES "ON")
    SET(IRIS_NET_SSL_HEADERS include/net/ClientTCPSSLSocket.hpp
                             include/net/ServerClientTCPSSLSocket.hpp
                             include/net/ServerClientTCP4SSLSocket.hpp
                             include/net/ServerClientTCP6SSLSocket.hpp
                             include/net/ServerClientTCPUnixSSLSocket.hpp
                             include/net/ServerTCPSSLSocket.hpp
                             include/net/ServerTCP4SSLSocket.hpp
                             include/net/ServerTCP6SSLSocket.hpp
                             include/net/ServerTCPUnixSSLSocket.hpp
        )
ENDIF(IRIS_TLS_SUPPORT MATCHES "ON")

SET(IRIS_UTIL_HEADERS include/util/AbstractFactory.hpp
                      include/util/AbstractFactory.tcc
                      include/util/Allocator.hpp
                      include/util/AVLTree.hpp
                      include/util/BlockAllocator.hpp
                      include/util/CharTables.hpp
                      include/util/ConfigFileParser.hpp
                      include/util/CRC32.hpp
                      include/util/DataBuffer.hpp
                      include/util/Debug.hpp
                      include/util/DLLibraryLoader.hpp
                      include/util/Exception.hpp
                      include/util/FileHandleCache.hpp
                      include/util/FileUtils.hpp
                      include/util/FixedSizeAllocator.hpp
                      include/util/GetOpt.hpp
                      include/util/Handler.hpp
                      include/util/IPMap.hpp
                      include/util/IPv4Map.hpp
                      include/util/JSONParser.hpp
                      include/util/JSONParser.tcc
                      include/util/LineBuffer.hpp
                      include/util/LoadableObject.hpp
                      include/util/LoggerFile.hpp
                      include/util/Logger.hpp
                      include/util/LoggerObject.hpp
                      include/util/LogPriorities.hpp
                      include/util/LRUCache.hpp
                      include/util/LRUList.hpp
                      include/util/MallocAllocator.hpp
                      include/util/MessageDigest.hpp
                      include/util/Message.hpp
                      include/util/MessageQueue.hpp
                      include/util/MessageSubscriber.hpp
                      include/util/Module.hpp
                      include/util/ObjectFactory.hpp
                      include/util/Object.hpp
                      include/util/ObjectId.hpp
                      include/util/RateLimit.hpp
                      include/util/RateLimitStorage.hpp
                      include/util/SharedPtr.hpp
                      include/util/ScopedPtr.hpp
                      include/util/SignalHandler.hpp
                      include/util/StringBuffer.hpp
                      include/util/SystemVars.hpp
                      include/util/TimeProfiler.hpp
                      include/util/UnicodeUtils.hpp
                      include/util/UnicodeUtils.tcc
                      include/util/UnitTest.hpp
                      include/util/Utils.hpp
                      include/util/VariantDumper.hpp
                      include/util/VariantDumperJSON.hpp
                      include/util/Variant.hpp
                      include/util/Variant.tcc
   )

SET(IRIS_HEADERS include/Types.h
                 include/IrisConfig.h
                 include/STLAlgorithm.hpp
                 include/STLException.hpp
                 include/STLFunctional.hpp
                 include/STLIosfwd.hpp
                 include/STLList.hpp
                 include/STLMap.hpp
                 include/STLMultimap.hpp
                 include/STLOstream.hpp
                 include/STLPair.hpp
                 include/STLQueue.hpp
                 include/STLSet.hpp
                 include/STLSstream.hpp
                 include/STLString.hpp
                 include/STLVector.hpp
   )

# ################################################################################
# Platform-specific options
#
SET (RT_LIBRARY)
IF("${CMAKE_SYSTEM_NAME}" MATCHES "FreeBSD")
    MESSAGE(STATUS "Building for FreeBSD...")
    SET(IRIS_CONFIG_FILE "/usr/local/etc/iris/iris.conf")

ENDIF("${CMAKE_SYSTEM_NAME}" MATCHES "FreeBSD")

IF("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
    MESSAGE(STATUS "Building for Linux...")
    SET(IRIS_CONFIG_FILE "/etc/iris/iris.conf")

    SET(CMAKE_PLATFORM_BUILD_FLAGS "-D_LARGEFILE64_SOURCE")
    FIND_LIBRARY(RT_LIBRARY NAMES rt PATHS /lib
                                           /usr/lib
                                           /usr/local/lib
                 DOC "Path where the rt libraries can be found")

    CONFIGURE_LIBRARY(dlfcn.h  "/usr/include /usr/local/include /opt/include"
                      dl       "/lib /usr/lib /usr/local/lib /opt/lib")

    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64")

ENDIF("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")

IF("${CMAKE_SYSTEM_NAME}" MATCHES "SunOS")
    MESSAGE(STATUS "Building for SunOS...")
    SET(IRIS_CONFIG_FILE "/opt/csw/etc/iris/iris.conf")

    FIND_LIBRARY(RT_LIBRARY NAMES socket PATHS /lib
                                                  /usr/lib
                                                  /usr/local/lib
                 DOC "Path where the socket library can be found")

    FIND_LIBRARY(RT_LIBRARY NAMES nsl PATHS /lib
                                            /usr/lib
                                            /usr/local/lib
                 DOC "Path where the nsl library can be found")

ENDIF("${CMAKE_SYSTEM_NAME}" MATCHES "SunOS")

IF("${CMAKE_COMPILER_IS_GNUCXX}" MATCHES "1")

    SET(CMAKE_CXX_WARN_FLAGS   "-Werror -Wall -pedantic -Wno-long-long")
    IF(DEBUG_MODE MATCHES "ON")
        MESSAGE(STATUS "#### Debug mode ON")
        SET(CMAKE_CXX_DEFINES      "-g -D_DEBUG")
        SET(CMAKE_CXX_OPTIMIZATION "-O0")
    ELSE (DEBUG_MODE MATCHES "OFF")
        MESSAGE(STATUS "#### Debug mode OFF")
        SET(CMAKE_CXX_DEFINES      "")
        SET(CMAKE_CXX_ARCH_FLAGS   "")
    ENDIF(DEBUG_MODE MATCHES "ON")

    IF(ENABLE_OPTIMIZATION MATCHES "ON")
        MESSAGE(STATUS "#### Optimization ON")
        SET(CMAKE_CXX_OPTIMIZATION "-O3 -fomit-frame-pointer -unroll-loops")
    ENDIF(ENABLE_OPTIMIZATION MATCHES "ON")

    IF(ENABLE_TEST_COVERAGE MATCHES "ON")
        MESSAGE(STATUS "#### Test coverage ON")
        SET(CMAKE_CXX_GCOW "-fprofile-arcs -ftest-coverage ")
    ENDIF(ENABLE_TEST_COVERAGE MATCHES "ON")

    SET(CMAKE_CXX_FLAGS "${CXX_FLAGS} ${CMAKE_CXX_GCOW} ${CMAKE_CXX_WARN_FLAGS} ${CMAKE_CXX_DEFINES} ${CMAKE_CXX_ARCH_FLAGS} ${CMAKE_CXX_OPTIMIZATION}")

    IF("${SKIP_RELINK_RPATH}" MATCHES "ON")
        SET(CMAKE_SKIP_RPATH  TRUE)
        MESSAGE(STATUS "#### Building for Linux...")
        # skip the full RPATH for the build tree
        SET(CMAKE_SKIP_BUILD_RPATH  TRUE)
        # when building, use the install RPATH already
        # (so it doesn't need to relink when installing)
        SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
        # the RPATH to be used when installing
        SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
        # add the automatically determined parts of the RPATH
        # which point to directories outside the build tree to the install RPATH
        SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
    ENDIF("${SKIP_RELINK_RPATH}" MATCHES "ON")
ENDIF("${CMAKE_COMPILER_IS_GNUCXX}" MATCHES "1")

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/IrisConfig.h.cmake ${CMAKE_CURRENT_SOURCE_DIR}/include/IrisConfig.h)

# ################################################################################
# Generate parsers
#
RAGEL_GENERATE(ragel/ConfigFileParser.rl src/util/ConfigFileParser.cpp)
RAGEL_GENERATE(ragel/JSONParser.rl include/util/JSONParser.tcc)

# ################################################################################
# Iris multithread support
#
SET(IRIS_MT_SRCS src/mt/ConditionVariable.cpp
                 src/mt/Mutex.cpp
                 src/mt/RWLock.cpp
                 src/mt/Semaphore.cpp
    )

# ################################################################################
# Iris event machine
#
SET(IRIS_EV_SRCS src/ev/EventLoop.cpp
                 src/ev/EventTimer.cpp
                 src/ev/IOAsyncWatcher.cpp
                 src/ev/IOWatcher.cpp
   )

# ################################################################################
# Iris utilities
#
SET(IRIS_UTIL_SRCS src/util/Allocator.cpp
                   src/util/CRC32.cpp
                   src/util/CharTables.cpp
                   src/util/ConfigFileParser.cpp
                   src/util/DataBuffer.cpp
                   src/util/Debug.cpp
                   src/util/Exception.cpp
                   src/util/FileHandleCache.cpp
                   src/util/BlockAllocator.cpp
                   src/util/FixedSizeAllocator.cpp
                   src/util/GetOpt.cpp
                   src/util/IPMap.cpp
                   src/util/IPv4Map.cpp
                   src/util/LineBuffer.cpp
                   src/util/LogPriorities.cpp
                   src/util/Logger.cpp
                   src/util/LoggerFile.cpp
                   src/util/FileUtils.cpp
                   src/util/MallocAllocator.cpp
                   src/util/MessageDigest.cpp
                   src/util/ObjectId.cpp
                   src/util/RateLimit.cpp
                   src/util/RateLimitStorage.cpp
                   src/util/SignalHandler.cpp
                   src/util/StringBuffer.cpp
                   src/util/SystemVars.cpp
                   src/util/TimeProfiler.cpp
                   src/util/UnicodeUtils.cpp
                   src/util/Utils.cpp
                   src/util/Variant.cpp
                   src/util/VariantDumper.cpp
                   src/util/VariantDumperJSON.cpp
    )

# ################################################################################
# Iris network support
#
SET(IRIS_NET_SRCS src/net/ClientTCP4Socket.cpp
                  src/net/ClientTCP6Socket.cpp
                  src/net/ClientTCPUnixSocket.cpp
                  src/net/ClientTCPSocket.cpp
                  src/net/Resolver.cpp
                  src/net/ServerClientTCP4Socket.cpp
                  src/net/ServerClientTCP6Socket.cpp
                  src/net/ServerClientTCPUnixSocket.cpp
                  src/net/ServerTCP4Socket.cpp
                  src/net/ServerTCP6Socket.cpp
                  src/net/ServerTCPUnixSocket.cpp
                  src/net/ServerTCPSocket.cpp
                  src/net/Socket.cpp
                  src/net/TCPSocket.cpp
    )

# ################################################################################
# Iris network support
#
SET(IRIS_NET_SSL_SRCS
#src/net/ClientTCP4SSLSocket.cpp
#src/net/ClientTCP6SSLSocket.cpp
#src/net/ClientTCPUnixSSLSocket.cpp
                  src/net/ClientTCPSSLSocket.cpp
                  src/net/ServerClientTCPSSLSocket.cpp
                  src/net/ServerClientTCP4SSLSocket.cpp
                  src/net/ServerClientTCP6SSLSocket.cpp
                  src/net/ServerClientTCPUnixSSLSocket.cpp
                  src/net/ServerTCP4SSLSocket.cpp
                  src/net/ServerTCP6SSLSocket.cpp
                  src/net/ServerTCPUnixSSLSocket.cpp
                  src/net/ServerTCPSSLSocket.cpp
    )

# ################################################################################
#  Configuration routines
#
SET(IRIS_CONF_SRCS src/conf/ConfigHelper.cpp
                   src/conf/HandlerConfig.cpp
                   src/conf/MainConfig.cpp
                   src/conf/ModuleConfig.cpp
                   src/conf/NetworkThreadWorker.cpp
                   src/conf/ServiceConfig.cpp
    )

# ################################################################################
# Core library
#
SET(IRIS_CORE_SRCS src/core/AcceptLoopContext.cpp
                   src/core/AcceptLoopStopWatcher.cpp
                   src/core/AcceptLoopWorker.cpp
                   src/core/AcceptSignalWatcher.cpp
                   src/core/GlobalContext.cpp
                   src/core/LoggerContext.cpp
                   src/core/MainLoopEventWatcher.cpp
                   src/core/MainLoopContext.cpp
                   src/core/MainLoopSignalHandler.cpp
                   src/core/MainLoopStopWatcher.cpp
                   src/core/NetworkEventTimer.cpp
                   src/core/NetworkEventWatcher.cpp
                   src/core/NetworkHandler.cpp
                   src/core/NetworkLoggerContext.cpp
                   src/core/NetworkWatcherList.cpp
                   src/core/SocketChangeSignalWatcher.cpp
                   src/core/SSL.cpp
   )

# ################################################################################
# Main binary
#
SET(IRIS_MAIN_SRCS src/UnixUtils.cpp
                   src/IrisMain.cpp
                   src/MainHelpers.cpp
                   src/MainProcess.cpp
                   src/MainSignalHandler.cpp
   )

# ################################################################################
# Various utilities
#
ADD_LIBRARY(iris-util SHARED ${IRIS_UTIL_SRCS})
SET_TARGET_PROPERTIES(iris-util PROPERTIES OUTPUT_NAME iris-util)
SET_TARGET_PROPERTIES(iris-util PROPERTIES LINKER_LANGUAGE CXX)
SET_TARGET_PROPERTIES(iris-util PROPERTIES VERSION ${IRIS_VERSION} SOVERSION ${IRIS_VERSION_MAJOR}.${IRIS_VERSION_MINOR})
SET_TARGET_PROPERTIES(iris-util PROPERTIES COMPILE_FLAGS "-D_THREAD_SAFE -D_PTHREADS -D_REENTRANT")
TARGET_LINK_LIBRARIES(iris-util ${CMAKE_THREAD_LIBS_INIT} ${MD5_LIBRARY} ${RT_LIBRARY} ${HAVE_dl} ${HAVE_uuid})

# ################################################################################
# Mutithread support
#
ADD_LIBRARY(iris-mt SHARED ${IRIS_MT_SRCS})
SET_TARGET_PROPERTIES(iris-mt PROPERTIES OUTPUT_NAME iris-mt)
SET_TARGET_PROPERTIES(iris-mt PROPERTIES LINKER_LANGUAGE CXX)
SET_TARGET_PROPERTIES(iris-mt PROPERTIES VERSION ${IRIS_VERSION} SOVERSION ${IRIS_VERSION_MAJOR}.${IRIS_VERSION_MINOR})
SET_TARGET_PROPERTIES(iris-mt PROPERTIES COMPILE_FLAGS "-D_THREAD_SAFE -D_PTHREADS -D_REENTRANT")
TARGET_LINK_LIBRARIES(iris-mt ${CMAKE_THREAD_LIBS_INIT} ${MD5_LIBRARY} iris-util)

# ################################################################################
# Mutithread support
#
ADD_LIBRARY(iris-ev SHARED ${IRIS_EV_SRCS})
SET_TARGET_PROPERTIES(iris-ev PROPERTIES OUTPUT_NAME iris-ev)
SET_TARGET_PROPERTIES(iris-ev PROPERTIES LINKER_LANGUAGE CXX)
SET_TARGET_PROPERTIES(iris-ev PROPERTIES VERSION ${IRIS_VERSION} SOVERSION ${IRIS_VERSION_MAJOR}.${IRIS_VERSION_MINOR})
TARGET_LINK_LIBRARIES(iris-ev ${CMAKE_THREAD_LIBS_INIT} iris-mt ${HAVE_ev})

# ################################################################################
# Network routines
#
ADD_LIBRARY(iris-net SHARED ${IRIS_NET_SRCS})
SET_TARGET_PROPERTIES(iris-net PROPERTIES OUTPUT_NAME iris-net)
SET_TARGET_PROPERTIES(iris-net PROPERTIES LINKER_LANGUAGE CXX)
SET_TARGET_PROPERTIES(iris-net PROPERTIES VERSION ${IRIS_VERSION} SOVERSION ${IRIS_VERSION_MAJOR}.${IRIS_VERSION_MINOR})
SET_TARGET_PROPERTIES(iris-net PROPERTIES COMPILE_FLAGS "-D_THREAD_SAFE -D_PTHREADS -D_REENTRANT")
TARGET_LINK_LIBRARIES(iris-net ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} ${MD5_LIBRARY})

# ################################################################################
# Network SSL routines
#
IF (IRIS_TLS_SUPPORT MATCHES "ON")
    ADD_LIBRARY(iris-net-ssl SHARED ${IRIS_NET_SSL_SRCS})
    SET_TARGET_PROPERTIES(iris-net-ssl PROPERTIES OUTPUT_NAME iris-net-ssl)
    SET_TARGET_PROPERTIES(iris-net-ssl PROPERTIES LINKER_LANGUAGE CXX)
    SET_TARGET_PROPERTIES(iris-net-ssl PROPERTIES VERSION ${IRIS_VERSION} SOVERSION ${IRIS_VERSION_MAJOR}.${IRIS_VERSION_MINOR})
    SET_TARGET_PROPERTIES(iris-net-ssl PROPERTIES COMPILE_FLAGS "-D_THREAD_SAFE -D_PTHREADS -D_REENTRANT")
    TARGET_LINK_LIBRARIES(iris-net-ssl ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} ${MD5_LIBRARY} iris-net)
    SET(IRIS_NET_SSL_LIBRARY "iris-net-ssl")
ENDIF (IRIS_TLS_SUPPORT MATCHES "ON")

# ################################################################################
# Configuration routines
#
ADD_LIBRARY(iris-conf SHARED ${IRIS_CONF_SRCS})
SET_TARGET_PROPERTIES(iris-conf PROPERTIES OUTPUT_NAME iris-conf)
SET_TARGET_PROPERTIES(iris-conf PROPERTIES LINKER_LANGUAGE CXX)
SET_TARGET_PROPERTIES(iris-conf PROPERTIES VERSION ${IRIS_VERSION} SOVERSION ${IRIS_VERSION_MAJOR}.${IRIS_VERSION_MINOR})
SET_TARGET_PROPERTIES(iris-conf PROPERTIES COMPILE_FLAGS "-D_THREAD_SAFE -D_PTHREADS -D_REENTRANT")
TARGET_LINK_LIBRARIES(iris-conf ${CMAKE_THREAD_LIBS_INIT} iris-util)

# ################################################################################
# Core library routines
#
ADD_LIBRARY(iris-core SHARED ${IRIS_CORE_SRCS})
SET_TARGET_PROPERTIES(iris-core PROPERTIES OUTPUT_NAME iris-core)
SET_TARGET_PROPERTIES(iris-core PROPERTIES LINKER_LANGUAGE CXX)
SET_TARGET_PROPERTIES(iris-core PROPERTIES VERSION ${IRIS_VERSION} SOVERSION ${IRIS_VERSION_MAJOR}.${IRIS_VERSION_MINOR})
SET_TARGET_PROPERTIES(iris-core PROPERTIES COMPILE_FLAGS "-D_THREAD_SAFE -D_PTHREADS -D_REENTRANT")
TARGET_LINK_LIBRARIES(iris-core ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} iris-conf iris-util iris-mt iris-net ${IRIS_NET_SSL_LIBRARY} iris-ev)

# ################################################################################
# Main process
#
ADD_EXECUTABLE(iris-server ${IRIS_MAIN_SRCS})
TARGET_LINK_LIBRARIES(iris-server iris-core)

# ################################################################################
# Loadable modules
#
ADD_LIBRARY(mod_file_logger MODULE modules/IrisFileLogger.cpp)
SET_TARGET_PROPERTIES(mod_file_logger PROPERTIES COMPILE_FLAGS "-D_THREAD_SAFE -D_PTHREADS -D_REENTRANT")
TARGET_LINK_LIBRARIES(mod_file_logger iris-core)
RENAME_MODULE(mod_file_logger mod_file_logger)

INCLUDE_DIRECTORIES(modules/IrisEchoModule/include)
ADD_LIBRARY(mod_iris_echo MODULE modules/IrisEchoModule/src/IrisEchoEventWatcher.cpp
                                 modules/IrisEchoModule/src/IrisEchoHandler.cpp
                                 modules/IrisEchoModule/src/IrisEchoSignalHandler.cpp
                                 modules/IrisEchoModule/src/IrisEchoThreadWorker.cpp)
SET_TARGET_PROPERTIES(mod_iris_echo PROPERTIES COMPILE_FLAGS "-D_THREAD_SAFE -D_PTHREADS -D_REENTRANT")
TARGET_LINK_LIBRARIES(mod_iris_echo iris-core)
RENAME_MODULE(mod_iris_echo mod_iris_echo)

# ################################################################################
# Tests
#
ENABLE_TESTING()

# ################################################################################
# Utilities
#
ADD_EXECUTABLE(logger-test tests/LoggerTest.cpp)
TARGET_LINK_LIBRARIES(logger-test iris-util)

ADD_EXECUTABLE(fixed-size-allocator-test tests/FixedSizeAllocatorTest.cpp)
TARGET_LINK_LIBRARIES(fixed-size-allocator-test iris-util)

ADD_EXECUTABLE(fixed-size-allocator-test2 tests/FixedSizeAllocatorTest2.cpp)
TARGET_LINK_LIBRARIES(fixed-size-allocator-test2 iris-util)

ADD_EXECUTABLE(avl-tree-test tests/AVLTreeTest.cpp)
TARGET_LINK_LIBRARIES(avl-tree-test iris-util)

ADD_EXECUTABLE(avl-tree-test2 tests/AVLTreeTest2.cpp)
TARGET_LINK_LIBRARIES(avl-tree-test2 iris-util)

ADD_EXECUTABLE(avl-tree-test3 tests/AVLTreeTest3.cpp)
TARGET_LINK_LIBRARIES(avl-tree-test3 iris-util)

ADD_EXECUTABLE(avl-tree-test4 tests/AVLTreeTest4.cpp)
TARGET_LINK_LIBRARIES(avl-tree-test4 iris-util)

ADD_EXECUTABLE(avl-tree-test5 tests/AVLTreeTest5.cpp)
TARGET_LINK_LIBRARIES(avl-tree-test5 iris-util)

ADD_EXECUTABLE(lru-list-test tests/LRUListTest.cpp)
TARGET_LINK_LIBRARIES(lru-list-test  iris-util)

ADD_EXECUTABLE(block-allocator-test tests/BlockAllocatorTest.cpp)
TARGET_LINK_LIBRARIES(block-allocator-test iris-util)

ADD_EXECUTABLE(block-allocator-test2 tests/BlockAllocatorTest2.cpp)
TARGET_LINK_LIBRARIES(block-allocator-test2 iris-util)

ADD_EXECUTABLE(signal-test tests/SignalTest.cpp)
TARGET_LINK_LIBRARIES(signal-test iris-util)

ADD_EXECUTABLE(unicode-test tests/UnicodeTest.cpp)
TARGET_LINK_LIBRARIES(unicode-test iris-util)

ADD_EXECUTABLE(unicode-compare-test tests/UnicodeCompareTest.cpp)
TARGET_LINK_LIBRARIES(unicode-compare-test iris-util)

ADD_EXECUTABLE(unicode-find-test tests/UnicodeFindTest.cpp)
TARGET_LINK_LIBRARIES(unicode-find-test iris-util)

ADD_EXECUTABLE(message-digest-test tests/MessageDigestTest.cpp)
TARGET_LINK_LIBRARIES(message-digest-test iris-util)

ADD_EXECUTABLE(crc32-test tests/CRC32Test.cpp)
TARGET_LINK_LIBRARIES(crc32-test iris-util)

ADD_LIBRARY(mod_test_library MODULE tests/TestLibrary.cpp)
TARGET_LINK_LIBRARIES(mod_test_library)
RENAME_MODULE(mod_test_library mod_test_library)

ADD_EXECUTABLE(dl-loader-test tests/DLLoaderTest.cpp)
TARGET_LINK_LIBRARIES(dl-loader-test iris-util)

ADD_EXECUTABLE(time-profiler-test tests/TimeProfilerTest.cpp)
TARGET_LINK_LIBRARIES(time-profiler-test iris-util)

ADD_EXECUTABLE(data-buffer-test tests/DataBufferTest.cpp)
TARGET_LINK_LIBRARIES(data-buffer-test iris-util)

ADD_EXECUTABLE(line-buffer-test tests/LineBufferTest.cpp)
TARGET_LINK_LIBRARIES(line-buffer-test iris-util)

ADD_EXECUTABLE(lru-cache-test tests/LRUCacheTest.cpp)
TARGET_LINK_LIBRARIES(lru-cache-test iris-util)

ADD_EXECUTABLE(file-handle-cache-test tests/FileHandleCacheTest.cpp)
TARGET_LINK_LIBRARIES(file-handle-cache-test iris-util)

ADD_EXECUTABLE(variant-test tests/VariantTest.cpp)
TARGET_LINK_LIBRARIES(variant-test iris-util)

ADD_EXECUTABLE(variant-nc-test tests/VariantNCTest.cpp)
TARGET_LINK_LIBRARIES(variant-nc-test iris-util)

ADD_EXECUTABLE(iris-config-parser tests/ConfigFileParserTest.cpp)
TARGET_LINK_LIBRARIES(iris-config-parser iris-conf iris-util)

ADD_EXECUTABLE(json-parser-test tests/JSONParserTest.cpp)
TARGET_LINK_LIBRARIES(json-parser-test iris-util)

ADD_EXECUTABLE(rate-limit-test tests/RateLimitTest.cpp)
TARGET_LINK_LIBRARIES(rate-limit-test iris-util)

ADD_EXECUTABLE(scoped-ptr-test tests/ScopedPtrTest.cpp)

ADD_EXECUTABLE(shared-ptr-test tests/SharedPtrTest.cpp)

ADD_EXECUTABLE(ipv4-map-test tests/IPv4MapTest.cpp)
TARGET_LINK_LIBRARIES(ipv4-map-test iris-util)

# ################################################################################
# Multithread
#
ADD_EXECUTABLE(mutex-test tests/MutexTest.cpp)
TARGET_LINK_LIBRARIES(mutex-test iris-mt)
SET_TARGET_PROPERTIES(mutex-test PROPERTIES COMPILE_FLAGS "-D_THREAD_SAFE -D_PTHREADS -D_REENTRANT")

ADD_EXECUTABLE(mutex-multi-test tests/MutexMultiTest.cpp)
TARGET_LINK_LIBRARIES(mutex-multi-test iris-mt)
SET_TARGET_PROPERTIES(mutex-multi-test PROPERTIES COMPILE_FLAGS "-D_THREAD_SAFE -D_PTHREADS -D_REENTRANT")

ADD_EXECUTABLE(rwlock-test tests/RWLockTest.cpp)
TARGET_LINK_LIBRARIES(rwlock-test iris-mt)
SET_TARGET_PROPERTIES(rwlock-test PROPERTIES COMPILE_FLAGS "-D_THREAD_SAFE -D_PTHREADS -D_REENTRANT")

ADD_EXECUTABLE(condvar-test tests/ConditionVariableTest.cpp)
TARGET_LINK_LIBRARIES(condvar-test iris-mt)
SET_TARGET_PROPERTIES(condvar-test PROPERTIES COMPILE_FLAGS "-D_THREAD_SAFE -D_PTHREADS -D_REENTRANT")

ADD_EXECUTABLE(cond-test tests/ConditionTest.cpp)
TARGET_LINK_LIBRARIES(cond-test iris-mt)
SET_TARGET_PROPERTIES(cond-test PROPERTIES COMPILE_FLAGS "-D_THREAD_SAFE -D_PTHREADS -D_REENTRANT")

# ################################################################################
# Event machine
#
ADD_EXECUTABLE(ev-echo-test tests/EchoTest.cpp)
TARGET_LINK_LIBRARIES(ev-echo-test ${CMAKE_THREAD_LIBS_INIT} iris-ev)
SET_TARGET_PROPERTIES(ev-echo-test PROPERTIES COMPILE_FLAGS "-D_THREAD_SAFE -D_PTHREADS -D_REENTRANT")

ADD_EXECUTABLE(ev-echo-mt-test tests/EchoMTTest.cpp)
TARGET_LINK_LIBRARIES(ev-echo-mt-test ${CMAKE_THREAD_LIBS_INIT} iris-ev)
SET_TARGET_PROPERTIES(ev-echo-mt-test PROPERTIES COMPILE_FLAGS "-D_THREAD_SAFE -D_PTHREADS -D_REENTRANT")

# ################################################################################
# Network
#
ADD_EXECUTABLE(tcp4-socket-io-test tests/TCP4SocketIOTest.cpp)
TARGET_LINK_LIBRARIES(tcp4-socket-io-test iris-net iris-util)

ADD_EXECUTABLE(tcp6-socket-io-test tests/TCP6SocketIOTest.cpp)
TARGET_LINK_LIBRARIES(tcp6-socket-io-test iris-net iris-util)

ADD_EXECUTABLE(unix-socket-io-test tests/UnixSocketIOTest.cpp)
TARGET_LINK_LIBRARIES(unix-socket-io-test iris-net iris-util)

# ################################################################################
# Network
#
IF (IRIS_TLS_SUPPORT MATCHES "ON")
    ADD_EXECUTABLE(ssl-test tests/SSLTest.cpp)
    TARGET_LINK_LIBRARIES(ssl-test iris-core)
ENDIF (IRIS_TLS_SUPPORT MATCHES "ON")

# ################################################################################
# Need diff utility for testing
#
FIND_PROGRAM(DIFF_EXECUTABLE "diff" /usr/local/bin /usr/bin)

# ################################################################################
# Utilities
#
ADD_TEST(FSAllocatorTest            fixed-size-allocator-test)
ADD_TEST(AVLTreeTest                avl-tree-test 1 2 3 4 5 6 7 8 9 0 11 22 33 44 55 3 2 1)
ADD_TEST(AVLTreeTest2               avl-tree-test2 1 2 3 4 5 6 7 8 9 0 11 22 33 44 55 3 2 1)
ADD_TEST(AVLTreeTest3               avl-tree-test3 1 2 3 4 5 6 7 8 9 0 11 22 33 44 55 3 2 1)
ADD_TEST(AVLTreeTest4               avl-tree-test4 8 16 24 32 14 30 36 17 6 19)
ADD_TEST(AVLTreeTest                avl-tree-test5 1 2 3 4 5 6 7 8 9 0 11 22 33 44 55 3 2 1)

#ADD_TEST(VSAllocatorTest           variable-size-allocator-test)
ADD_TEST(LoggerTest                 logger-test LoggerTest.out)
#ADD_TEST(SignalTest                 signal-test)
ADD_TEST(UnicodeTest                unicode-test "Привет, мир!" UnicodeTest.out)
ADD_TEST(UnicodeCompareTest         unicode-compare-test UnicodeCompareTest.out)
ADD_TEST(MessageDigestTest          message-digest-test "Hello, World!" 65a8e27d8879283831b664bd8b7f0ad4)
ADD_TEST(CRC32Test                  crc32-test "Hello, World!" 0xEC4AC3D0)
ADD_TEST(DLLoaderTest               dl-loader-test)
ADD_TEST(TimeProfilerTest           time-profiler-test)
ADD_TEST(DataBufferTest             data-buffer-test)
ADD_TEST(LRUCacheTest               lru-cache-test)
ADD_TEST(FileHandleCacheTest        file-handle-cache-test)

IF (DIFF_EXECUTABLE)
    ADD_TEST(LoggerTestD              ${DIFF_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/output/LoggerTest.out LoggerTest.out)
    SET_TESTS_PROPERTIES(LoggerTestD  PROPERTIES DEPENDS LoggerTest)

    ADD_TEST(UnicodeTestD             ${DIFF_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/output/UnicodeTest.out UnicodeTest.out)
    SET_TESTS_PROPERTIES(UnicodeTestD PROPERTIES DEPENDS UnicodeTest)

    ADD_TEST(UnicodeCompareTestD      ${DIFF_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tests/output/UnicodeCompareTest.out UnicodeCompareTest.out)
    SET_TESTS_PROPERTIES(UnicodeCompareTestD PROPERTIES DEPENDS UnicodeCompareTest)
ENDIF (DIFF_EXECUTABLE)

# ################################################################################
# Multithread
#
ADD_TEST(MutexTest                  mutex-test)
ADD_TEST(RWLockTest                 rwlock-test)
ADD_TEST(ConditionVariableTest      condvar-test)

# ################################################################################
# Network
#
ADD_TEST(TCP4SocketIOTest           tcp4-socket-io-test)
ADD_TEST(TCP6SocketIOTest           tcp6-socket-io-test)
ADD_TEST(UnixSocketIOTest           unix-socket-io-test)

# ################################################################################
# Core
#
IF (IRIS_TLS_SUPPORT MATCHES "ON")
   ADD_TEST(SSLTest                    ssl-test ${CMAKE_CURRENT_SOURCE_DIR}/ssl/ssl-cert-snakeoil.pem ${CMAKE_CURRENT_SOURCE_DIR}/ssl/ssl-cert-snakeoil.key ${CMAKE_CURRENT_SOURCE_DIR}/ssl/dh1024.pem)
   ADD_TEST(SSLTest_Built_In_DH        ssl-test ${CMAKE_CURRENT_SOURCE_DIR}/ssl/ssl-cert-snakeoil.pem ${CMAKE_CURRENT_SOURCE_DIR}/ssl/ssl-cert-snakeoil.key "")
ENDIF (IRIS_TLS_SUPPORT MATCHES "ON")

SET(IRIS_COMPILER_HELPER "#!/bin/sh
#
# iris Configurator
#
PREFIX=${CMAKE_INSTALL_PREFIX}
CXXFLAGS=\"${CMAKE_CXX_FLAGS}\"
MAKE=${CMAKE_MAKE_PROGRAM}
CC=${CMAKE_C_COMPILER}
CXX=${CMAKE_CXX_COMPILER}
INCLUDE=\"${CMAKE_INSTALL_PREFIX}/include/iris ${CMAKE_INSTALL_PREFIX}/include/iris/client ${CMAKE_INSTALL_PREFIX}/include/iris/conf ${CMAKE_INSTALL_PREFIX}/include/iris/core ${CMAKE_INSTALL_PREFIX}/include/iris/ev ${CMAKE_INSTALL_PREFIX}/include/iris/mt ${CMAKE_INSTALL_PREFIX}/include/iris/net ${CMAKE_INSTALL_PREFIX}/include/iris/util\"
LIBS=\"${CMAKE_INSTALL_PREFIX}/lib\"
VERSION=\"${IRIS_VERSION}\"
SHAREDIR=\"${IRIS_SHAREDIR}\"
LIBEXEC_DIR=\"${IRIS_LIBEXEC_DIR}\"

if test \"x$1\" = \"x\"\; then
  echo \"Usage:\"
  echo \"  iris-config [--version] [--flags] [--cc] [--cxx] [--make] [--libs] [--includes] [--sharedir] [--libexecdir]\"
  echo \"                  ... [see below for complete flag list]\"
  echo \"\"
  echo \"    --version         displays the Iris version number\"
  echo \"    --flags           displays C++ compiler flags\"
  echo \"    --cc              displays executable name of C compiler\"
  echo \"    --cxx             displays executable name of C++ compiler\"
  echo \"    --make            displays executable name of make\"
  echo \"    --libs            displays list of libraries\"
  echo \"    --includes        displays list of include dirs\"
  echo \"    --sharedir        displays directory with shared files, examples, etc\"
  echo \"    --libexecdir      displays directory with loadable modules\"
  echo \"\"
else
   while test \"x$done\" = \"x\" -a \"x$1\" != \"x\"\; do
       case $1 in
           --version*)
           echo \${VERSION}
           \;\;

           --flags*)
           echo \${CXXFLAGS}
           \;\;

           --cc*)
           echo \${CC}
           \;\;

           --cxx*)
           echo \${CXX}
           \;\;

           --make*)
           echo \${MAKE}
           \;\;

           --libs*)
           echo \${LIBS}
           \;\;

           --includes*)
           echo \${INCLUDE}
           \;\;

           --sharedir*)
           echo \${SHAREDIR}
           \;\;

           --libexecdir*)
           echo \${LIBEXEC_DIR}
           \;\;
       esac
       shift
   done
fi
")

# Install hints for compiler
FILE(WRITE iris-config ${IRIS_COMPILER_HELPER})

# ################################################################################
# Install binaries
#
INSTALL(FILES iris-config
              iris-config-parser
        DESTINATION bin
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                    GROUP_READ GROUP_EXECUTE
                    WORLD_READ WORLD_EXECUTE)

INSTALL(FILES iris-server
        DESTINATION sbin
        PERMISSIONS OWNER_READ OWNER_WRITE GROUP_EXECUTE
                    GROUP_READ GROUP_EXECUTE
                    WORLD_READ WORLD_EXECUTE)

# ################################################################################
# Install library headers
#
INSTALL(FILES ${IRIS_HEADERS}
        DESTINATION include/iris
        PERMISSIONS OWNER_READ OWNER_WRITE
                    GROUP_READ
                    WORLD_READ)

INSTALL(FILES ${IRIS_CONF_HEADERS}
        DESTINATION include/iris/conf
        PERMISSIONS OWNER_READ OWNER_WRITE
                    GROUP_READ
                    WORLD_READ)

INSTALL(FILES ${IRIS_CORE_HEADERS}
        DESTINATION include/iris/core
        PERMISSIONS OWNER_READ OWNER_WRITE
                    GROUP_READ
                    WORLD_READ)

INSTALL(FILES ${IRIS_EV_HEADERS}
        DESTINATION include/iris/ev
        PERMISSIONS OWNER_READ OWNER_WRITE
                    GROUP_READ
                    WORLD_READ)

INSTALL(FILES ${IRIS_MT_HEADERS}
        DESTINATION include/iris/mt
        PERMISSIONS OWNER_READ OWNER_WRITE
                    GROUP_READ
                    WORLD_READ)

INSTALL(FILES ${IRIS_NET_HEADERS}
              ${IRIS_NET_SSL_HEADERS}
        DESTINATION include/iris/net
        PERMISSIONS OWNER_READ OWNER_WRITE
                    GROUP_READ
                    WORLD_READ)

INSTALL(FILES ${IRIS_UTIL_HEADERS}
        DESTINATION include/iris/util
        PERMISSIONS OWNER_READ OWNER_WRITE
                    GROUP_READ
                    WORLD_READ)

# ################################################################################
# Install library
#
INSTALL(TARGETS iris-conf
                iris-core
                iris-ev
                iris-net
                ${IRIS_NET_SSL_LIBRARY}
                iris-mt
                iris-util
        DESTINATION lib
        PERMISSIONS OWNER_READ OWNER_WRITE
                    GROUP_READ
                    WORLD_READ)

# ################################################################################
# Install loadable modules
#
INSTALL(FILES mod_file_logger.so
              mod_iris_echo.so
        DESTINATION lib/iris
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
                    GROUP_READ GROUP_EXECUTE
                    WORLD_READ WORLD_EXECUTE)

# ################################################################################
# Install man pages
#
INSTALL(FILES man/iris-config-parser.1
              man/iris-config-test.1
        DESTINATION share/man/man1
        PERMISSIONS OWNER_READ OWNER_WRITE
                    GROUP_READ
                    WORLD_READ)

INSTALL(FILES man/iris-config.8
              man/iris-server.8
              man/iris-server-ctl.8
        DESTINATION share/man/man8
        PERMISSIONS OWNER_READ OWNER_WRITE
                    GROUP_READ
                    WORLD_READ)

# ################################################################################
# Install example configuration
#
INSTALL(FILES conf/iris.conf
        DESTINATION ${IRIS_SHAREDIR}/conf
        PERMISSIONS OWNER_READ OWNER_WRITE
                    GROUP_READ
                    WORLD_READ)

INSTALL(FILES conf/handlers.d/echo-handler.conf
        DESTINATION ${IRIS_SHAREDIR}/conf/handlers.d
        PERMISSIONS OWNER_READ OWNER_WRITE
                    GROUP_READ
                    WORLD_READ)

INSTALL(FILES conf/loggers.d/general-logger.conf
        DESTINATION ${IRIS_SHAREDIR}/conf/loggers.d
        PERMISSIONS OWNER_READ OWNER_WRITE
                    GROUP_READ
                    WORLD_READ)

INSTALL(FILES conf/services.d/echo-service.conf
              conf/services.d/echo-ssl-service.conf
        DESTINATION ${IRIS_SHAREDIR}/conf/services.d
        PERMISSIONS OWNER_READ OWNER_WRITE
                    GROUP_READ
                    WORLD_READ)

IF (IRIS_TLS_SUPPORT MATCHES "ON")
    INSTALL(FILES ssl/dh1024.pem
                  ssl/ssl-cert-snakeoil.key
                  ssl/ssl-cert-snakeoil.pem
            DESTINATION ${IRIS_SHAREDIR}/conf/ssl
            PERMISSIONS OWNER_READ OWNER_WRITE
                        GROUP_READ
                        WORLD_READ)
ENDIF (IRIS_TLS_SUPPORT MATCHES "ON")

# ########################################################
# End
