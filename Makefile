ctemplate:
	mkdir -p ./gen
	ctemplate ./test/types.ctypes ./src/zsl.ht ./gen/zsl.h

proj: ctemplate
	cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug
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
