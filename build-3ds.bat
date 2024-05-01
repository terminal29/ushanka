set ORIGINAL_DIR=%CD% 
REM
mkdir build
cd build 
cmake .. -DCMAKE_TOOLCHAIN_FILE=platform/3ds/DevkitArm3DS.cmake -DCMAKE_CXX_FLAGS=-fdiagnostics-color=always -G Ninja --fresh
ninja ushanka_3ds
chdir /d %ORIGINAL_DIR% 