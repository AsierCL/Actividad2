#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Para a función sleep

#include "cola.h"
#include "lista.h"
#include "estructura.h"


    /* Secuencias de escape ANSI:
    "\x1b[31m"  --  Vermello
    "\x1b[32m"  --  Verde
    "\x1b[0m"   --  Color pred.
    */



void barraDeProgreso(int porcentaxe) {
    const int lonxitudeBarra = 50; // Lonx total da barra de progreso
    int caracteresParaMostrar = porcentaxe * lonxitudeBarra / 100; // Num de caracteres

    printf("["); // Print do inicio da barra
    for (int i = 0; i < lonxitudeBarra; ++i) {
        if (i < caracteresParaMostrar)
            printf("="); // Parte completada
        else
            printf(" "); // Parte sin completar
    }
    printf("] %d%%\r", porcentaxe); // Print final da barra
    fflush(stdout); // Limpa o buffer de salida para escribir o seguinte porcentaxe da barra
}

void mostrarBarra(){
    for (int i = 0; i <= 100; ++i) {
        barraDeProgreso(i); // Funcion debuxar barra
        usleep(10000); // Pausa en microsegundos (1/1,000,000 segundos)
        if(i>100)exit(1);
    }
    printf("\n"); 
}



void leerArquivo(char* nombre_arquivo, TLISTA* lista_impresoras) {

    // Abro o arquivo
    FILE *arquivo_impresoras = fopen(nombre_arquivo, "r");
    
    // Fago un bucle infinito, e pároo cando no fscanf se chegue a EOF
    while(1){
        TIPOELEMENTOLISTA impresora_aux;
        crearCola(&impresora_aux.cola_impresion);
        int i = 0;
        i = fscanf(arquivo_impresoras, "%s %s %s %s", impresora_aux.nombre, impresora_aux.marca, impresora_aux.modelo, impresora_aux.ubicacion);
        if (i==EOF) return;
        insertarElementoLista(lista_impresoras, finLista(*lista_impresoras), impresora_aux);
    }
}


void escribirArquivo(char* nombre_arquivo, TLISTA* lista_impresoras){
    
    // Abro o arquivo
    FILE *arquivo_impresoras = fopen(nombre_arquivo, "w");
    TPOSICION posicion_aux = primeroLista(lista_impresoras);

    while (siguienteLista(lista_impresoras, posicion_aux) != NULL){
        TIPOELEMENTOLISTA impresora_aux;
        recuperarElementoLista(lista_impresoras, posicion_aux, &impresora_aux);
        fprintf(arquivo_impresoras,"%s %s %s %s\n", impresora_aux.nombre, impresora_aux.marca, impresora_aux.modelo, impresora_aux.ubicacion);
        posicion_aux = siguienteLista(lista_impresoras, posicion_aux);
    }
}


void printearLista(TLISTA lista_impresoras){
    // Empezo dende o primeiro elemento, e vou printeando e pasando ao seguinte ca función do TAD ata que chegue a NULL
    TPOSICION posicion_aux = primeroLista(lista_impresoras);
    while (siguienteLista(lista_impresoras, posicion_aux) != NULL){
        TIPOELEMENTOLISTA impresora_aux;
        recuperarElementoLista(lista_impresoras, posicion_aux, &impresora_aux);
        printf("%s %s %s %s\n", impresora_aux.nombre, impresora_aux.marca, impresora_aux.modelo, impresora_aux.ubicacion);
        posicion_aux = siguienteLista(lista_impresoras, posicion_aux);
    }
}


void eliminarImpresora(TLISTA* lista_impresoras){
    
    // Declaro as variables necesarias
    char impresora_eliminar[32];
    TPOSICION posicion_elimiar;
    TIPOELEMENTOLISTA impresora_aux;
    int check = 0;
    posicion_elimiar = primeroLista(*lista_impresoras);
    recuperarElementoLista(lista_impresoras, posicion_elimiar, &impresora_aux);

    // Salida por pantalla
    printf("\n\nEscriba a impresora que queere eliminar:\n");
    printf("(Únicamente escriba o nome (primera columna))\n\n");
    printearLista(*lista_impresoras);
    printf("\n");
    scanf(" %s",impresora_eliminar);


    while ((siguienteLista(lista_impresoras, posicion_elimiar) != NULL) && (check == 0)){
        recuperarElementoLista(lista_impresoras, posicion_elimiar, &impresora_aux);
        
        if(strcmp(impresora_aux.nombre, impresora_eliminar) == 0){
            check = 1;
            printf("\x1b[32mImpresora eliminada:\n%s %s %s %s\x1b[0m\n", impresora_aux.nombre, impresora_aux.marca, impresora_aux.modelo, impresora_aux.ubicacion);
            suprimirElementoLista(lista_impresoras, posicion_elimiar);
            sleep(2);
            return;
        }
        else{
            posicion_elimiar = siguienteLista(lista_impresoras, posicion_elimiar);
        }
    }

    // Caso error
    printf("\x1b[31mNon se atopou a impresora %s\x1b[0m\n",impresora_eliminar);
    sleep(2);
}

void engadirImpresora(TLISTA* lista_impresoras){
    
    // Declaro as variables necesarias
    TIPOELEMENTOLISTA impresora_aux;
    crearCola(&impresora_aux.cola_impresion);
    
    printf("Introduce a nova impresora (nombre|marca|modelo|ubicación):\n");
    scanf("%s %s %s %s", impresora_aux.nombre, impresora_aux.marca, impresora_aux.modelo, impresora_aux.ubicacion);
    insertarElementoLista(lista_impresoras, finLista(*lista_impresoras), impresora_aux);

    printf("\nLista actualizada:\n");
    printearLista(*lista_impresoras);
    sleep(2);
}

void engadirCola(TLISTA* lista_impresoras){
    
    // Declaro as variables necesarias
    char impresora_modificada[32];
    TPOSICION posicion_impresora_modificada;
    TIPOELEMENTOLISTA impresora_aux;
    TIPOELEMENTOCOLA id;
    int check = 0;
    posicion_impresora_modificada = primeroLista(*lista_impresoras);
    recuperarElementoLista(lista_impresoras, posicion_impresora_modificada, &impresora_aux);

    // Salida por terminal
    printearLista(*lista_impresoras);
    printf("Introduce o nome da impresora a que se lle quere engadir traballo:\n");
    scanf(" %s",impresora_modificada);

    // Comprobo unha por unha que coincida
    while ((siguienteLista(lista_impresoras, posicion_impresora_modificada) != NULL) && (check == 0)){
        recuperarElementoLista(lista_impresoras, posicion_impresora_modificada, &impresora_aux);
        
        // Se atopa a impresora:
        if(strcmp(impresora_aux.nombre, impresora_modificada) == 0){
            check = 1;
            printf("\nIntroduce o id do traballo que queres engadir:\n");
            scanf("%d",&id);
            anadirElementoCola(&impresora_aux.cola_impresion, id);
            return;
        }
        else{
            posicion_impresora_modificada = siguienteLista(lista_impresoras, posicion_impresora_modificada);
        }
    }

    // Non atopa a impresora
    printf("\x1b[31mNon se atopou a impresora %s\x1b[0m\n",impresora_modificada);
    sleep(2);
}



void imprimirCola(TCOLA* cola_impresion){
    if (tamanoCola(*cola_impresion)==0){
        printf("\nNon hai traballos pendentes\n");
    }
    
    for(int i=0;i<tamanoCola(*cola_impresion);i++){
        TIPOELEMENTOCOLA elemento_aux;
        consultarPrimerElementoCola(*cola_impresion, &elemento_aux);
        printf("\nO id do elemento %d é: %d\n", i+1, elemento_aux);
        suprimirElementoCola(cola_impresion);
        anadirElementoCola(cola_impresion, elemento_aux);
    }
}

void imprimirTraballosPendentes(TLISTA* lista_impresoras){
    
    // Declaro as variables necesarias
    char nombre_impresora_aux[32];
    TPOSICION posicion_impresora_aux;
    TIPOELEMENTOLISTA impresora_aux;
    int check = 0;
    posicion_impresora_aux = primeroLista(*lista_impresoras);
    recuperarElementoLista(lista_impresoras, posicion_impresora_aux, &impresora_aux);
    printearLista(*lista_impresoras);
    
    printf("Introduce o nome da impresora a que se lle quere consultar a cola de impresión:\n");
    scanf(" %s",nombre_impresora_aux);
    
    while ((siguienteLista(lista_impresoras, posicion_impresora_aux) != NULL) && (check == 0)){
        recuperarElementoLista(lista_impresoras, posicion_impresora_aux, &impresora_aux);
        
        // Se atopa a impresora:
        if(strcmp(impresora_aux.nombre, nombre_impresora_aux) == 0){
            check = 1;
            imprimirCola(&impresora_aux.cola_impresion);
            fflush(stdout);
            sleep(2);
            return;
        }
        else{
            posicion_impresora_aux = siguienteLista(lista_impresoras, posicion_impresora_aux);
        }
    }
    // Non atopa a impresora
    printf("\x1b[31mNon se atopou a impresora %s\x1b[0m\n",posicion_impresora_aux);
    sleep(2);
}

// Menu de axuda
void mostrarAxuda(){
    printf("\na) Eliminar impresora: Elimina unha impresora da lista de impresoras dispoñibles.\n");
    printf("\nb) Engadir impresora: Engade unha impresora á lista de impresoras dispoñibles.\n");
    printf("\nc) Enviar traballo: Envia un traballo de impresión a unha impresora específica.\n");
    printf("\nd) Cola de impresión: Amosa a cola de traballos de impresión en espera.\n");
    printf("\ne) Imprimir traballo: Imprime un traballo dunha impresora específica.\n");
    printf("\nf) Buscar impresoras con pouca carga: Amosa as impresoras con baixa carga de traballo.\n");
    printf("\ng) Axuda: Amosa este menú de axuda.\n");
    printf("\ns) Sair: Sai do programa.\n");
    printf("\n (Enter para sair)\n");
    printf("\n--------------------------------------------------------------\n");
    getchar();
    getchar();
    system("clear");
}
