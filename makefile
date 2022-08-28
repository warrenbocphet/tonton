default: cmake

cmake: cmake_config cmake_build cmake_install 

cmake_config:
	cmake -S . -B build -DgRPC_INSTALL=ON -DgRPC_BUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=install -DCMAKE_FIND_USE_PACKAGE_REGISTRY=OFF

cmake_build:
	cmake --build build --parallel 4 --config Release

cmake_install:
	cmake --install build

clean:
	rm -rf build
	rm -rf install

protoc:
	build/protobuf/bin/protoc -I protos --grpc_out=libs/localisation --plugin=protoc-gen-grpc=build/grpc/bin/grpc_cpp_plugin localisation.proto
	build/protobuf/bin/protoc -I protos --cpp_out=libs/localisation localisation.proto