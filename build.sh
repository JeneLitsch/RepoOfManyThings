cmake . -D CMAKE_CXX_COMPILER=clang++ -B build -S .
cd build
make -j8
cd ..
mkdir bin
mv ./build/bin/visit_erased bin/visit_erased