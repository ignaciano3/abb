#include "abb.h"
#include "pila.h"
#include <string.h>

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
    nodo->izq = NULL;
    nodo->der = NULL;
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

nodo_t ** buscar_nodo(const abb_t* arbol, const char *clave){
    // Suponiendo que no se busca la raiz
    nodo_t *nodo = arbol->raiz;

    while (nodo != NULL) {
        if (arbol->cmp(nodo->clave, clave) < 0) { // nodo->clave < clave
            if (nodo->der == NULL || arbol->cmp(nodo->der->clave, clave) == 0){
                // nodo->der->clave == clave o nodo no existe
                return &nodo->der;
            }
            nodo = nodo->der;

        } else if (arbol->cmp(nodo->clave, clave) > 0){ // nodo->clave > clave o nodo
            if (nodo->izq == NULL || arbol->cmp(nodo->izq->clave, clave) == 0){
                // nodo->izq->clave == clave o nodo no existe
                return &nodo->izq;
            }
            nodo = nodo->izq;
        }
    }
    return NULL;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato) {

    // caso arbol vacio
    if (arbol->raiz == NULL){
        arbol->raiz = crear_nodo(clave, dato);
        arbol->cantidad++;
        return true;
    }

    nodo_t **puntero_a_ubicacion;
    if (arbol->cmp(clave, arbol->raiz->clave) == 0){
        puntero_a_ubicacion = &arbol->raiz;
    } else {
        puntero_a_ubicacion = buscar_nodo(arbol, clave);
    }

    nodo_t *nodo = *puntero_a_ubicacion;
    if (nodo == NULL){
        *puntero_a_ubicacion = crear_nodo(clave, dato);
        arbol->cantidad++;
        return true;
    }

    if (arbol->cmp(nodo->clave, clave) == 0){
        if (arbol->destruir_dato){
            arbol->destruir_dato(nodo->dato);
        }
        nodo->dato = dato;
        return true;
    }

    return false;
}

bool abb_pertenece(const abb_t *arbol, const char *clave) {

    if (arbol->raiz == NULL) return false;
    if (arbol->cmp(arbol->raiz->clave, clave) == 0) return true;

    nodo_t *nodo = *buscar_nodo(arbol, clave);
    return (nodo != NULL) ? true : false;
}

void *abb_obtener(const abb_t *arbol, const char *clave) {

    if (arbol->raiz == NULL) return NULL;
    if (arbol->cmp(arbol->raiz->clave, clave) == 0) return arbol->raiz->dato;

    nodo_t *nodo = *buscar_nodo(arbol, clave);
    return nodo->dato;
}

void *abb_borrar(abb_t *arbol, const char *clave) {

    nodo_t **pNodo;
    if (arbol->cmp(arbol->raiz->clave, clave) == 0){
        pNodo = &arbol->raiz;
    } else {
        pNodo = buscar_nodo(arbol, clave);
    }

    if (*pNodo == NULL) return NULL;

    nodo_t *nodo = *pNodo;
    void* dato = nodo->dato;

    arbol->cantidad--;
    if (nodo->izq !=NULL && nodo->der == NULL){
        *pNodo = nodo->izq;
    } else if (nodo->der != NULL && nodo->izq == NULL){
        *pNodo = nodo->der;
    } else if (nodo->izq != NULL && nodo->der != NULL){
        // cuando tengo hijo izq y der
        nodo_t *reemplazante = nodo->izq;
        while (reemplazante->der != NULL)
            reemplazante = reemplazante->der;
        char *clave_reemplazante = strdup(reemplazante->clave);
        void *dato_reemplazante = abb_borrar(arbol, reemplazante->clave);
        nodo->clave = clave_reemplazante;
        nodo->dato = dato_reemplazante;
        return nodo->dato;
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
        void * dato = abb_borrar(arbol, arbol->raiz->clave);
        if (arbol->destruir_dato)
            arbol->destruir_dato(dato);
    }
    free(arbol);
}
