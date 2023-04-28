#include <containers/eq.h>
#include <containers/hash.h>

#include "parser.h"

bool eq_sl_variable_type_name(SLVariableTypeName *a, SLVariableTypeName *b) {
    return a->type == b->type && containers_eq_dstr(&a->name, &b->name);
}

size_t hash_sl_variable_type_name(SLVariableTypeName *a) {
    return a->type ^ containers_hash_dstr(&a->name);
}
