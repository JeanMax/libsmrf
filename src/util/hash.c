#include "util/hash.h"
#include "util/malloc.h"

#define DEFAULT_BUCKET_SIZE  241
#define BUCKET_RESIZE_TRIGGER 0.75
#define FREE_NODE_IS_OK


static void default_node_deleter(size_t key, void *value)
{
    (void)key;
#ifdef FREE_NODE_IS_OK
    FREE(value);
#else
    value = NULL;
#endif
}

Htable          *hnew(size_t bucket_size, del_node_fun *del)
{
    Htable  *new;

    if (!bucket_size) {
        bucket_size = DEFAULT_BUCKET_SIZE;
    }
    ZALLOC(new, sizeof(Htable));
    ZALLOC(new->bucket, sizeof(Hnode *) * (bucket_size + 1));
    new->bucket_size = bucket_size;
    new->del = del ? del : default_node_deleter;
    return new;
}

////////////////////////////////////////////////////////////////////////////////

static void     move_nodes(Hnode **bucket, Hnode *node, size_t bucket_size)
{
    Hnode   **dst;
    Hnode   *next;

    while (node) {
        next = node->next;
        dst = bucket + node->key % bucket_size;
        node->next = *dst;
        *dst = node;
        node = next;
    }
}

static size_t   get_new_size(size_t old_size)
{
    static const size_t primes[] = {
        31, 61, 121, 241, 487, 977, 1973, 3947, 7901,
        15803, 31607, 63241, 126481, 252971, 505949,
        1011917, 2023837, 4047697, 8095393, 16190789,
        32381627, 64763263, 129526531, 259053083,
        518106181, 1036212413, 2072424839,
        0
    };

    for (size_t i = 0; primes[i]; i++) {
        if (primes[i] > old_size) {
            return primes[i];
        }
    }
    return old_size;
}

static void     resize_table(Htable *table)
{
    Hnode   **new_bucket;
    Hnode   **bucket;
    size_t  size = table->bucket_size;

    table->bucket_size = get_new_size(size);
    LOG_WARNING("hset: resizing table (new size = %d)", table->bucket_size);
    if (size == table->bucket_size) {
        LOG_WARNING("hset: max table size reached, can't resize");
        return;
    }
    ZALLOC(new_bucket, sizeof(Hnode *) * table->bucket_size);
    bucket = table->bucket;
    while (size--) {
        if (*bucket) {
            move_nodes(new_bucket, *bucket, table->bucket_size);
        }
        bucket++;
    }
    FREE(table->bucket);
    table->bucket = new_bucket;
}

static bool     add_node(Htable *table, size_t key, void *value)
{
    Hnode **bucket_spot = table->bucket + key % table->bucket_size;
    Hnode  *node = *bucket_spot;

    while (node) {
        if (key == node->key) {
            LOG_WARNING("hset: key [%d] already exists in Htable (overwrite)",
                        key);
            table->del(node->key, node->value);
            node->value = value;
            return FALSE;
        }
        node = node->next;
    }

    ZALLOC(node, sizeof(Hnode));
    node->key = key;
    node->value = value;
    node->next = *bucket_spot;
    *bucket_spot = node;
    return TRUE;
}

void            hset(Htable *table, size_t key, void *value)
{
    if (add_node(table, key, value)) {
        table->length++;
    }
    if (table->length > (size_t)((double)table->bucket_size * BUCKET_RESIZE_TRIGGER)) {
        resize_table(table);
    }
}

////////////////////////////////////////////////////////////////////////////////

void           *hget(Htable *table, size_t key)
{
    Hnode *node = *(table->bucket + key % table->bucket_size);

    while (node) {
        if (key == node->key) {
            return node->value;
        }
        node = node->next;
    }
    return NULL;
}

////////////////////////////////////////////////////////////////////////////////

void           *hiter(Htable *table, iter_callback f, void *callback_data)
{
    Hnode **bucket = table->bucket;
    size_t size = table->bucket_size;
    Hnode *node;
    Hnode *next;

    while (size--) {
        node = *bucket;
        while (node) {
            next = node->next;  // in case f calls hdel on node :|
            if (f(node->value, callback_data)) {
                return node->value;
            }
            node = next;
        }
        bucket++;
    }
    return NULL;
}

////////////////////////////////////////////////////////////////////////////////

static inline void free_node(Htable *table, Hnode *node)
{
    table->del(node->key, node->value);
    FREE(node);
    table->length--;
}

void            hdel(Htable *table, size_t key)
{
    Hnode **first = table->bucket + key % table->bucket_size;
    Hnode *node = *first;
    Hnode *prev = NULL;

    while (node) {
        if (key == node->key) {
            if (prev) {
                prev->next = node->next;
            } else  {
                *first = NULL;
            }
            free_node(table, node);
            return;
        }
        prev = node;
        node = node->next;
    }
}

////////////////////////////////////////////////////////////////////////////////

static inline void free_list(Htable *table, Hnode **node_addr)
{
    Hnode *node = *node_addr;
    Hnode *next;

    while (node) {
        next = node->next;
        free_node(table, node);
        node = next;
    }
    *node_addr = NULL;
}

void            hdelall(Htable *table)
{
    Hnode **bucket = table->bucket;
    size_t  size = table->bucket_size;

    while (size--) {
        if (*bucket) {
            free_list(table, bucket);
        }
        bucket++;
    }
}

////////////////////////////////////////////////////////////////////////////////

void            hfree(Htable **table_addr)
{
    if (*table_addr) {
        hdelall(*table_addr);
        FREE((*table_addr)->bucket);
        FREE(*table_addr);
    }
}
