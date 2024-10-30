rmdir /S /Q build
mkdir build
cmake -DCMAKE_BUILD_TYPE=Release -DLIB_TYPE=STATIC -DNPCAP_LIB="C:\Program Files\Npcap\Lib" -DNPCAP_INCLUDE="C:\Program Files\Npcap\Include" -B build -S .