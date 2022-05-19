#include "abb.h"
#include "pila.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define _POSIX_C_SOURCE 200809L

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
    } else if (cmp(clave, (*puntero_nodo)->clave) < 0){
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
    return *puntero_nodo != NULL;
}

void *abb_obtener(const abb_t *arbol, const char *clave) {
    nodo_t *nodo = arbol->raiz;
    nodo_t** puntero_nodo = buscar_nodo(&nodo, clave, arbol->cmp);
    return (*puntero_nodo) ? (*puntero_nodo)->dato : NULL;
}


void *abb_borrar(abb_t *arbol, const char *clave) {
    nodo_t** puntero_nodo = buscar_nodo(&arbol->raiz, clave, arbol->cmp);
    if (*puntero_nodo == NULL) return NULL;

    void* dato = (*puntero_nodo)->dato;
    nodo_t *nodo = *puntero_nodo;

    if (nodo->izq == NULL && nodo->der == NULL){
        *puntero_nodo = NULL;
    } else if (nodo->izq !=NULL && nodo->der == NULL){
        *puntero_nodo = nodo->izq;
    } else if (nodo->der != NULL && nodo->izq == NULL){
        *puntero_nodo = nodo->der;
    } else if (nodo->izq != NULL && nodo->der != NULL){

        nodo_t **nodoMaxIzq = buscar_nodo(&arbol->raiz, (*puntero_nodo)->izq->clave, arbol->cmp);
        nodo_t* izq_max = *nodoMaxIzq;

        while (izq_max->der != NULL){
            izq_max = izq_max->der;
        }

        char* nodo_izq_max_clave = strdup(izq_max->clave);
        void* nodo_izq_max_dato = abb_borrar(arbol, nodo_izq_max_clave);

        free(nodo->clave);
        (*puntero_nodo)->clave = nodo_izq_max_clave;
        (*puntero_nodo)->dato = nodo_izq_max_dato;

        return dato;
    }
    free(nodo->clave);
    free(nodo);
    arbol->cantidad--;
    return dato;
}

bool inorder(nodo_t *nodo, bool f(const char *, void *, void *), void *extra){
    if (nodo == NULL) return true;
    bool seguir = inorder(nodo->izq, f, extra);
    if (seguir == false || !f(nodo->clave, nodo->dato, extra)) return false;
    seguir &= inorder(nodo->der, f, extra);
    return seguir;
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
    inorder(arbol->raiz, visitar, extra);
}

void abb_destruir(abb_t *arbol) {
    while (arbol->raiz != NULL){
        void *dato = abb_borrar(arbol, arbol->raiz->clave);
        if (arbol->destruir_dato)
            arbol->destruir_dato(dato);
    }
    free(arbol);
}

//---------------ARBOL ITER------------------------//

struct abb_iter {
    pila_t *pila;
};

abb_iter_t *abb_iter_in_crear(const abb_t *arbol) {
    abb_iter_t *iter = malloc(sizeof (abb_iter_t));
    if (iter == NULL) return NULL;

    pila_t *pila = pila_crear();
    if (pila == NULL){
        free(iter);
        return NULL;
    }
    nodo_t *nodo = arbol->raiz;
    while (nodo != NULL){
        pila_apilar(pila, nodo);
        nodo = nodo->izq;
    }
    iter->pila = pila;
    return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter) {
    if (abb_iter_in_al_final(iter)){
        return false;
    }
    nodo_t *nodo = pila_desapilar(iter->pila);
    if (nodo->der){
        nodo = nodo->der;
        pila_apilar(iter->pila, nodo);
    } else {
        return true;
    }

    while (nodo->izq){
        nodo = nodo->izq;
        pila_apilar(iter->pila, nodo);
    }
    return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter) {
    nodo_t *actual = pila_ver_tope(iter->pila);
    return (actual) ? actual->clave : NULL;
}

bool abb_iter_in_al_final(const abb_iter_t *iter) {
    return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t *iter) {
    pila_destruir(iter->pila);
    free(iter);
}
