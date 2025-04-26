# Labs

### Чтобы запустить CMAke ведите команды:

```aiignore
cmake -DCMAKE_BUILD_TYPE:STRING=Debug -S./ -B./build
cmake --build build
cd build
./tests_deque
cmake --build . --target coverage_deque
```
### Удаление сборки:
```aiignore
rm -rf build
```