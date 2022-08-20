default: cmake

cmake: cmake_config cmake_build cmake_install 

cmake_config:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=install -DCMAKE_FIND_USE_PACKAGE_REGISTRY=OFF

cmake_build:
	cmake --build build --parallel --config Release

cmake_install:
	cmake --install build

clean:
	rm -r build
	rm -r install