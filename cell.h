#ifndef CELL_H_
#define CELL_H_

#include "string_view.h"

// Forward declaration for circular dependency
typedef struct Expr Expr;

// -- Cell --

typedef enum {
    CELL_TYPE_TEXT,
    CELL_TYPE_NUMBER,
    CELL_TYPE_EXPR,
} Cell_Type;

typedef struct {
    Cell_Type type;
    union {
        String_View text;
        double number;
        Expr *expr;
    } as;
} Cell;

// -- Expression --

typedef enum {
    EXPR_TYPE_LITERAL_NUMBER,
    EXPR_TYPE_CELL_REF,
    EXPR_TYPE_BINARY_OP,
} Expr_Type;

typedef struct {
    size_t row;
    size_t col;
} Cell_Ref;

typedef enum {
    OP_PLUS,
    OP_MINUS,
    OP_MUL,
    OP_DIV,
} Op_Type;

typedef struct {
    Op_Type type;
    Expr *left;
    Expr *right;
} Bin_Op;

struct Expr {
    Expr_Type type;
    union {
        double literal_number;
        Cell_Ref cell_ref;
        Bin_Op binary_op;
    } as;
};

#endif // CELL_H_
