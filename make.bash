make.bash
x86_64-w64-mingw32-g++ -Wall adjust-volume-abbreviated.cc -o adjust-volume.exe -lole32 -static
x86_64-w64-mingw32-g++ -Wall toggle-volume.cc -o toggle-volume.exe -lole32 -static
