# Build
mkdir -p build && cd build
cmake ..
make

# Install
cd mcmpy
pip install .
