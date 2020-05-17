add_definitions(-DGLM_EXT_INCLUDED -DGLM_FORCE_RADIANS)

set(GLM_TEST_ENABLE_CXX_17 ON CACHE BOOL "Enable GLM C++ 17 features.")
set(GLM_TEST_ENABLE OFF CACHE BOOL "Disable GLM unit tests build.")

add_subdirectory(glm)

add_subdirectory(assimp)
