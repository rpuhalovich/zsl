build:
	cmake --build build

ctemplate:
	mkdir -p ./gen
	ctemplate ./src/types.ctypes ./src/array.ht ./gen/array.h
	ctemplate ./src/types.ctypes ./src/array.ct ./gen/array.c
	ctemplate ./src/types.ctypes ./src/result.ht ./gen/result.h

ninja: ctemplate
	cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug -G Ninja
	cp build/compile_commands.json compile_commands.json
	ctags -R .

xcode: ctemplate
	cmake -S . -B xcode -GXcode

release:
	cmake -S . -B release -DCMAKE_BUILD_TYPE=Release -G Ninja
	cmake --build release --config Release

test: ninja
	cmake --build build
	./build/app_tests

check: test
	find src -iname "*.h" -o -iname "*.c" | xargs clang-format -i --style=file
	find src -iname "*.h" -o -iname "*.c" | xargs clang-tidy

clean:
	rm -rf build release xcode gen

.PHONY: check xcode makefile build release clean ninja test ctemplate
.SILENT:
