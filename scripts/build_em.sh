emcmake cmake ../ -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -B ../embuild -DCMAKE_BUILD_TYPE=DEBUG
cd ../embuild/
emmake make
cd ../scripts