# If project not ready, generate cmake file.
if [[ ! -d build ]]; then
    mkdir -p build
fi

# Build project.
cmake -S . -B build
cmake --build build
