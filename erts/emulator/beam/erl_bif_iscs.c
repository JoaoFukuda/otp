#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "bif.h"
#include "sys.h"
#include "big.h"

#include <malloc.h>
#include <memory.h>
#include <stdio.h>

#define HASHMAP_MAX_SIZE 1 << 22
#define HASHMAP_VAR_TYPE Eterm

typedef struct aux {
		unsigned int key;
		HASHMAP_VAR_TYPE value;
		struct aux* next;
} fuk_hashmap_node;

fuk_hashmap_node* fuk_hashmap_node_new(unsigned int key, HASHMAP_VAR_TYPE value)
{
	fuk_hashmap_node* node = (fuk_hashmap_node*)malloc(sizeof(fuk_hashmap_node));
	node->key = key;
	node->value = value;
	node->next = NULL;
	return node;
}

void fuk_hashmap_node_delete(fuk_hashmap_node* node)
{
	free(node);
}

typedef struct {
		fuk_hashmap_node* values[HASHMAP_MAX_SIZE];
} fuk_hashmap;

static fuk_hashmap list_streets;

void fuk_hashmap_insert(unsigned int key,
                        HASHMAP_VAR_TYPE value)
{
	int real_key = key % HASHMAP_MAX_SIZE;
	fuk_hashmap_node* node = fuk_hashmap_node_new(key, value);
	node->next = list_streets.values[real_key];
	list_streets.values[real_key] = node;
}

fuk_hashmap_node* fuk_hashmap_find(unsigned int key)
{
	int real_key = key % HASHMAP_MAX_SIZE;
	fuk_hashmap_node* node = list_streets.values[real_key];
	while (node && node->key != key) {
		node = node->next;
	}
	return node;
}

char* alphabool(int value)
{
	return value ? "\x1b[1;92mtrue\x1b[m" : "\x1b[91mfalse\x1b[m";
}

void check_type(Eterm x)
{
	erts_printf("is_atom: %s\n", alphabool(is_atom(x)));
	erts_printf("is_big: %s\n", alphabool(is_big(x)));
	erts_printf("is_binary: %s\n", alphabool(is_binary(x)));
	erts_printf("is_boxed: %s\n", alphabool(is_boxed(x)));
	erts_printf("is_byte: %s\n", alphabool(is_byte(x)));
	erts_printf("is_catch: %s\n", alphabool(is_catch(x)));
	erts_printf("is_export: %s\n", alphabool(is_export(x)));
	erts_printf("is_external: %s\n", alphabool(is_external(x)));
	erts_printf("is_float: %s\n", alphabool(is_float(x)));
	erts_printf("is_fun: %s\n", alphabool(is_fun(x)));
	erts_printf("is_header: %s\n", alphabool(is_header(x)));
	erts_printf("is_immed: %s\n", alphabool(is_immed(x)));
	erts_printf("is_integer: %s\n", alphabool(is_integer(x)));
	erts_printf("is_internal: %s\n", alphabool(is_internal(x)));
	erts_printf("is_list: %s\n", alphabool(is_list(x)));
	erts_printf("is_map: %s\n", alphabool(is_map(x)));
	erts_printf("is_nil: %s\n", alphabool(is_nil(x)));
	erts_printf("is_number: %s\n", alphabool(is_number(x)));
	erts_printf("is_pid: %s\n", alphabool(is_pid(x)));
	erts_printf("is_port: %s\n", alphabool(is_port(x)));
	erts_printf("is_ref: %s\n", alphabool(is_ref(x)));
	erts_printf("is_small: %s\n", alphabool(is_small(x)));
	erts_printf("is_thing: %s\n", alphabool(is_thing(x)));
	erts_printf("is_tuple: %s\n", alphabool(is_tuple(x)));
	erts_printf("is_value: %s\n", alphabool(is_value(x)));
}

Eterm uint_to_Eterm(unsigned int i)
{
	return i << 4;
}

BIF_RETTYPE iscs_checktype_1(BIF_ALIST_1)
{
	check_type(BIF_ARG_1);
	BIF_RET(am_ok);
}

BIF_RETTYPE iscs_insert_1(BIF_ALIST_1)
{
	Eterm* tp = tuple_val(BIF_ARG_1);
	unsigned long int key = atom_val(CDR(tp));
	fuk_hashmap_insert(key, BIF_ARG_1);
	return am_ok;
}

BIF_RETTYPE iscs_lookup_1(BIF_ALIST_1)
{
	unsigned long int key = atom_val(BIF_ARG_1);
	BIF_RET(fuk_hashmap_find(key)->value);
}

BIF_RETTYPE iscs_update_2(BIF_ALIST_2)
{
	unsigned long int key = atom_val(BIF_ARG_1);
	Eterm* info = tuple_val(BIF_ARG_2);
	unsigned int position = unsigned_val(info[1]);

	fuk_hashmap_node* result = fuk_hashmap_find(key);

	tuple_val(result->value)[position] = info[2];

	BIF_RET(am_ok);
}

BIF_RETTYPE iscs_update_counter_2(BIF_ALIST_2)
{

	unsigned long int key = atom_val(BIF_ARG_1);
	Eterm* info = tuple_val(BIF_ARG_2);
	unsigned int position = unsigned_val(info[1]);

	fuk_hashmap_node* result = fuk_hashmap_find(key);

	tuple_val(result->value)[position] = make_small(signed_val(tuple_val(result->value)[position]) + signed_val(info[2]));

	BIF_RET(am_ok);
}
