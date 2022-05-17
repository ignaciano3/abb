#include "abb.h"
#include "testing.h"
#include <string.h>
#include <stdio.h>

static void prueba_abb_basico(){
    abb_t *abb = abb_crear(strcmp, NULL);
    // abb:
    //   1
    // 0   2
    char *clave_1 = "clave1", *clave_2 = "clave2", *clave_0 = "clave0";
    int valor_1 = 1, valor_2 = 2, valor_0 = 0;

    print_test("Arbol vacio", abb_cantidad(abb) == 0);
    print_test("Clave 1 no pertenece", !abb_pertenece(abb, clave_1));

    abb_guardar(abb,clave_1, &valor_1);
    print_test("Clave 1 si pertenece", abb_pertenece(abb, clave_1));
    print_test("Arbol tiene cantidad 1", abb_cantidad(abb) == 1);
    abb_guardar(abb,clave_2, &valor_2);
    print_test("Clave 2 si pertenece", abb_pertenece(abb, clave_2));
    print_test("Arbol tiene cantidad 2", abb_cantidad(abb) == 2);

    print_test("Arbol obtener clave 1 es valor 1", abb_obtener(abb, clave_1) == &valor_1);
    print_test("Arbol obtener clave 2 es valor 2", abb_obtener(abb, clave_2) == &valor_2);

    abb_guardar(abb, clave_0, &valor_0);
    print_test("Arbol obtener clave 0 es valor 0", abb_obtener(abb, clave_0) == &valor_0);

    print_test("Arbol borrar clave 1 es valor 1", abb_borrar(abb, clave_1) == &valor_1);
    print_test("Arbol borrar clave 2 es valor 2", abb_borrar(abb, clave_2) == &valor_2);
    print_test("Arbol obtener clave 2 ahora es NULL", abb_obtener(abb, clave_2) == NULL);
    print_test("Arbol obtener clave 0 es valor 0", abb_obtener(abb, clave_0) == &valor_0);
    abb_destruir(abb);
}

static void prueba_iter(){
    abb_t* abb = abb_crear(strcmp, NULL);
    char *a = "a", *b = "b", *c = "c", *d= "d", *e="e" ;
    int va = 1, vb = 2, vc = 3, vd = 4, ve = 5;
    //      c
    //   a     d
    //     b     e

    abb_guardar(abb,c, &vc);
    abb_guardar(abb,d, &vd);
    abb_guardar(abb,a, &va);
    abb_guardar(abb,b, &vb);
    abb_guardar(abb,e, &ve);

    abb_iter_t *iter = abb_iter_in_crear(abb);
    print_test("Primer nodo es a", strcmp(abb_iter_in_ver_actual(iter), a) == 0);
    print_test("Avanzar es true", abb_iter_in_avanzar(iter));
    print_test("Segundo nodo es b", strcmp(abb_iter_in_ver_actual(iter), b) == 0);
    print_test("Iter no esta al final", !abb_iter_in_al_final(iter));
    abb_iter_in_avanzar(iter);
    print_test("Tercer nodo es c", strcmp(abb_iter_in_ver_actual(iter), c) == 0);
    abb_iter_in_avanzar(iter);
    print_test("Cuarto nodo es d", strcmp(abb_iter_in_ver_actual(iter), d) == 0);
    abb_iter_in_avanzar(iter);
    print_test("Quinto nodo es e", strcmp(abb_iter_in_ver_actual(iter), e) == 0);
    abb_iter_in_avanzar(iter);
    print_test("Iter esta al final", abb_iter_in_al_final(iter));
    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}

void pruebas_abb_estudiante(){
    printf("\nPRUEBAS ABB BASICO\n");
    prueba_abb_basico();
    printf("\nPRUEBAS ITER\n");
    prueba_iter();
    printf("\n");
}

int main(){
    pruebas_abb_estudiante();
    return 0;
}
