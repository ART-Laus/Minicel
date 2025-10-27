#include <stdlib.h> // Для strtod
#include <string.h> // Для memcpy
#include "parser.h"

// Вспомогательная функция для разбора String_View как числа
static bool parse_number(String_View sv, double *number) {
    // Преобразуем String_View в null-terminated строку для strtod
    char *str = (char *)malloc(sv.count + 1);
    if (str == NULL) return false;
    memcpy(str, sv.data, sv.count);
    str[sv.count] = '\0';

    char *endptr;
    *number = strtod(str, &endptr);

    // Проверяем, что вся строка была разобрана как число
    bool success = (*endptr == '\0' && endptr != str);
    free(str);
    return success;
}

bool parse_cell_content(String_View sv, Cell *cell) {
    // Проверяем, является ли это выражением (начинается с '=')
    if (sv.count > 0 && sv.data[0] == '=') {
        // Пока что просто помечаем как выражение и сохраняем сырую строку (без '=')
        cell->type = CELL_TYPE_EXPR;
        // TODO: Здесь будет фактический парсинг выражения
        sv_chop_left(&sv, 1); // Удаляем '='
        cell->as.text = sv; // Сохраняем оставшуюся часть как текст для отладки
        cell->as.expr = NULL; // Заглушка
        return true;
    }

    // Пытаемся разобрать как число
    double number_val;
    if (parse_number(sv, &number_val)) {
        cell->type = CELL_TYPE_NUMBER;
        cell->as.number = number_val;
        return true;
    }

    // В противном случае, это текст
    cell->type = CELL_TYPE_TEXT;
    cell->as.text = sv;
    return true;
}
