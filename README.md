# tensor_compiler

Минимальный C++ frontend для ONNX-графов.

## Уже реализовано
- Внутренний граф (`Node`, `Graph`) с проверкой DAG.
- Загрузка ONNX в граф (`Add`, `Mul`, `Conv`, `Relu`, `MatMul`, `Gemm`).
- Базовые алгоритмы (`topological_sort`, `dfs`, `bfs`).
- Экспорт графа в DOT.
- Тесты: unit + интеграционные.

## Быстрый старт

Требования: CMake 3.16+, C++17, Protobuf, Abseil.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/fe
```

## Тесты

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON
cmake --build build --target graph_tests onnx_loader_tests frontend_pipeline_tests
ctest --test-dir build --output-on-failure
```

Опция CMake: `BUILD_TESTS=ON` включает тестовые таргеты.
