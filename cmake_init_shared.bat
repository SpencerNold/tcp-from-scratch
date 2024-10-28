rmdir /S /Q build
mkdir build
cmake -DCMAKE_BUILD_TYPE=Release -DLIB_TYPE=SHARED -B build -S .