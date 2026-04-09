# Projekt: Interpreter języka SQL w C++ z silnikiem bazodanowym

---

## 1. Dane autorów
* **Krzysztof Leśniak**
* **Konrad Mateja**
* **Kontakt:** 
  * krlesniak@student.agh.edu.pl
  * kmateja@student.agh.edu.pl

---

## 2. Założenia programu
* **Ogólne cele:** Implementacja interpretera języka SQL oraz relacyjnego silnika bazodanowego w języku C++. 
Celem projektu jest stworzenie systemu zarządzania danymi, który oferuje pełną obsługę typów (INT, DOUBLE, TEXT, BOOL) 
oraz obsługę wartości NULL. Program umożliwia definiowanie struktur tabel (`CREATE`, `DROP`, `ALTER`) oraz manipulację 
rekordami (`INSERT`, `UPDATE`, `DELETE`, `SELECT`).
* **Rodzaj translatora:** Interpreter.
* **Planowany wynik:** Funkcjonalny silnik bazodanowy, który parsuje zapytania SQL do drzewa AST, a następnie wykonuje je na 
autorskiej strukturze danych (klasy Database, Table, Column, Cell). Silnik zapewnia bezpieczeństwo typów, obsługę brakujących 
danych (NULL) oraz prezentację wyników zapytań wraz z ich filtrowaniem i agregacją.
* **Język implementacji:** C++.
* **Realizacja skanera/parsera:** Generatory Flex i Bison.

---

## 3. Opis tokenów
Poniższa tabela zawiera spis tokenów zdefiniowanych w skanerze `lexer.l`.

| Kategoria | Tokeny                                                                                                                         | Opis                                                                                         |
| :--- |:-------------------------------------------------------------------------------------------------------------------------------|:---------------------------------------------------------------------------------------------|
| **Słowa kluczowe (DML/DDL)** | `SELECT`, `FROM`, `WHERE`, `CREATE`, `TABLE`, `INSERT`, `INTO`, `VALUES`, `UPDATE`, `DELETE`, `SET`, `DROP`, `COLUMN`, `ALTER` | Podstawowe komendy zarządzania danymi i strukturą.                                           |
| **Logika i Filtrowanie** | `IN`, `LIKE`, `BETWEEN`, `AND`, `OR`                                                                                           | Operatory warunkowe używane w klauzuli WHERE.                                                |
| **Agregacja i Sortowanie** | `COUNT`, `SUM`, `MIN`, `MAX`, `ORDER`, `BY`, `ASC`, `DESC`, `DISTINCT`                                                         | Funkcje obliczeniowe i sterowanie prezentacją danych.                                        |
| **Typy Danych** | `TYPE_INT`, `TYPE_STRING`, `TYPE_BOOL`, `TYPE_DOUBLE`, `TYPE_FLOAT`, `TYPE_NULL`                                               | Obsługiwane typy kolumn w komendzie CREATE TABLE.                                            |
| **Operatory i Symbole** | `STAR` (*), `EQ` (=), `GT` (>), `GE` (>=), `LE` (<=), `LT` (<), `LPAREN` ('('), `RPAREN` (')'), `COMMA` (,), `SEMICOLON` (;)   | Operatory porównania i znaki interpunkcyjne SQL.                                             |
| **Wartości i Nazwy** | `NUMBER`, `ID`, `STR`                                                                                                          | Liczby całkowite, identyfikatory (nazwy) oraz napisy w apostrofach.           |

---

## 4. Gramatyka projektu (Notacja Bison)
Pełna gramatyka znajduje się w pliku: 
**[grammar/parser.y](./grammar/parser.y)**

---

## 5. Project Structure

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
