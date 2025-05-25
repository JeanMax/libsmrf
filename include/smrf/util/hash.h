#ifndef _HASH_H
#define _HASH_H

#include "types.h"

typedef struct s_htable Htable;
typedef struct s_hnode Hnode;

typedef void del_node_fun(size_t key, void *value);

struct s_htable
{
    size_t length;
    Hnode **bucket;
    size_t bucket_size;
    del_node_fun *del;
};

struct s_hnode
{
    size_t key;
    Hnode *next;
    void *value;
};


/*
** create (with malloc(3)) a new hash table
** if the given bucket_size is null, an arbitrary size will be choosen
** the default table setting aims to store pointers
** if the given del_node_fun is null, free(3) will be used on node's .value
*/
Htable *hnew(size_t bucket_size, del_node_fun *del);

/*
** add a new key/value pair in the hash table
** if the key already exists, the value will be overwritten
** the table will automagically resize itself if needed
*/
void hset(Htable *table, size_t key, void *value);

/*
** return the value corresponding to 'key' in the hash table
*/
void *hget(Htable *table, size_t key);

/*
** apply f to each node
** if f returns TRUE, stop iterating and return the current node .value
** otherwise returns NULL
*/
typedef bool iter_callback(void *node_value, void *data);
void *hiter(Htable *table, iter_callback f, void *callback_data);

/*
** delete the node corresponding to 'key' from the table
*/
void hdel(Htable *table, size_t key);

/*
** delete all the nodes
*/
void hdelall(Htable *table);

/*
** free the hash table itself and all its nodes
*/
void hfree(Htable **table_addr);



#endif
