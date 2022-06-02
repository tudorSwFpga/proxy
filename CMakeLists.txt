# CMakeList.txt : projet CMake pour CTA_SW_Test, incluez la source et définissez
# la logique spécifique au projet ici.
#
cmake_minimum_required (VERSION 3.8)
include(${CMAKE_CURRENT_SOURCE_DIR}/plog/CMakeLists.txt)

set (CMAKE_CXX_STANDARD 14)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_BUILD_TYPE Debug)
set(HEADER_FILES "/usr/local/include/plog")




project ("v2x_test")
find_package(Threads REQUIRED)



# Ajoutez une source à l'exécutable de ce projet.
include_directories("/usr/local/include/plog")
add_executable (v2x_test "main.cpp" "proxy.hpp" "proxy.cpp" "runnable.hpp" "runnable.cpp" "common.hpp" "plog/include/plog/Log.h"
			    "dataManager.hpp" "dataManager.cpp")
target_link_libraries(v2x_test Threads::Threads)