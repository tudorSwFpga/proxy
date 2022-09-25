# CMakeList.txt : projet CMake pour CTA_SW_Test, incluez la source et définissez
# la logique spécifique au projet ici.
#
cmake_minimum_required (VERSION 3.8)
set (CMAKE_CXX_STANDARD 14)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_BUILD_TYPE Debug)

project ("v2x_test")
find_package(Threads REQUIRED)
add_subdirectory(plog)


# Ajoutez une source à l'exécutable de ce projet.
#include_directories("/usr/local/include/plog")
add_executable (testFramework "main.cpp" "proxy.hpp" "runnable.hpp" "runnable.cpp" "common.hpp" "plog/include/plog/Log.h"
			    "dataManager.hpp" "consumer.hpp")
		    target_link_libraries(testFramework 
	Threads::Threads 
	plog::plog)
