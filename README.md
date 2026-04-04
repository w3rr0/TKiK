Project Structure

```
TKiK/
├── grammar/
│   ├── lexer.l
│   └── parser.y
├── include/
│   ├── ast/
│   │   ├── Statement.hpp
│   │   ├── SelectStmt.hpp
│   │   ├── CreateStmt.hpp
│   │   └── ...
│   ├── storage/
│   │   ├── Table.hpp
│   │   ├── Cell.hpp
│   │   └── Database.hpp
│   ├── executor/
│   │   ├── Executor.hpp
│   │   └── SelectExecutor.hpp
│   └── utils/
│       └── Types.hpp
└── src/
    ├── ast/
    │   └── ...
    ├── storage/
    │   └── ...
    ├── executor/
    │   └── ...
    └── main.cpp
```
