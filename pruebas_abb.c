#include "abb.h"
#include "testing.h"
#include "stdio.h"
#include <string.h>
#define LARGO_PRUEBA_VOLUMEN 10000

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

bool visitador(const char *clave, void *dato, void *extra){
    //printf("%c", *clave);
    if (*(int*)dato >= LARGO_PRUEBA_VOLUMEN*9/10){
        *(char*)extra = *strdup(clave);
        return false;
    }

    return true;
}

static void prueba_volumen(){
    abb_t* abb = abb_crear(strcmp, NULL);
    int arr[LARGO_PRUEBA_VOLUMEN];
    char* random_letters = malloc(sizeof (char) * LARGO_PRUEBA_VOLUMEN*2);
    char **claves = malloc(sizeof (char*) *LARGO_PRUEBA_VOLUMEN);

    for (int i = 0; i < LARGO_PRUEBA_VOLUMEN; i++){
        arr[i] = rand()%LARGO_PRUEBA_VOLUMEN;
        random_letters[2*i] = (char)(rand() % 79 + '0');
        claves[i] = &random_letters[2*i];
        abb_guardar(abb, claves[i], &arr[i]);
    }
    char hola;
    abb_in_order(abb, visitador, &hola);
    printf("\n%c", hola);



}

int main(){
    printf("\nPRUEBA ABB BASICO\n");
    prueba_abb_basico();
    printf("\nPRUEBA ABB VOLUMEN\n");
    prueba_volumen();
    return 0;
}
