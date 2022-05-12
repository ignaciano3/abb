#include "abb.h"
#include "testing.h"
#include "stdio.h"
#include <string.h>

static void prueba_abb_basico(){
    abb_t *abb = abb_crear(strcmp, NULL);
    char *clave_1 = "clave1", *clave_2 = "clave2";
    int valor_1 = 1, valor_2 = 2;

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

    abb_destruir(abb);
}

int main(){
    printf("\nPRUEBA ABB BASICO\n");
    prueba_abb_basico();
    return 0;
}
