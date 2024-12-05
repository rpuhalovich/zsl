ctemplate:
	mkdir -p ./gen
	ctemplate ./src/_types.ctypes ./src/zarray.ht ./gen/zarray.h
	ctemplate ./src/_types.ctypes ./src/zarray.ct ./gen/zarray.c
	ctemplate ./src/_types.ctypes ./src/zresult.ht ./gen/zresult.h
	ctemplate ./src/_types.ctypes ./src/zresult.ct ./gen/zresult.c

proj: ctemplate
	cmake -S . -B build -DBUILD_TESTS=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug -GNinja
	cp build/compile_commands.json compile_commands.json
	ctags -R .

test: proj
	cmake --build build
	./build/ztest

format:
	find src -iname "*.h" -o -iname "*.c" | xargs clang-format -i --style=file
	find test -iname "*.h" -o -iname "*.c" | xargs clang-format -i --style=file

tidy:
	find src -iname "*.h" -o -iname "*.c" | xargs clang-tidy

check: clean ctemplate proj format tidy
	cmake --build build
	leaks --atExit -- ./build/ztest

clean:
	rm -rf build gen

.PHONY: check clean proj test ctemplate
.SILENT:
