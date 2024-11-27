ctemplate:
	mkdir -p ./gen
	ctemplate ./src/types.ctypes ./src/array.ht ./gen/array.h
	ctemplate ./src/types.ctypes ./src/array.ct ./gen/array.c
	ctemplate ./src/types.ctypes ./src/result.ht ./gen/result.h
	ctemplate ./src/types.ctypes ./src/result.ct ./gen/result.c

proj: ctemplate
	cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug
	cp build/compile_commands.json compile_commands.json
	ctags -R .

test: proj
	cmake --build build
	leaks -q -atExit -- ./build/ztest

format:
	find src -iname "*.h" -o -iname "*.c" | xargs clang-format -i --style=file

tidy:
	find src -iname "*.h" -o -iname "*.c" | xargs clang-tidy

check: clean ctemplate proj format tidy test

clean:
	rm -rf build gen

.PHONY: check clean proj test ctemplate
.SILENT:
