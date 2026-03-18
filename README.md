conan install . --build=missing --profile=native -of ./conan --deployer=full_deploy --envs-generation=false
cmake -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=... && cmake --build build && cmake --install build
