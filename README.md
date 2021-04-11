# Task10_4
## Интерпретатор модельного языка программирования

### Синтаксис модельного языка
    <программа>::={ <описание> } int main() <составной оператор>                        +
    <имя>::=<буква>|<имя><буква>|<имя><цифра>                                           +
    <оператор>::=<оператор выражения> | <оператор ввода/вывода> |                       +
                 <составной оператор> | <спец. оператор> | <описание>                   +
    <оператор выражения>::= <выражение>;                                                +
    <выражение> ::= <выражение4> | <имя> = <выражение>                                  +
    <выражение4>:: <выражение3> { || <выражение3> }                                     +
    <выражение3>:: <выражение2> { && <выражение2> }                                     +
    <выражение2>:: <выражение1> {<операция отношения 1> <выражение1> }                  +
    <выражение1>::= <простое выражение> { <операция отношения><выражение1>              +
    <операция отношения>::= > | <                                                       +
    <операция отношения1>::= != | <= | >= | ==                                          +
    <простое выражение>::= <терм>{<операция сложения><терм>}                            +
    <терм>::= <атом1>{<операция умножения><атом1>}                                      +
    <атом1>::=<атом> | <атом><возведение в степень><атом>|<атом><инкремент>             +
    <атом>::= <имя> | (<выражение>) | <спец. атом>                                      +
    <спец. атом>::= <конст> | <логич. значение> | ! <атом>                              ++
    <операция сложения>::= + | -                                                        +
    <операция умножения>::= * | / | %                                                   +
    <возведение в степень>::= ^                                                         +
    <операция присваивания> ::= =                                                       +
    <инкремент>::= ++ |--                                                               +
    <знак>::= <пусто> | + | –                                                           +
    <конст>::= <знак> <цифра> {<цифра>} | <цифра> {<цифра>}.<цифра> {<цифра>}           ++
    <логич. значение>::= true | false                                                   +
    <оператор ввода-вывода>::= cinout <список элементов>;                               +
    <список элементов>::= <элемент>{<элемент>}                                          +
    <элемент>::= << <выражение> | << endl | << “строка”  | >> <имя>                     +
    <составной оператор>::= { <оператор>{<оператор>} }                                  +
    <описание>::= <тип> <секция>{,<секция>};                                            +
    <секция>::= <имя> | <имя> = <выражение>                                             +
    <тип>::= int | bool | double                                                        +
    <спец. оператор>::= <оператор for> | <оператор dowhile> | 
                        <оператор if> | <оператор while>                          
    <оператор dowhile>::= do <оператор> while (<выражение>);                            +
    <оператор for> ::= for ([<выражение>]; [<выражение>]; [<выражение>]) <оператор>     +
    <оператор if> ::= if ( <выражение> ) <оператор> | 
                      if ( <выражение> ) <оператор> else <оператор>
    <оператор while> ::= while ( <выражение> ) <оператор>

## Comments
    
    1) Переменная, описанная в теле составного оператора, не должна быть видима после тела этого оператора, имя переменной можно будет повторно             объявлять дальше в этой программе.
    2) Операция присваивания изменяет значение переменной при вычислении выражения, при этом возвращает результат присваивания. 
       Например, cinout << a = b = 1+1 присвоит a и b значение 2 и выведет 2.
    3) Расширенное понятие истенности: `true` - все что не `НОЛЬ`

#### Таблица приоритетов операций

| № | Операция                 | Описание              |
|:-:|:-------------------------|:----------------------|
| 1 | `!`                      | логическое отрицание  |
|   | `+`                      | унарный плюс          |
|   | `-`                      | унарный минус         |
|   | `^`                      | возведение в степень  |
| 2 | `*`                      | умножение             |
|   | `/`                      | деление               |
|   | `%`                      | взятие остатка        |
| 3 | `+`                      | сложение              |
|   | `-`                      | вычитание             |
| 4 | `==`                     | равно                 |
|   | `!=`                     | не равно              |
| 5 | `&&`                     | логическое умножение  |
| 6 | `\\`                     | логическое сложение   |
| 7 | `=`                      | присваивание          |
| 8 | `++`                     | посфиксный инкремент  |
|   | `--`                     | постфиксный декремент | 

    
#### Правило вычисления логических выражений

«Ленивые» вычисления логических выражений (слева направо; до тех пор, пока не станет известно значение выражения). 

#### Таблица типов операндов операций

| Операция                 | тип X    | тип Y    | тип результата |
|:-------------------------|:--------:|:--------:|:--------------:|
| `+` `−` `*` `/` `%`      | `int`    | `int`    | `int`          |
| `+`  `−`  `*`  `/`       | `double` | `double` | `double`       |
| `+` `−` `*` `/`          | `double` | `int`    | `double`       |
| `+` `−` `*` `/`          | `int`    | `double` | `double`       |
| унарные `+` `−`          | `int`    | –        | `int`          |
| унарные `+` `−`          | `double` | –        | `double`       |
| `<>` `<=` `>=` `==` `!=` | `int`    | `int`    | `bool`         | 
| `<>` `<=` `>=` `==` `!=` | `double` | `double` | `bool`         | 
| `<>` `<=` `>=` `==` `!=` | `double` | `int`    | `bool`         | 
| `<>` `<=` `>=` `==` `!=` | `int`    | `double` | `bool`         |
|`&&` `\\`                 | `bool`   | `bool`   | `bool`         |
|`!`                       | `bool`   | –        | `bool`         |
|`=`                       | `int`    | `int`    | `int`          |
|`=`                       | `double` | `double` | `double`       | 
|`=`                       | `int`    | `double` | `int`          |
|`=`                       | `double` | `int`    | `double`       | 
|`=`                       | `bool`   | `bool`   | `bool`         |
