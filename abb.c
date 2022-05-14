#include "abb.h"
#include "pila.h"
#include <string.h>
#include "stdio.h"

typedef struct nodo nodo_t;

struct nodo {
    nodo_t *izq, *der;
    void *dato;
    char *clave;
};

nodo_t *crear_nodo(const char *clave, void *dato){
    nodo_t *nodo = malloc(sizeof (nodo_t));
    if (nodo == NULL) return NULL;
    nodo->clave = strdup(clave);
    nodo->dato = dato;
    nodo->izq = nodo->der = NULL;
    return nodo;
}

struct abb {
    nodo_t *raiz;
    size_t cantidad;
    abb_comparar_clave_t cmp;
    abb_destruir_dato_t destruir_dato;
};

abb_t *abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato) {
    abb_t *abb = malloc(sizeof (abb_t));
    if (abb == NULL) return NULL;
    abb->raiz = NULL;
    abb->cantidad = 0;
    abb->cmp = cmp;
    abb->destruir_dato = destruir_dato;
    return abb;
}

size_t abb_cantidad(const abb_t *arbol) {
    return arbol->cantidad;
}

nodo_t** buscar_nodo(nodo_t **puntero_nodo, const char * clave, abb_comparar_clave_t cmp){
    if (*puntero_nodo == NULL){
        return puntero_nodo;
    }

    if (cmp(clave, (*puntero_nodo)->clave) < 0){
        puntero_nodo = buscar_nodo(&(*puntero_nodo)->izq, clave, cmp);
    } else if (cmp(clave, (*puntero_nodo)->clave) > 0){
        puntero_nodo = buscar_nodo(&(*puntero_nodo)->der, clave, cmp);
    }

    return puntero_nodo;
}


bool abb_guardar(abb_t *arbol, const char *clave, void *dato) {
    nodo_t** puntero_nodo = buscar_nodo(&arbol->raiz, clave, arbol->cmp);
    if (*puntero_nodo == NULL){
        *puntero_nodo = crear_nodo(clave, dato);
        if (*puntero_nodo == NULL) return false;
        arbol->cantidad++;
        return true;
    } else {
        if (arbol->destruir_dato)
            arbol->destruir_dato((*puntero_nodo)->dato);
        (*puntero_nodo)->dato = dato;
        return true;
    }
}

bool abb_pertenece(const abb_t *arbol, const char *clave) {
    nodo_t *nodo = arbol->raiz;
    nodo_t** puntero_nodo = buscar_nodo(&nodo, clave, arbol->cmp);
    return (*puntero_nodo != NULL) ? true : false;
}

void *abb_obtener(const abb_t *arbol, const char *clave) {
    nodo_t *nodo = arbol->raiz;
    nodo_t** puntero_nodo = buscar_nodo(&nodo, clave, arbol->cmp);
    return (*puntero_nodo)->dato;
}

void *abb_borrar(abb_t *arbol, const char *clave) {
    nodo_t** puntero_nodo = buscar_nodo(&arbol->raiz, clave, arbol->cmp);
    if (*puntero_nodo == NULL) return NULL;

    nodo_t *nodo = *puntero_nodo;
    void* dato = nodo->dato;
    arbol->cantidad--;

    if (nodo->izq !=NULL && nodo->der == NULL){
        *puntero_nodo = nodo->izq;
    } else if (nodo->der != NULL && nodo->izq == NULL){
        *puntero_nodo = nodo->der;
    } else if (nodo->izq != NULL && nodo->der != NULL){
        // cuando tengo hijo izq y der
        exit(1);
    }

    free(nodo->clave);
    free(nodo);
    return dato;
}

#include "stdio.h"
void inorder(nodo_t *nodo, bool visitar(const char *, void *, void *), void *extra){
    if (nodo == NULL) return;
    inorder(nodo->izq, visitar, extra);
    visitar(nodo->clave, nodo->dato, extra);
    inorder(nodo->der, visitar, extra);
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
    if (arbol->raiz == NULL) return;
    inorder(arbol->raiz, visitar, extra);
}

void abb_destruir(abb_t *arbol) {
    while (arbol->cantidad > 0){
        void *dato = abb_borrar(arbol, arbol->raiz->clave);
        if (arbol->destruir_dato)
            arbol->destruir_dato(dato);
    }
    free(arbol);
}
