/* vim: set tabstop=4 expandtab shiftwidth=4 softtabstop=4: */

/*
 * Copyright 2015 University of Piemonte Orientale, Computer Science Institute
 *
 * This file is part of UPOalglib.
 *
 * UPOalglib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * UPOalglib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with UPOalglib.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "bst_private.h"
#include <stdio.h>
#include <stdlib.h>


/**** EXERCISE #1 - BEGIN of FUNDAMENTAL OPERATIONS ****/


upo_bst_t upo_bst_create(upo_bst_comparator_t key_cmp)
{
    upo_bst_t tree = malloc(sizeof(struct upo_bst_s));
    if (tree == NULL)
    {
        perror("Unable to create a binary search tree");
        abort();
    }

    tree->root = NULL;
    tree->key_cmp = key_cmp;

    return tree;
}

void upo_bst_destroy(upo_bst_t tree, int destroy_data)
{
    if (tree != NULL)
    {
        upo_bst_clear(tree, destroy_data);
        free(tree);
    }
}

void upo_bst_clear_impl(upo_bst_node_t *node, int destroy_data)
{
    if (node != NULL)
    {
        upo_bst_clear_impl(node->left, destroy_data);
        upo_bst_clear_impl(node->right, destroy_data);

        if (destroy_data)
        {
            free(node->key);
            free(node->value);
        }

        free(node);
    }
}

void upo_bst_clear(upo_bst_t tree, int destroy_data)
{
    if (tree != NULL)
    {
        upo_bst_clear_impl(tree->root, destroy_data);
        tree->root = NULL;
    }
}

void* upo_bst_put(upo_bst_t tree, void *key, void *value)
{
    void *vold=NULL;
    tree->root=upo_bst_put_impl(tree->root,key,value,vold);
    return vold;
}

upo_bst_node_t *upo_bst_put_impl(upo_bst_node_t *n, void *k, void *v, void *vo)
{
    vo=NULL;
    if (n==NULL)
        return upo_node_create(k,v);
    else if (*(int *)k < (*(int *)(n->key)))
        n->left=upo_bst_put_impl(n->left,k,v,vo);
    else if (*(int *)k > (*(int *)(n->key)))
        n->right=upo_bst_put_impl(n->right,k,v,vo);
    else
    {
        vo=n->value;
        n->value=v;
    }
    return n;
}
void upo_bst_insert(upo_bst_t tree, void *key, void *value)
{
    tree->root=upo_bst_insert_impl(tree->root,key,value);
}

upo_bst_node_t *upo_bst_insert_impl(upo_bst_node_t *n, void *k, void *v)
{
    if (n==NULL)
        return upo_node_create(k,v);
    else if (*(int *)k < (*(int *)(n->key)))
        n->left=upo_bst_insert_impl(n->left,k,v);
    else if (*(int *)k > (*(int *)(n->key)))
        n->right=upo_bst_insert_impl(n->right,k,v);

    return n;
}

void* upo_bst_get(const upo_bst_t tree, const void *key)
{
    upo_node_t *n=upo_bst_get_impl(treee->root,key);
    if (n!=NULL)
        return n->value;
    else
        return NULL;
}

upo_node_t *upo_bst_get_impl(upo_bst_node_t *n, void *k)
{
    if (n==NULL)
        return NULL;
    else if (*(int *)k < (*(int *)(n->key)))
        return upo_bst_get_impl(n->left,k);
    else if (*(int *)k > (*(int *)(n->key)))
       return upo_bst_get_impl(n->right,k);
    else
        return n;
}

int upo_bst_contains(const upo_bst_t tree, const void *key)
{
    if (upo_bst_get_impl(tree->root,key)!=NULL)
        return 1;
    else
        return 0;
}

void upo_bst_delete(upo_bst_t tree, const void *key, int destroy_data)
{
    tree->root=upo_bst_delete_impl(tree->root,key);

}

size_t upo_bst_size(const upo_bst_t tree)
{
    return upo_bst_size_impl(tree->root);
}

size_t upo_bst_size_impl(upo_node_t *n)
{
    if (n==NULL)
        return (size_t) 0;
    return (size_t) 1 + upo_bst_size_impl(n->left) + upo_bst_size_impl(n->right);
}

size_t upo_bst_height(const upo_bst_t tree)
{
   return upo_bst_height_impl(tree->root);
}

int upo_is_leaf(upo_node_t *n)
{
    if (n->left==NULL && n->right==NULL)
        return 1;
    else
        return 0;
}

size_t upo_bst_height_impl(upo_node_t *n)
{
    if (n==NULL || upo_is_leaf(n))
        return 0;
    return 1+upo_bst_max(upo_bst_height_impl(n->left));
    upo_bst_height_impl(n->right);
}

void upo_bst_traverse_in_order(const upo_bst_t tree, upo_bst_visitor_t visit, void *visit_context)
{
    upo_bst_traverse_in_order_impl(tree->root,visit,visit_context);
}

void upo_bst_traverse_in_order_impl(upo_node_t *n, upo_bst_visitor_t visit, void *visit_context)
{
    if (n!=NULL)
    {
        upo_bst_traverse_in_order_impl(n->left,visit,visit_context);
        visit(n->key,n->value,visit_context);
        upo_bst_traverse_in_order_impl(n->right,visit,visit_context);
    }
}

int upo_bst_is_empty(const upo_bst_t tree)
{
    if (t==NULL || tree->root==NULL)
        return 1;
    else
        return 0;
}


/**** EXERCISE #1 - END of FUNDAMENTAL OPERATIONS ****/


/**** EXERCISE #2 - BEGIN of EXTRA OPERATIONS ****/


void* upo_bst_min(const upo_bst_t tree)
{
    upo_node_t *n=upo_bst_min_impl(treee->root);
    if (n!=NULL)
        return n->value;
    else
        return NULL;
}

upo_node_t *upo_bst_min_impl(upo_bst_node_t *n)
{
    int min;
    upo_bst_node_t *minL,*minR;
    if (n!=NULL)
    {
        minL=upo_bst_min_impl(n->left);
        minR=upo_bst_min_impl(n->right);
        min=n->value;
        if (*(int *)(minL->value)<min)
            n=minL;
        if (*(int *)(minR->value)<min && *(int *)(minR->value)<*(int *)(minL->value))
            n=minR;
        return n;
    }
    else
        return NULL;
}

void* upo_bst_max(const upo_bst_t tree)
{
    upo_node_t *n=upo_bst_max_impl(treee->root);
    if (n!=NULL)
        return n->value;
    else
        return NULL;
}

upo_node_t *upo_bst_max_impl(upo_bst_node_t *n)
{
    int max;
    upo_bst_node_t *maxL,*maxR;
    if (n!=NULL)
    {
        maxL=upo_bst_max_impl(n->left);
        maxR=upo_bst_max_impl(n->right);
        max=n->value;
        if (*(int *)(maxL->value)>max)
            n=maxL;
        if (*(int *)(maxR->value)>max && *(int *)(maxR->value)>*(int *)(maxL->value))
            n=maxR;
        return n;
    }
    else
        return NULL;
}

void upo_bst_delete_min(upo_bst_t tree, int destroy_data)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    fprintf(stderr, "To be implemented!\n");
    abort();
}

void upo_bst_delete_max(upo_bst_t tree, int destroy_data)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    fprintf(stderr, "To be implemented!\n");
    abort();
}

void* upo_bst_floor(const upo_bst_t tree, const void *key)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    fprintf(stderr, "To be implemented!\n");
    abort();
}

void* upo_bst_ceiling(const upo_bst_t tree, const void *key)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    fprintf(stderr, "To be implemented!\n");
    abort();
}

upo_bst_key_list_t upo_bst_keys_range(const upo_bst_t tree, const void *low_key, const void *high_key)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    fprintf(stderr, "To be implemented!\n");
    abort();
}

upo_bst_key_list_t upo_bst_keys(const upo_bst_t tree)
{
    if (tree->root==NULL)
}

int upo_bst_is_bst(const upo_bst_t tree, const void *min_key, const void *max_key)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    fprintf(stderr, "To be implemented!\n");
    abort();
}


/**** EXERCISE #2 - END of EXTRA OPERATIONS ****/


upo_bst_comparator_t upo_bst_get_comparator(const upo_bst_t tree)
{
    if (tree == NULL)
    {
        return NULL;
    }

    return tree->key_cmp;
}
