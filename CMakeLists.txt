cmake_minimum_required(VERSION 3.1)

project(smt-switch)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include_directories ("${PROJECT_SOURCE_DIR}/include")

# add to search path for find_package
list(APPEND CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/deps")

if (NOT SMT_SWITCH_LIB_TYPE)
  set(SMT_SWITCH_LIB_TYPE SHARED)
endif()

add_library(smt-switch "${SMT_SWITCH_LIB_TYPE}"
  "${PROJECT_SOURCE_DIR}/src/ops.cpp"
  "${PROJECT_SOURCE_DIR}/src/result.cpp"
  "${PROJECT_SOURCE_DIR}/src/sort.cpp"
  "${PROJECT_SOURCE_DIR}/src/term.cpp"
  "${PROJECT_SOURCE_DIR}/src/solver.cpp"
  "${PROJECT_SOURCE_DIR}/src/term_translator.cpp"
  "${PROJECT_SOURCE_DIR}/src/identity_walker.cpp")

# Should we build python bindings
option (BUILD_PYTHON_BINDINGS
   "Build Python bindings")
option (USE_PYTHON2
   "Use Python2 for bindings")

# Decide on Python version before setting up googletest
# Otherwise might use wrong version
if (BUILD_PYTHON_BINDINGS)
  if (USE_PYTHON2)
    message("-- We understand: it's hard to let go. We'll try to use Python2, but it's not really supported, so no promises.")
    find_package(PythonInterp 2.7 REQUIRED)
  else()
    find_package(PythonInterp 3 REQUIRED)
  endif()
endif()

# build testing infrastructure
enable_testing()

# should we build boolector?
option (BUILD_BTOR
  "Should we build the libraries for boolector" OFF)

if (BUILD_BTOR)
  set(BTOR_HOME "${PROJECT_SOURCE_DIR}/deps/boolector")
  add_subdirectory (btor)
  set (SOLVER_BACKEND_LIBS ${SOLVER_BACKEND_LIBS} smt-switch-btor)

  # testing
  add_subdirectory(tests/btor)
endif (BUILD_BTOR)


# should we build cvc4?
option (BUILD_CVC4
  "Should we build the libraries for cvc4" OFF)

if (BUILD_CVC4)
  set(CVC4_HOME "${PROJECT_SOURCE_DIR}/deps/CVC4")
  add_subdirectory (cvc4)
  set (SOLVER_BACKEND_LIBS ${SOLVER_BACKEND_LIBS} smt-switch-cvc4)

  # testing
  add_subdirectory(tests/cvc4)
endif (BUILD_CVC4)

# should we build msat?
option (BUILD_MSAT
  "Should we build the libraries for mathsat" OFF)

if (BUILD_MSAT)
  set(MSAT_HOME "${PROJECT_SOURCE_DIR}/deps/mathsat")
  add_subdirectory (msat)
  set (SOLVER_BACKEND_LIBS ${SOLVER_BACKEND_LIBS} smt-switch-msat)

  # testing
  add_subdirectory(tests/msat)

endif()

if (BUILD_PYTHON_BINDINGS)
  add_subdirectory(python)
endif()

# Add tests subdirectory
# The CMakeLists.txt file there sets up googletest
# and builds all the parametrized tests
add_subdirectory(tests)

# install smt-switch
install(TARGETS smt-switch DESTINATION lib)

# install public headers
install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/"
        DESTINATION include/smt-switch
        FILES_MATCHING PATTERN "*.h")

# uninstall target
# copied from https://gitlab.kitware.com/cmake/community/wikis/FAQ#can-i-do-make-uninstall-with-cmake
if(NOT TARGET uninstall)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE @ONLY)

    add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()
