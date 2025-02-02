# SQLite for Umka

Database engine bindings for the [Umka](https://github.com/vtereshkov/umka-lang) programming language

## Quick reference card

```
// Database connection
type Database* = struct {/* Implementation-defined */}

fn open*(name: str): (^Database, std::Err)
fn (db: ^Database) prepare*(request: str): (^Row, std::Err)


// Row iterator
type Row* = struct {/* Implementation-defined */}

fn (row: ^Row) step*(): (bool, std::Err)
fn (row: ^Row) count*(): int
fn (row: ^Row) name*(col: int): str
fn (row: ^Row) names*(): []str
fn (row: ^Row) value*(col: int): any
fn (row: ^Row) values*(): []any


// Error codes
type Result* = enum {/* ... */}
```

## Usage example

```
import (
    "std.um"
    "sqlite.um"
)

fn getStr(): str {
    s := ""
    for true {
        c := std::getchar()
        if c == '\n' {
            break
        }
        s += c
    }
    return s
}

fn main() {
    db, err := sqlite::open(std::argv(1))
    std::exitif(err)

    for true {
        printf("\n>>> ")  
        request := getStr()
        if request == "" {
            break
        }

        row, err := db.prepare(request)
        if sqlite::Result(err.code) != .OK {
            printf("SQLite error: %s\n", err.msg)
            continue
        }

        got, err := row.step()
        for got {
            printf("\n<<< %v\n", row.values())
            got, err = row.step()
        }

        if sqlite::Result(err.code) != .OK {
            printf("SQLite error: %s\n", err.msg)
            continue
        }       
    }   
}
```
