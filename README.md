Project Structure

```
TKiK/
├── grammar/
│   ├── lexer.l
│   └── parser.y
├── include/
│   ├── ast/
│   │   ├── Statement.h
│   │   ├── SelectStmt.h
│   │   ├── CreateStmt.h
│   │   └── ...
│   ├── storage/
│   │   ├── Table.h
│   │   ├── Cell.h
│   │   └── Database.h
│   ├── executor/
│   │   ├── Executor.h
│   │   └── SelectExecutor.h
│   └── utils/
│       └── Types.h
└── src/
    ├── ast/
    │   └── ...
    ├── storage/
    │   └── ...
    ├── executor/
    │   └── ...
    └── main.cpp
```
