set SS_PLATFORM=win32
cd ..
cd ..
mkdir build
cd build
del CMakeCache.txt
cmake -G "Visual Studio 15 2017" .. -DMSVC_RUNTIME=dynamic -DCMAKE_BUILD_TYPE=Debug -DTESTS=true -DCMAKE_USE_PTHREADS_INIT=1
cd ..
cd ./buildscripts/debug/