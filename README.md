# sqW database


Autorski interpreter języka SQL oraz lokalny silnik bazodanowy zaimplementowany w C++.
Program pozwala na zarządzanie strukturą tabel oraz manipulację danymi i wykonywanie zapytań.

## Założenia programu
* **Ogólne cele:** Implementacja interpretera języka SQL oraz silnika bazodanowego w języku C++. 
Celem projektu jest stworzenie systemu zarządzania danymi, który oferuje pełną obsługę typów (INT, DOUBLE, VARCHAR, BOOL) 
oraz obsługę wartości NULL. Program umożliwia definiowanie struktur tabel (`CREATE`, `DROP`, `ALTER`) oraz manipulację 
rekordami (`INSERT`, `UPDATE`, `DELETE`, `SELECT`).
* **Rodzaj translatora:** Interpreter.
* **Planowany wynik:** Funkcjonalny silnik bazodanowy, który parsuje zapytania SQL do drzewa AST, a następnie wykonuje je na 
autorskiej strukturze danych (klasy Database, Table, Column, Cell). Silnik zapewnia bezpieczeństwo typów, obsługę brakujących 
danych (NULL) oraz prezentację wyników zapytań wraz z ich filtrowaniem i agregacją.
* **Język implementacji:** C++.
* **Realizacja skanera/parsera:** Generatory Flex i Bison.

---

## Dialect
Poniższa tabela zawiera spis tokenów zdefiniowanych w skanerze `lexer.l`.

| Kategoria | Tokeny                                                                                                                         | Opis                                                                                         |
| :--- |:-------------------------------------------------------------------------------------------------------------------------------|:---------------------------------------------------------------------------------------------|
| **Słowa kluczowe (DML/DDL)** | `SELECT`, `FROM`, `WHERE`, `CREATE`, `TABLE`, `INSERT`, `INTO`, `VALUES`, `UPDATE`, `DELETE`, `SET`, `DROP`, `COLUMN`, `ALTER` | Podstawowe komendy zarządzania danymi i strukturą.                                           |
| **Logika i Filtrowanie** | `IN`, `LIKE`, `BETWEEN`, `AND`, `OR`                                                                                           | Operatory warunkowe używane w klauzuli WHERE.                                                |
| **Agregacja i Sortowanie** | `COUNT`, `SUM`, `MIN`, `MAX`, `ORDER`, `BY`, `ASC`, `DESC`, `DISTINCT`, `LIMIT`, `OFFSET`                                      | Funkcje obliczeniowe i sterowanie prezentacją danych.                                        |
| **Typy Danych** | `INT`, `STRING`, `BOOL`, `DOUBLE`, `FLOAT`, `NULL`                                                                             | Obsługiwane typy kolumn w komendzie CREATE TABLE.                                            |
| **Operatory i Symbole** | `STAR` (*), `EQ` (=), `GT` (>), `GE` (>=), `LE` (<=), `LT` (<), `LPAREN` ('('), `RPAREN` (')'), `COMMA` (,), `SEMICOLON` (;)   | Operatory porównania i znaki interpunkcyjne SQL.                                             |
| **Wartości i Nazwy** | `NUMBER`, `ID`, `STR`                                                                                                          | Liczby całkowite, identyfikatory (nazwy) oraz napisy w apostrofach.           |

---

## Prerequisites

To build this project, you need to have the following tools installed:

* **Bison**
* **Flex**
* **GLFW**

  ```shell
  sudo apt install bison flex libglfw3-dev    # Linux (Debian)
  ```

  ```shell
  brew install bison flex glfw                # MacOS
  ```
  
---

## Getting Started

Follow these steps to build and run the application from the root directory.

1. **Configure the project** \
   Create a build directory and configure the project using CMake:
   ```shell
   cmake -S . -B build
   ```
2. **Build the application** \
   Compile the source code:
   ```shell
    cmake --build build
   ```
3. **Run the application** \
   After a successful build, you can run the executable.
   ```shell
   ./build/Interpreter-SQL             # Linux / MacOS
   ```
   ```powershell
   .\build\Debug\Interpreter-SQL.exe   # Windows
   ```

---

## Gramatyka projektu (Notacja Bison)
Pełna gramatyka znajduje się w pliku: 
**[grammar/parser.y](./grammar/parser.y)**

---

## Project Structure

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

---

## License

The program is licensed under the MIT license.
Details in the [License](LICENSE) file

---

## Contact

[Krzysztof Leśniak](https://github.com/krlesniak) \
[krlesniak@student.agh.edu.pl](mailto:krlesniak@student.agh.edu.pl)

[Konrad Mateja](https://github.com/w3rr0) \
[kmateja@student.agh.edu.pl](mailto:kmateja@student.agh.edu.pl)