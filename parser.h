#ifndef PARSER_H_
#define PARSER_H_

#include "cell.h"
#include "string_view.h"
#include <stdbool.h> // Для типа bool

// Разбирает содержимое ячейки (String_View) и заполняет структуру Cell.
// Возвращает true в случае успеха, false в случае ошибки.
bool parse_cell_content(String_View sv, Cell *cell);

#endif // PARSER_H_
