#include "abb.h"
#include "testing.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define LARGO_PRUEBA_VOLUMEN 10000

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
    char *a = "a", *b = "b", *c = "c", *d= "d", *e = "e", *f = "f", *g = "g";
    int va = 1, vb = 2, vc = 3, vd = 4, ve = 5, vf=6, vg=7;
    //       d
    //   b        f
    // a   c    e   g

    abb_guardar(abb,d, &vd);
    abb_guardar(abb,b, &vb);
    abb_guardar(abb,f, &vf);
    abb_guardar(abb,c, &vc);
    abb_guardar(abb,a, &va);
    abb_guardar(abb,e, &ve);
    abb_guardar(abb,g, &vg);

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
    print_test("Sexto nodo es f", strcmp(abb_iter_in_ver_actual(iter), f) == 0);
    abb_iter_in_avanzar(iter);
    print_test("Septimo nodo es g", strcmp(abb_iter_in_ver_actual(iter), g) == 0);
    abb_iter_in_avanzar(iter);

    print_test("Iter esta al final", abb_iter_in_al_final(iter));
    print_test("Avanzar es false", !abb_iter_in_avanzar(iter));

    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}

bool visitador(const char *clave, void *dato, void *extra) {
    strcat((char*)extra, clave);
    return true;
}

static void prueba_volumen(){
    abb_t* abb = abb_crear(strcmp, NULL);
    int arr[LARGO_PRUEBA_VOLUMEN];
    char* random_digits = calloc(LARGO_PRUEBA_VOLUMEN*2, sizeof (char));
    char **claves = malloc(sizeof (char*) *LARGO_PRUEBA_VOLUMEN);

    for (int i = 0; i < LARGO_PRUEBA_VOLUMEN; i++){
        arr[i] = rand()%LARGO_PRUEBA_VOLUMEN;
        random_digits[2*i] = (char)(rand() % 79 + '0');
        claves[i] = &random_digits[2*i];
        abb_guardar(abb, claves[i], &arr[i]);
    }
    char claves_ordenadas[200] ="";
    abb_in_order(abb, visitador, &claves_ordenadas);

    abb_iter_t *iter = abb_iter_in_crear(abb);

    int i = 0, error = 0;
    while (!abb_iter_in_al_final(iter)){
        const char* actual_iter = abb_iter_in_ver_actual(iter);
        if (actual_iter[0] != claves_ordenadas[i]){
            error++;
        }
        i++;
        abb_iter_in_avanzar(iter);
    }

    print_test("Iterador itera bien el arbol", error == 0);
    free(random_digits);
    free(claves);
    abb_iter_in_destruir(iter);
    abb_destruir(abb);


}

void pruebas_abb_estudiante(){
    printf("\nPRUEBAS ABB BASICO\n");
    prueba_abb_basico();
    printf("\nPRUEBAS ITER\n");
    prueba_iter();
    printf("\nPRUEBA VOLUMEN\n");
    prueba_volumen();
    printf("\n");
}