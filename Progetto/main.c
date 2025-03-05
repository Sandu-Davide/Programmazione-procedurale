#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gamelib.h"

int main( ) {
    time_t t;
    srand((unsigned) time(&t));

    boolean continue_menu = false;
    do{
        int chose_options = 0;

        print_title("Menu", _YELLOW_);
        puts("\n" _YELLOW_ "1. " _WHITE_ "Imposta gioco\n"_YELLOW_ "2. " _WHITE_ "Gioca\n" _YELLOW_ "3. " _WHITE_ "Termina gioco\n" _YELLOW_ "4. " _WHITE_ "Crediti\n");
        puts("Inserisci un valore da 1 a 4 in base alla funzione che vuoi eseguire\n");
        
        chose_options = input_scan(1,4, _MENU_ "Comando >> ");

        switch (chose_options){
            case 1: 
                continue_menu = imposta_gioco();
                continue_menu = true;
                break;
            case 2:
                continue_menu = gioca();
                break;
            case 3: 
                continue_menu = termina_gioco();
                break;
            case 4: 
                continue_menu = crediti();
                break;
        }
    }while (continue_menu);
    
}
