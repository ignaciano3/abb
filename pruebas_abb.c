#include "abb.h"
#include "testing.h"
#include "stdio.h"
#include <string.h>
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

bool visitador(const char *clave, void *dato, void *extra) {
    //printf("%c", *clave);
    if (*clave >= *(char *) extra) {
        *(char *)extra = *clave;
    }
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
    char ultimo;
    abb_in_order(abb, visitador, &ultimo);
    print_test("Ultimo es ~", ultimo);
    printf("\n%c", ultimo);
    free(random_digits);
    free(claves);
    abb_destruir(abb);


}

int main(){
    printf("\nPRUEBA ABB BASICO\n");
    prueba_abb_basico();
    printf("\nPRUEBA ABB VOLUMEN\n");
    //prueba_volumen();
    return 0;
}
