ctemplate:
	mkdir -p ./gen
	ctemplate ./src/types.ctypes ./src/array.ht ./gen/array.h
	ctemplate ./src/types.ctypes ./src/array.ct ./gen/array.c
	ctemplate ./src/types.ctypes ./src/result.ht ./gen/result.h
	ctemplate ./src/types.ctypes ./src/result.ct ./gen/result.c

proj: ctemplate
	cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug -G Ninja
	cp build/compile_commands.json compile_commands.json
	ctags -R .

test: proj check
	cmake --build build
	leaks -atExit -- ./build/ztest

check:
	find src -iname "*.h" -o -iname "*.c" | xargs clang-format -i --style=file
	find src -iname "*.h" -o -iname "*.c" | xargs clang-tidy

clean:
	rm -rf build gen

.PHONY: check clean proj test ctemplate
.SILENT:
