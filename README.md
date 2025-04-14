# Labs

### Чтобы запустить CMAke ведите команды:

```aiignore
mkdir build && cd build
cmake -DCMAKE_CXX_FLAGS="-fsanitize=address" -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=address" -DCMAKE_BUILD_TYPE=Debug ..  # Или Release
cmake --build .         # Сборка
ctest -V               # Запуск тестов
```
### Удаление сборки:
```aiignore
rm -rf build
```