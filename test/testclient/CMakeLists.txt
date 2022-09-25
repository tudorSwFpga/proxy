# CMakeList.txt : projet CMake pour CTA_SW_Test, incluez la source et définissez
# la logique spécifique au projet ici.
#
cmake_minimum_required (VERSION 3.8)
set (CMAKE_CXX_STANDARD 14)
set(THREADS_PREFER_PTHREAD_FLAG ON)





project ("v2x_test")
find_package(Threads REQUIRED)


# Ajoutez une source à l'exécutable de ce projet.
add_executable (testClient "testclient.cpp")
