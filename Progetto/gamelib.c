#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gamelib.h"

// Definizioni delle funzioni in gamelib.c.
// Piu altre funzioni di supporto.
// Le funzioni richiamabili in main.c non devono essere static.
// Le altre devono essere static (non visibili all'esterno).

#define _IM_GAME_ _CYAN_"Imposta gioco > "_WHITE_
#define _IM_GAME_PLAYER_ _CYAN_"Imposta gioco > Crea personaggi > "_WHITE_
#define _MAP_ _GREEN_"Mappa > "_WHITE_
#define _TAB_ "   "
#define _INFO_ _GREEN_" > "_WHITE_
#define _INFO_GAME_ _YELLOW_" > "_WHITE_
#define _INFO_GAME_R _RED_" > "_WHITE_
#define _INFO_GAME_G _GREEN_" > "_WHITE_
#define _INFO_CREDITI_ _MAGENTA_" > "_WHITE_

#define NUMBER_PLAYER 3
#define MAX_CHAR_NAME 25
#define PV_MAX 3
#define NUM_DADI 2
#define NUM_ROOM_MIN 15
#define FACCE_DADI 6
#define CONTROLLO_VITA_NEMICI 5
#define CONTROLLO_VITA_GIOCATORI 15

static Giocatore *last_player_game[3] = {NULL, NULL, NULL};
static int position_player;

static Giocatore *giocatori[3] = {NULL, NULL, NULL};
static int num_player_in_game = 0;

static boolean print_jaffar = false;

static boolean imposta_players();
static void create_players(const int position);
static boolean check_class();
static void delete_players(boolean print_message);
static void print_player();

static boolean player_impostati = false;

static Stanza *first_room = NULL;
static Stanza *last_room = NULL;

static boolean imposta_map();
static void ins_stanza();
static void canc_stanza();
static void stampa_stanze();
static void genera_random();
static boolean chiudi_mappa();
static void insert_info_room(Stanza *room);
static void insert_info_random(Stanza *room);
static void delete_all_map(boolean print_message);

static boolean prince_pass = true;
static boolean mappa_impostata = false;

static void ordine_player();
static boolean avanza(Giocatore *player, boolean possibile_avanzare);
static int passa(int value, Giocatore *player);
static void stampa_zona(Stanza *room);
static void stampa_giocatore(Giocatore *player);
static boolean cerca_porta_segreta(Stanza *room, Giocatore *player);
static void combatti(Giocatore *player, boolean *avanzamento);
static void scappa(Giocatore *player);
static void prendi_tesoro(Giocatore *player);

static void name_players_member();

static void idle_menu(char *content, char *color);
static void death_player(Giocatore *player);

static void apparizione_mercante(Giocatore *player);

static boolean sconfitta_jaffar = false;
static boolean lose_turno = false;

static boolean morte_burrone = false;

static All_name *first_name = NULL;
static All_name *last_name = NULL;

boolean imposta_gioco(){
    boolean continue_menu = false;

    do{
        int chose_options = 0;

        print_title("Imposta gioco", _CYAN_);
        puts("\n" _CYAN_ "1. " _WHITE_ "Imposta giocatori\n"_CYAN_ "2. " _WHITE_ "Imposta mappa\n" _CYAN_ "3. " _WHITE_ "Torna indietro\n");
        puts("Inserisci un valore da 1 a 3 in base alla funzione che vuoi eseguire\n");

        chose_options = input_scan(1,3,_IM_GAME_ "Comando >> ");

        switch (chose_options){
            case 1:
                if(player_impostati){
                    delete_players(true);
                    player_impostati = false;
                }
                player_impostati = imposta_players();
                continue_menu = player_impostati;
                break;
            case 2:
                continue_menu = imposta_map();
                break;
            case 3:
                continue_menu = false;
                break;
        }
    }while(continue_menu);

    return true;
}

static boolean imposta_players(){
    int num_players = 0;
    boolean result = false;

    puts("\nInserisci il numero di giocatori che parteciperanno [massimo 3]\n");

    num_players = input_scan(1,3, _IM_GAME_PLAYER_ "Inserimento >> ");
    num_player_in_game = num_players;
    puts("");
    for(int i=0; i<num_players; i++){
        create_players(i);
    }

    result = check_class();
    print_player();

    idle_menu("Premi invio per tornale al menu [Imposta gioco]...", _CYAN_);

    return result;
}

static void create_players(const int position){
    char name_player[MAX_CHAR_NAME];
	giocatori[position] = (Giocatore*) malloc(sizeof(Giocatore));
    //-----------------------------------------------------------------------------------------
    if(giocatori[position] == NULL){
        puts(_ERROR_ "non e' stato possibile creare un nuovo giocatore");
    }else{
        printf("Inserisci il nome del giocatore " _CYAN_ "> " _WHITE_);
        fgets(name_player, MAX_CHAR_NAME, stdin);
        if(strlen(name_player) == 24){
            while(getchar() != '\n');
        }
        for(int i=0; i<strlen(name_player); i++){
            if(name_player[i] == '\n'){
                name_player[i] = '\0';
            }
        }

        strcpy(giocatori[position]->nome_giocatore, name_player);

        int value_class = doppleganger;
        
        value_class = input_scan(0,1, "Inserisci la classe [0 principe, 1 doppleganger] " _CYAN_ "> " _WHITE_);

        giocatori[position]->classe_giocatore = value_class;
        giocatori[position]->p_vita_max = PV_MAX;
        giocatori[position]->p_vita = PV_MAX;
        giocatori[position]->dadi_attacco = NUM_DADI;
        giocatori[position]->dadi_difesa = NUM_DADI;
        giocatori[position]->monete = 0;
        giocatori[position]->weapon = nessuna;
        giocatori[position]->volte_scappa = giocatori[position]->classe_giocatore == principe ? 1 : 2;
    }

    puts("");

}

static boolean check_class(){
    boolean result = false;

    Giocatore* possible_prince = NULL;
	boolean main_class = false;

    for(int i=0; i<NUMBER_PLAYER; i++){
        if(giocatori[i] != NULL){
            switch (giocatori[i]->classe_giocatore)
            {
            case 0:
                main_class = true;
                break;
            case 1:
                if(possible_prince == NULL) possible_prince = giocatori[i];
                break;
            }
        }
    }

    if(!main_class && possible_prince != NULL){
        possible_prince->classe_giocatore = principe;
        printf( _RED_  ">> " _WHITE_ "Non era presente nessun principe quindi la classe del giocatore " _CYAN_ "%s" _WHITE_ " e' stata cambiata in" _CYAN_ " principe" _WHITE_ ".\n" _TAB_ "Se vuoi fare diversamente imposta di nuovo i giocatori!\n\n" , possible_prince->nome_giocatore);
    }else if(main_class){
        boolean check_prince = false;
        for(int i=0; i<NUMBER_PLAYER; i++){
            if(giocatori[i] != NULL){
                if(giocatori[i]->classe_giocatore == principe){
                    if(check_prince){
                        printf("\n" _RED_  ">> " _WHITE_ "Il giocatore " _CYAN_  "%s " _WHITE_ "aveva la classe principe ma solo un giocatore puo' averla!\n" _TAB_ "La sua classe e' stata cambiata in doppleganger.\n" _TAB_ "Se vuoi fare diversamente imposta di nuovo i giocatori!\n\n", giocatori[i]->nome_giocatore);
                        giocatori[i]->classe_giocatore = doppleganger;
                    }
                    if(giocatori[i]->classe_giocatore == 0){
                        check_prince = true;
                    }
                }
            }

        }
    }

    puts(_CYAN_ "Tutti i giocatori sono stati impostati correttamente!\n" _WHITE_);
    result = true;
    
    return result;
}

static void print_player(){
    for(int i=0; i<NUMBER_PLAYER; i++){
        if(giocatori[i] != NULL){
            printf("Giocatore" _CYAN_" %d:" _WHITE_ "\n\tNome " _CYAN_ ">" _WHITE_ " %s\n", i+1, giocatori[i]->nome_giocatore);
            switch ( giocatori[i]->classe_giocatore){
                case 0:
                    puts("\tClasse " _CYAN_ "> principe" _WHITE_);
                    break;
                case 1:
                    puts("\tClasse " _CYAN_ "> doppleganger" _WHITE_);
                    break;
            }
        }else{
            printf(_RED_"ATTENZIONE" _WHITE_ ": il giocatore %d non e' stato inserito, dunque non partecipera'.\n", i+1);
        }
    }
    puts("");
}

static void delete_players(boolean print_message){
    for(int i=0; i<NUMBER_PLAYER; i++){
        if(giocatori[i] != NULL){
            free(giocatori[i]);
            giocatori[i] = NULL;
        }
    }
    if(print_message){
        puts(_RED_ "\nATTENZIONE" _WHITE_ ": tutti i giocatori che erano presenti sono stati eliminati!");   
    }
}

static boolean imposta_map(){
    int chose_options = 0;
    boolean continue_menu = false;
    
    do{
        print_title("Imposta mappa", _GREEN_);
        puts("\n" _GREEN_ "1. " _WHITE_ "Inserisci stanza/e\n"_GREEN_ "2. " _WHITE_ "Cancella stanza\n" _GREEN_ "3. " _WHITE_ "Stampa stanze");
        puts(_GREEN_ "4. " _WHITE_ "Genera casualmente\n" _GREEN_ "5. " _WHITE_ "Chiudi mappa\n");

        if(mappa_impostata){
            delete_all_map(false);
            puts(_ATTENZIONE_ "La mappa creata precedentemente e' stata eliminata\n");
            mappa_impostata = false;
        }

        puts("Inserisci un valore da 1 a 5 in base alla funzione che vuoi eseguire\n");

        chose_options = input_scan(1,5, _MAP_ "Comando >> ");

        switch (chose_options){
            case 1:
                puts("\nInserisci il numero di stanza da creare. [massimo 30 per volta]");
                int num_room = input_scan(1,30, _MAP_ "Inserimento >> ");
                for(int i=0; i<num_room; i++){
                    ins_stanza();
                }
                continue_menu = true;
                break;
            case 2: 
                canc_stanza();
                idle_menu("Premi invio per tornare al menu [Imposta Mappa]...", _GREEN_);
                continue_menu = true;
                break;
            case 3: 
                stampa_stanze();
                idle_menu("Premi invio per tornare al menu [Imposta Mappa]...", _GREEN_);
                continue_menu = true;
                break;
            case 4:
                genera_random();
                idle_menu("Premi invio per tornare al menu [Imposta Mappa]...", _GREEN_);
                continue_menu = true;
                break;
            case 5:
                continue_menu = chiudi_mappa();
                break;
        }
    }while(continue_menu);

    return mappa_impostata;
}

static void ins_stanza(){
    //la mappa viene creata strutturalmente, ogni volta che si crea una stanza si impostano
    //i puntatori delle direzioni a NULL e' si chiede le informazioni della stanza appena creata,
    //dopo viene chiesto la direzione della nuova stanza rispetto alla precedente, quindi se l'utente
    //ci chiede che questa stanza deve stare a destra rispetto alla precedente allora impostiamo il valore
    //della direzione della stanza precedente verso la nuova stanza. Questo grazie a last_room che tiene traccia
    //dell'ultima stanza creata. La struttura tiene traccia della stanza precedente rispetto alla propria stanza.

    print_title("Creazione stanza", _GREEN_);
    Stanza *new_room = malloc(sizeof(Stanza));

    if(new_room == NULL){
        puts(_ERROR_ "non e' stato possibile creare una nuova stanza");
    }else{
        if(first_room == NULL){
            puts(_GREEN_ "\nATTENZIONE" _WHITE_ ": questa e' la prima stanza che stai creando.\n");

            new_room->stanza_destra = NULL;
            new_room->stanza_sinistra = NULL;
            new_room->stanza_sopra = NULL;
            new_room->stanza_sotto = NULL;
            new_room->precedente = NULL;
            
            insert_info_room(new_room);
            if(new_room->trabocchetto != nessuno){
                puts(_ATTENZIONE_ "Il trabocchetto inserito nella prima stanza e' stato eliminato! Non ci possono essere nella prima stanza.");
            }

            first_room = new_room;
            last_room = new_room;
        }else{
            //chiedo la direzione della stanza rispetto alla precedente
            int chose_options = 0;
            boolean error = false;

            puts("Inserisci un valore da 1 a 4 per indicare la\ndirezione della stanza rispetto alla precedente");
            puts("[1-destra]\t[2-sinistra]\t[3-sopra]\n\t[4-sotto]\n");
            
            do{
                printf(_MAP_ "Comando >> ");
                int check_scan = scanf("%d", &chose_options); 
                // la funzione scanf restituisce 1 se legge in input un numero, 0 se non legge in input un numero
                if(check_scan != 1){
                    puts(_ERROR_ "hai inserito un carattere al posto di un numero!");
                    while (getchar() != '\n');
                    error = true;
                }else{
                    error = (chose_options < 1 || chose_options > 4) ? true : false;
                    if(error){
                        puts(_ERROR_ "hai inserito un comando non valido! Riprova");
                    }
                }
            }while(error);
            //dopo aver saputo quale direzione deve avere la stanza la imposto sulla precedente
            //(tutte le altre direzioni sono sempre a NULL quindi non c'e' bisogno di specificare le altre direzioni a NULL)
            switch (chose_options){
                case 1:
                    last_room->stanza_destra = new_room;
                    break;
                case 2:
                    last_room->stanza_sinistra = new_room;
                    break;
                case 3:
                    last_room->stanza_sopra = new_room;
                    break;
                case 4:
                    last_room->stanza_sotto = new_room;
                    break;
            
            }

            new_room->stanza_destra = NULL;
            new_room->stanza_sinistra = NULL;
            new_room->stanza_sopra = NULL;
            new_room->stanza_sotto = NULL;
            new_room->precedente = last_room;
            insert_info_room(new_room);
            //quando ho finito di lavorare con la stanza precedente la nuova stanza diventa
            //l'ultima creata.
            last_room = new_room;

        }
    }
}

static void insert_info_room(Stanza *room){
    puts("Inserisci un numero da 1 a 10 per scegliere la tipologia della stanza");
    puts("[corridoio]\t[scala]\t[sala_banchetto]\n[magazzino]\t[posto_guardia]\t[prigione]\n[armeria]\t[moschea]\t[torre]\n[bagni]");
    int type_room = input_scan(1,10,_GREEN_ "Creazione mappa" _WHITE_ " > Tipo stanza >> ");
    type_room--;
    room->stanza = type_room;
    puts("\nInserisci un numero da 1 a 6 per scegliere la tipologia del tesoro");
    puts("[nessun_tesoro]\t[verde_veleno]\t[blu_guarigione]\n[rosso_aumenta_vita]\t[spada_tagliente]");
    int type_tesoro = input_scan(1,6, _GREEN_ "Creazione mappa" _WHITE_ " > Tipo tesoro >> ");
    type_tesoro--;
    room->tesoro = type_tesoro;
    puts("\nInserisci un numero da 1 a 5 per scegliere la tipologia del trabocchetto");
    puts("[nessuno]\t[tegola]\t[lame]\n[caduta]\t[burrone]");
    int type_trabocchetto = input_scan(1,5, _GREEN_ "Creazione mappa" _WHITE_ " > Tipo trabocchetto >> ");
    type_trabocchetto--;
    room->trabocchetto = type_trabocchetto;
}

static void canc_stanza(){
    if(last_room == NULL)
        printf(_ERROR_ "Non e' stato possibile trovare l'ultima stanza per la sua rimozione!");
    else{
        if(last_room != first_room){
            Stanza *room_provisoria = last_room->precedente;
            free(last_room);
            last_room = room_provisoria;

            last_room->stanza_destra = NULL;
            last_room->stanza_sinistra = NULL;
            last_room->stanza_sopra = NULL;
            last_room->stanza_sotto = NULL;
        }else{
            free(first_room);
            first_room = NULL;
            last_room = NULL;
        }
        

        puts("\n" _ATTENZIONE_ ": l'ultima stanza inserita e' stata eliminata!");
    }
}

static void stampa_stanze(){
    puts("");
    if(first_room == NULL){
        puts(_ERROR_ "Non ci sono stanze create.");
    }else{
        Stanza *room_provisioria = first_room;
        int i=0;
        while(room_provisioria != NULL){
            i++;
            printf("[" _GREEN_ "Stanza n.%d" _WHITE_ "]\n",i);
            switch (room_provisioria->stanza){
                case 0:
                    puts(_INFO_ "Tipologia della stanza: " _GREEN_ "corridoio" _WHITE_);
                    break;
                case 1:
                    puts(_INFO_ "Tipologia della stanza: " _GREEN_ "scala" _WHITE_);
                    break;
                case 2:
                    puts(_INFO_ "Tipologia della stanza: " _GREEN_ "sala banchetto" _WHITE_);
                    break;
                case 3:
                    puts(_INFO_ "Tipologia della stanza: " _GREEN_ "magazzino" _WHITE_);
                    break;
                case 4:
                    puts(_INFO_ "Tipologia della stanza: " _GREEN_ "posto guardia" _WHITE_);
                    break;
                case 5:
                    puts(_INFO_ "Tipologia della stanza: " _GREEN_ "prigione" _WHITE_);
                    break;
                case 6:
                    puts(_INFO_ "Tipologia della stanza: " _GREEN_ "armeria" _WHITE_);
                    break;
                case 7:
                    puts(_INFO_ "Tipologia della stanza: " _GREEN_ "moschea" _WHITE_);
                    break;
                case 8:
                    puts(_INFO_ "Tipologia della stanza: " _GREEN_ "torre" _WHITE_);
                    break;
                case 9:
                    puts(_INFO_ "Tipologia della stanza: " _GREEN_ "bagni" _WHITE_);
                    break;
            }

            switch (room_provisioria->trabocchetto){
                case 0:
                    puts(_INFO_ "Tipologia di trabocchetto: " _GREEN_ "nessuno" _WHITE_);
                    break;
                case 1:
                    puts(_INFO_ "Tipologia di trabocchetto: " _GREEN_ "tegola" _WHITE_);
                    break;
                case 2:
                    puts(_INFO_ "Tipologia di trabocchetto: " _GREEN_ "lame" _WHITE_);
                    break;
                case 3:
                    puts(_INFO_ "Tipologia di trabocchetto: " _GREEN_ "caduta" _WHITE_);
                    break;
                case 4:
                    puts(_INFO_ "Tipologia di trabocchetto: " _GREEN_ "burrone" _WHITE_);
                    break;
            }

            switch (room_provisioria->tesoro){
                case 0:
                    puts(_INFO_ "Tipologia di tesoro: " _GREEN_ "nessun tesoro" _WHITE_);
                    break;
                case 1:
                    puts(_INFO_ "Tipologia di tesoro: " _GREEN_ "verde veleno" _WHITE_);
                    break;
                case 2:
                    puts(_INFO_ "Tipologia di tesoro: " _GREEN_ "blu guarigione" _WHITE_);
                    break;
                case 3:
                    puts(_INFO_ "Tipologia di tesoro: " _GREEN_ "rosso aumenta vita" _WHITE_);
                    break;
                case 4:
                    puts(_INFO_ "Tipologia di tesoro: " _GREEN_ "spada tagliente" _WHITE_);
                    break;
                case 5:
                    puts(_INFO_ "Tipologia di tesoro: " _GREEN_ "scudo" _WHITE_);
                    break;
            }

            printf("\n" _INFO_ "Stanza destra: %p\n",room_provisioria->stanza_destra);
            printf(_INFO_ "Stanza sinistra: %p\n",room_provisioria->stanza_sinistra);
            printf(_INFO_ "Stanza sopra: %p\n",room_provisioria->stanza_sopra);
            printf(_INFO_ "Stanza sotto: %p\n\n",room_provisioria->stanza_sotto);
            
            if(room_provisioria->stanza_destra != NULL){
                room_provisioria = room_provisioria->stanza_destra;
                puts(_INFO_ "La prossima stanza diretta verso: " _GREEN_ "destra"  _WHITE_);
            }else if(room_provisioria->stanza_sinistra != NULL){
                room_provisioria = room_provisioria->stanza_sinistra;
                puts(_INFO_ "La prossima stanza diretta verso: " _GREEN_ "sinistra" _WHITE_);
            }else if(room_provisioria->stanza_sopra != NULL){
                room_provisioria = room_provisioria->stanza_sopra;
                puts(_INFO_ "La prossima stanza diretta verso: " _GREEN_ "sopra" _WHITE_);
            }else if(room_provisioria->stanza_sotto != NULL){
                room_provisioria = room_provisioria->stanza_sotto;
                puts(_INFO_ "La prossima stanza diretta verso: " _GREEN_ "sotto" _WHITE_);
            }else{
                puts(_INFO_ "Non ci sono altre stanze");
                room_provisioria = NULL;
            }
        }
    }
}

static void genera_random(){
    puts("");

    delete_all_map(true);

    for(int i=0; i<NUM_ROOM_MIN; i++){
        Stanza *new_room = malloc(sizeof(Stanza));
        if(new_room == NULL){
            puts(_ERROR_ "Non e' possibile generare altre stanze!");
        }else{
            if(first_room == NULL){
                new_room->stanza_destra = NULL;
                new_room->stanza_sinistra = NULL;
                new_room->stanza_sopra = NULL;
                new_room->stanza_sotto = NULL;
                new_room->precedente = NULL;
                
                insert_info_random(new_room);
                new_room->trabocchetto = nessuno;
                new_room->nemico = NULL;
                
                first_room = new_room;
                last_room = new_room;
            }else{
                insert_info_random(new_room);
                new_room->nemico = NULL;
                int direzzione = rand() % 4;
                switch (direzzione){
                    case 0:
                        last_room->stanza_destra = new_room;
                        break;
                    case 1:
                        last_room->stanza_sinistra = new_room;
                        break;
                    case 2:
                        last_room->stanza_sopra = new_room;
                        break;
                    case 3:
                        last_room->stanza_sotto = new_room;
                        break;
                }
                new_room->stanza_destra = NULL;
                new_room->stanza_sinistra = NULL;
                new_room->stanza_sopra = NULL;
                new_room->stanza_sotto = NULL;
                new_room->precedente = last_room;
                //quando ho finito di lavorare con la stanza precedente la nuova stanza diventa
                //l'ultima creata.
                last_room = new_room;

            }
        }   
    }


    puts(_GREEN_ "ATTENZIONE" _WHITE_ ": Le stanze sono state generate.");
}

static void delete_all_map(boolean print_message){
    if(first_room != NULL){
        Stanza *room_provisioria = first_room;
        Stanza *free_room = room_provisioria;

        int i=0;
        while(room_provisioria != NULL){
            i++;
            if(print_message){
                printf("La stanza n.%d e' stata " _RED_ "eliminata" _WHITE_ "!\n",i);
            }

            if(room_provisioria->stanza_destra != NULL){
                room_provisioria = room_provisioria->stanza_destra;
                free(free_room);
                free_room = room_provisioria;
            }else if(room_provisioria->stanza_sinistra != NULL){
                room_provisioria = room_provisioria->stanza_sinistra;
                free(free_room);
                free_room = room_provisioria;
            }else if(room_provisioria->stanza_sopra != NULL){
                room_provisioria = room_provisioria->stanza_sopra;
                free(free_room);
                free_room = room_provisioria;
            }else if(room_provisioria->stanza_sotto != NULL){
                room_provisioria = room_provisioria->stanza_sotto;
                free(free_room);
                free_room = room_provisioria;
            }else{
                if(print_message){
                    puts(_RED_ "\nATTENZIONE" _WHITE_ ": Sono state eliminate tutte le stanze precedentemente create!\n");
                }
                room_provisioria = NULL;
                free(free_room);
                first_room = NULL;
                last_room = NULL;
            }
        }
    }
}

static boolean chiudi_mappa(){
    boolean close_map = false;
    puts("");
    int i=0;
    if(first_room == NULL){
        puts(_ERROR_ "Non ci sono stanze create.");
    }else{
        Stanza *room_provisioria = first_room;
        while(room_provisioria != NULL){
            i++;

            if(room_provisioria->stanza_destra != NULL){
                room_provisioria = room_provisioria->stanza_destra;
            }else if(room_provisioria->stanza_sinistra != NULL){
                room_provisioria = room_provisioria->stanza_sinistra;
            }else if(room_provisioria->stanza_sopra != NULL){
                room_provisioria = room_provisioria->stanza_sopra;
            }else if(room_provisioria->stanza_sotto != NULL){
                room_provisioria = room_provisioria->stanza_sotto;
            }else{
                room_provisioria = NULL;
            }
        }
    }

    if(i < NUM_ROOM_MIN){
        puts(_ATTENZIONE_ "La mappa non contiene il numero minimo di stanze. (15)");
        close_map = true;
        mappa_impostata = false;
        idle_menu("Premi invio per tornare al menu [Imposta Mappa]...", _GREEN_);
    }else{
        puts(_GREEN_ "SUCCESSO"  _WHITE_ ": La mappa e' impostata correttamente.");
        close_map = false;
        mappa_impostata = true;
        idle_menu("Premi invio per tornare al menu [Imposta gioco]...", _GREEN_);
    }

    return close_map;
}

boolean gioca(){
    print_title("Gioco", _YELLOW_);
    boolean result = false;
    int num_round = 1; //Serve per output
    if(player_impostati && mappa_impostata){
        //faccio partire tutti i giocatori disponibili alla prima stanza
        for(int i=0; i<NUMBER_PLAYER; i++){
            if(giocatori[i] != NULL){
                giocatori[i]->posizione = first_room;
                name_players_member(giocatori[i]);
            }
        }
        boolean game_continue = true; //decide se continuare il gioco o meno, diventa false se il gioco finisce

        do{
            boolean cerca_room_segreta = true;
            int num_turno = 0; //la posizione del player attuale che gioca
            printf("Round n." _YELLOW_ "%d" _WHITE_ ", ", num_round);
            ordine_player(); // ordine dei giocatori, ogni volta che tutti hanno giocato rigenera
            boolean turno_players = true; //diventa false se tutti i giocatori hanno giocato i turni
            boolean player_avanza = true; //il player attuale puo avanzare o meno
            do{
                if(giocatori[num_turno] != NULL){
                    printf("\nTurno del giocatore: " _YELLOW_ "%s\n" _WHITE_, giocatori[num_turno]->nome_giocatore);
                    puts("1. Avanza\t2. Combatti\n3. Scappa\t4. Stampa giocatore\n5. Stampa zona\t6. Prendi tesoro\n7. Cerca porta\t8. Passa turno");
                    int chose_option = input_scan(1, 8, "Comando " _YELLOW_ "> " _WHITE_);
                    switch (chose_option){
                        case 1:
                            if(player_avanza){
                                player_avanza = avanza(giocatori[num_turno], player_avanza);
                                cerca_room_segreta = false;
                                if(giocatori[num_turno]->p_vita < 1 || giocatori[num_turno]->p_vita > CONTROLLO_VITA_GIOCATORI){
                                    printf("\n" _INFO_GAME_ "Il giocatore "_RED_ "%s" _WHITE_ " e' morto!\n", giocatori[num_turno]->nome_giocatore);
                                    death_player(giocatori[num_turno]);
                                    player_avanza = true;
                                    lose_turno = false;
                                }
                            }else{
                                puts("\n" _INFO_GAME_ "Non puoi piu' avanzare.");
                            }
                            break;
                        case 2:
                            combatti(giocatori[num_turno], &player_avanza);
                            break;
                        case 3:
                            scappa(giocatori[num_turno]);
                            break;
                        case 4:
                            stampa_giocatore(giocatori[num_turno]);
                            break;
                        case 5:
                            stampa_zona(giocatori[num_turno]->posizione);
                            break;
                        case 6:
                            prendi_tesoro(giocatori[num_turno]);
                            if(giocatori[num_turno]->p_vita < 1 || giocatori[num_turno]->p_vita > CONTROLLO_VITA_GIOCATORI){
                                printf("\n" _INFO_GAME_ "Il giocatore "_RED_ "%s" _WHITE_ " e' morto!\n", giocatori[num_turno]->nome_giocatore);
                                death_player(giocatori[num_turno]);
                                player_avanza = true;
                            }
                            break;
                        case 7:
                            if(cerca_room_segreta){
                                player_avanza = cerca_porta_segreta(giocatori[num_turno]->posizione, giocatori[num_turno]);
                                cerca_room_segreta = false;
                            }else{
                                puts("\n" _INFO_GAME_R "Non puoi cercare la stanza segreta");
                            }
                            
                            break;
                        case 8:
                            num_turno = passa(num_turno, giocatori[num_turno]);
                            player_avanza = true;
                            cerca_room_segreta = true;
                            break;
                    }
                }

                if(lose_turno){
                    puts("Per colpa del trabocchetto hai perso un turno");
                    num_turno++;
                    lose_turno = false;
                }

                if(num_turno == num_player_in_game){
                    printf("\nTutti i giocatori hanno giocato il round n.%d\n\n", num_round++);
                    turno_players = false; 
                }

                if(num_player_in_game == 0){
                    puts(_INFO_GAME_R _RED_ "HAI PERSO" _WHITE_ "!\n" _INFO_GAME_R "Tutti i giocatori sono morti.");
                    game_continue = false;
                    turno_players = false;
                }
                if(sconfitta_jaffar){
                    printf(_GREEN_ " > HAI VINTO" _WHITE_ "!\n" _GREEN_ " > " _WHITE_ "Il giocatore %s e' riuscito a sconfiggere " _RED_ "Jaffar" _WHITE_ "!\n" _TAB_ "Ora e' il vincitore della partita!\n", giocatori[num_turno]->nome_giocatore);
                    game_continue = false;
                    turno_players = false;
                    sconfitta_jaffar = false;
                    print_jaffar = false;
                    last_player_game[position_player] = malloc(sizeof(Giocatore));
                    strcpy(last_player_game[position_player]->nome_giocatore, giocatori[num_turno]->nome_giocatore);
                    if(position_player <3){
                        position_player++;
                    }else{
                        position_player = 0;
                    }
                    
                }

            }while(turno_players);

        }while(game_continue);

        delete_all_map(false);
        delete_players(false);
        prince_pass = true;
        player_impostati = false;
        mappa_impostata = false;
        
        idle_menu("Premi invio per tornare al menu...", _YELLOW_);
        result = true;
    }else{
        puts("\n" _ERROR_ "Il gioco non e' stato impostato correttamente.");
        idle_menu("Premi invio per tornare al menu...", _YELLOW_);
        result = true;
    }

    return result;
}

static void ordine_player(){
    for(int i=0; i< num_player_in_game; i++){
        int casuale = rand() % num_player_in_game;
        Giocatore *pos_temp = giocatori[i];
        giocatori[i] = giocatori[casuale];
        giocatori[casuale] = pos_temp;
    }
    printf("Ordine dei giocatori: " _YELLOW_);
    for(int i=0; i<num_player_in_game; i++){
        printf("%s ", giocatori[i]->nome_giocatore);
    }
    
    puts(_WHITE_ "");
}

static boolean avanza(Giocatore *player, boolean possibile_avanzare){
    boolean avanzato = true;
    puts("");
    if(possibile_avanzare){
        //faccio avanzare il player alla prossima stanza disponibile
        if((player->posizione)->stanza_destra != NULL){
            (player->posizione) = (player->posizione)->stanza_destra;
            puts("Sei avanzato nella prossima stanza (" _YELLOW_ "destra" _WHITE_ ")");
        }else if((player->posizione)->stanza_sinistra != NULL){
            (player->posizione) = (player->posizione)->stanza_sinistra;
            puts("Sei avanzato nella prossima stanza (" _YELLOW_ "sinistra" _WHITE_ ")");
        }else if((player->posizione)->stanza_sopra != NULL){
            (player->posizione) = (player->posizione)->stanza_sopra;
            puts("Sei avanzato nella prossima stanza (" _YELLOW_ "sopra" _WHITE_ ")");
        }else if((player->posizione)->stanza_sotto != NULL){
            (player->posizione) = (player->posizione)->stanza_sotto;
            puts("Sei avanzato nella prossima stanza (" _YELLOW_ "sotto" _WHITE_ ")");
        }else{
            puts("Non puoi avanzare ulteriormente perche' non esiste un'altra stanza");
        }
        
        int monete_trovate = (rand() % 5)+1;
        player->monete += monete_trovate;
        printf(_INFO_GAME_G "Hai trovato " _GREEN_ "%d" _WHITE_ " %s!\n", monete_trovate, monete_trovate != 1 ? "monete" : "moneta");

        if((player->posizione) == last_room && print_jaffar){
            puts(_ATTENZIONE_ "Nella stanza e' presente Jaffar!");
        }

        if((player->posizione) != last_room){
            apparizione_mercante(player);
            //attivo l'effetto del trabochetto
            switch ((player->posizione)->trabocchetto){
                case 0:
                    puts(_INFO_GAME_ "Tipologia di trabocchetto: " _YELLOW_ "nessuno" _WHITE_);
                    break;
                case 1:
                    if(prince_pass && player->classe_giocatore == principe){
                        puts(_INFO_GAME_ "Hai ignorato il primo trabocchetto trovato!");
                        puts(_INFO_GAME_ "Tipologia di trabocchetto: " _YELLOW_ "tegola" _WHITE_);
                        prince_pass = false;
                    }else{
                        puts(_INFO_GAME_ "Tipologia di trabocchetto: " _YELLOW_ "tegola" _WHITE_);
                        puts(_INFO_GAME_R "Hai perso " _RED_ "1 " _WHITE_ "punto vita.");
                        player->p_vita -= 1;
                    }
                    
                    break;
                case 2:
                    if(prince_pass && player->classe_giocatore == principe){
                        puts(_INFO_GAME_ "Hai ignorato il primo trabocchetto trovato!");
                        puts(_INFO_GAME_ "Tipologia di trabocchetto: " _YELLOW_ "lame" _WHITE_);
                        prince_pass = false;
                    }else{
                        puts(_INFO_GAME_ "Tipologia di trabocchetto: " _YELLOW_ "lame" _WHITE_);
                        puts(_INFO_GAME_R "Hai perso " _RED_ "2 " _WHITE_ "punti vita.");
                        player->p_vita -= 2;
                    }
                    break;
                case 3:
                    if(prince_pass && player->classe_giocatore == principe){
                        puts(_INFO_GAME_ "Hai ignorato il primo trabocchetto trovato!");
                        puts(_INFO_GAME_ "Tipologia di trabocchetto: " _YELLOW_ "caduta" _WHITE_);
                        prince_pass = false;
                    }else{
                        puts(_INFO_GAME_ "Tipologia di trabocchetto: " _YELLOW_ "caduta" _WHITE_);
                        if((rand() % 100)+1 <= 50){
                            player->p_vita -= 1;
                            puts(_INFO_GAME_R "Hai perso " _RED_ "1 " _WHITE_ "punto vita.");
                        }else{
                            player->p_vita -= 2;
                            puts(_INFO_GAME_R "Hai perso " _RED_ "2 " _WHITE_ "punti vita.");
                        }
                    }
                    break;
                case 4:
                    //il player perde anche il turno
                    if(prince_pass && player->classe_giocatore == principe){
                        puts(_INFO_GAME_ "Hai ignorato il primo trabocchetto trovato!");
                        puts(_INFO_GAME_ "Tipologia di trabocchetto: " _YELLOW_ "burrone" _WHITE_);
                        prince_pass = false;
                    }else{
                        puts(_INFO_GAME_ "Tipologia di trabocchetto: " _YELLOW_ "burrone" _WHITE_);
                        if((rand() % 100) <= 50){
                            player->p_vita -= 1;
                            puts(_INFO_GAME_R "Hai perso " _RED_ "1 " _WHITE_ "punto vita.");
                        }else{
                            player->p_vita -= 2;
                            puts(_INFO_GAME_R "Hai perso " _RED_ "2 " _WHITE_ "punti vita.");
                        }
                        morte_burrone = true;
                        lose_turno = true;
                    }
                    break;
            }
            
            //probabilita di far comparire un nemico 25% (60% s, 40% g)
            if((player->posizione) != last_room){
                if((rand() % 100) <= 25){
                    int apparizione_enemy = rand() % 100;
                    if(apparizione_enemy <= 60){
                        //scheletro
                        Enemy *scheletro = (Enemy*) malloc(sizeof(Enemy));
                        strcpy(scheletro->nome_nemico, "Scheletro");
                        scheletro->id = 1;
                        scheletro->p_vita = 1;
                        scheletro->dadi_attacco = 1;
                        scheletro->dadi_difesa = 1;
                        scheletro->scudi = (rand() %1)+1;
                        (player->posizione)->nemico = scheletro;
                        puts(_ATTENZIONE_ "E' comparso un nemico:" _RED_ " Scheletro" _WHITE_);
                    }else{
                        //guardia
                        Enemy *guardia = (Enemy*) malloc(sizeof(Enemy));
                        strcpy(guardia->nome_nemico, "Guardia");
                        guardia->id = 2;
                        guardia->p_vita = 2;
                        guardia->dadi_attacco = 2;
                        guardia->dadi_difesa = 2;
                        guardia->scudi = (rand() %2)+1;;
                        (player->posizione)->nemico = guardia;
                        puts(_ATTENZIONE_ "E' comparso un nemico:" _RED_ " Guardia" _WHITE_);
                    }   
                }
            }
        }else{
            if((player->posizione)->nemico == NULL){
                //siamo nell'ultima stanza e faccio apparire Jaffar
                Enemy *jaffar = (Enemy*) malloc(sizeof(Enemy));
                strcpy(jaffar->nome_nemico, "Jaffar");
                jaffar->id = 3;
                jaffar->p_vita = 3;
                jaffar->dadi_attacco = 3;
                jaffar->dadi_difesa = 2;
                jaffar->scudi = (rand() %2)+1;;
                (player->posizione)->nemico = jaffar;
                puts(_ATTENZIONE_ "E' comparso un nemico:" _RED_ " Jaffar" _WHITE_);
                print_jaffar = true;
            }
        }
        
        avanzato = false;
    }else{
        puts("Non puoi avanzare");
        avanzato = true;
    }

    return avanzato;
}

static void combatti(Giocatore *player, boolean *avanzamento){
    puts(_WHITE_ "\n[" _YELLOW_ "Combattimento" _WHITE_ "]:");
    if((player->posizione)->nemico != NULL){
        int danni_armi = 1;
        switch (player->weapon){
            case nessuna:
                danni_armi = 0;
                break;
            case pugnale:
                danni_armi = 1;
                break;
            case spada:
                danni_armi = 2;
                break;
            case frusta:
                danni_armi = 1;
                break;
            case doppie_lame:
                danni_armi = 2;
                break;
            case mazza:
                danni_armi = 3;
                break;
        }
        int scelta_azione[2] = {1, 1};
        scelta_azione[0] = (rand() % 6) +1;
        scelta_azione[1] = (rand() % 6) +1;
        unsigned int valore_attacco = 0;
        unsigned int valore_difesa = 0;

        if(scelta_azione[0] > scelta_azione[1]){
            printf(_INFO_GAME_ "Il giocatore " _YELLOW_ "%s" _WHITE_" ha il ruolo di " _YELLOW_ "attaccante"_WHITE_"!\n", player->nome_giocatore);
            printf(_INFO_GAME_ "Il nemico " _YELLOW_ "%s" _WHITE_" ha il ruolo di " _YELLOW_ "difensore"_WHITE_"!\n\n", ((player->posizione)->nemico)->nome_nemico);
            
            int risultati_attacco[player->dadi_attacco];
            printf(_INFO_GAME_ "Il giocatore ha lanciato " _YELLOW_ "%d" _WHITE_ " dadi! Risultati [ "_YELLOW_, player->dadi_attacco);
            for(int i=0; i<player->dadi_attacco; i++){
                risultati_attacco[i] = (rand() % 6) +1;
                printf("%d ", risultati_attacco[i]);
                if(risultati_attacco[i] > 3 && risultati_attacco[i] < 6){
                    //Se quando lanci il dado esce 4 o 5 ottieni un colpo sicuro
                    valore_attacco += 1;
                }else if(risultati_attacco[i] > 5){
                    //Se invece lanci il dado ed esce 6 allorea ottieni due colpi sicuri
                    valore_attacco += 2;
                }else{
                    //Qua stiamo in tutti i restanti casi, visto che non e' possibile fare un punteggio maggiore di 6 significa
                    //che l'attuale punteggio al momento e' minore di 4 ovvero 0 colpi sicuri
                    valore_attacco += 0;
                }
            }
            puts(_WHITE_ "]");
            printf(_INFO_GAME_ "Il nemico ha lanciato " _YELLOW_ "%d" _WHITE_ " dadi! Risultati: [ " _YELLOW_, ((player->posizione)->nemico)->dadi_difesa);
            int risultati_difesa[((player->posizione)->nemico)->dadi_difesa];
            for(int i=0; i<((player->posizione)->nemico)->dadi_difesa; i++){
                risultati_difesa[i] = (rand() % 6) +1;
                printf("%d ", risultati_difesa[i]);
                if(risultati_difesa[i] > 3 && risultati_difesa[i] < 6){
                    //Se quando lanci il dado esce 4 o 5 ottieni un colpo difesa sicuro
                    valore_difesa += 1;
                }else if(risultati_difesa[i] > 5){
                    //Se invece lanci il dado ed esce 6 allorea ottieni due colpi difesa sicuri (neutralizza 2 dadi)
                    int colpi = 0;
                    for(int j=0; j<player->dadi_attacco; j++){
                        if(risultati_attacco[j] > 3 && risultati_attacco[j] < 6){
                            if(colpi <= 2){
                                valore_difesa += 1;
                                risultati_attacco[j] = 0;
                                colpi++;
                            }
                        }else if(risultati_attacco[j] > 5){
                            if(colpi <= 2){
                                valore_difesa += 2;
                                risultati_attacco[j] = 0;
                                colpi++;
                            }
                        }
                    }
                }else{
                    //Qua stiamo in tutti i restanti casi, visto che non e' possibile fare un punteggio maggiore di 6 significa
                    //che l'attuale punteggio al momento e' minore di 4 ovvero 0 colpi difesa sicuri
                    valore_difesa += 0;
                }
            }
            puts(_WHITE_ "]\n");
            if(danni_armi > 0){
                valore_attacco += danni_armi;
                puts(_INFO_GAME_G "Hai ricevuto un poteziamento grazie all'arma.");
                if(player->posizione->nemico->scudi > 0){
                    valore_difesa += (rand() % danni_armi);
                    puts(_INFO_GAME_G "Il nemico aveva uno scudo che ha assorbito alcuni danni.");
                    player->posizione->nemico->scudi--;
                }else{
                    player->posizione->nemico->scudi = 0;
                }
            }
            //Buff ulteriore
            //controllo quale parte vince o meno
            if(valore_attacco > valore_difesa){
                //il difensore subisce tutti danni che non para
                printf(_INFO_GAME_R "Il giocatore ha inferto " _RED_ "%d" _WHITE_ " danni, mentre il nemico ha ottenuto "_RED_"%d" _WHITE_" danni neutralizzati\n", valore_attacco, valore_difesa);
                valore_attacco -= valore_difesa;
                printf(_INFO_GAME_R "Il nemico ha subito "_RED_"%d"_WHITE_" danni", valore_attacco);
            }else if(valore_attacco < valore_difesa){
                //l'attacante non tira colpi sicuri quindi il difensore para tutti i colpi
                printf(_INFO_GAME_R "Il giocatore ha inferto " _RED_ "%d" _WHITE_ " danni, mentre il nemico ha ottenuto "_RED_"%d" _WHITE_" danni neutralizzati\n", valore_attacco, valore_difesa);
                valore_difesa -= valore_attacco;
                printf(_INFO_GAME_R "Il nemico non ha subito, ha neutralizzato tutti i colpi, [n. colpi che poteva ancora neutralizzare "_RED_"%d"_WHITE_"]", valore_difesa);
            }else{
                //il valore dei colpi e delle parate sono uguali nessuno subisce penalita
                printf(_INFO_GAME_R "Il giocatore ha inferto " _RED_ "%d" _WHITE_ " danni, mentre il nemico ha ottenuto "_RED_"%d" _WHITE_" danni neutralizzati\n", valore_attacco, valore_difesa);
                printf(_INFO_GAME_R "Il nemico non ha subito colpi, ha neutralizzato tutti i colpi");
                valore_attacco = 0;
            }
            //Controllo se il nemico ha subito danni e vedo se e' morto
            //la definizione CONTROLLO_VITA_NEMICI indica il valore che la vita del nemico non puo' mai raggiungere
            ((player->posizione)->nemico)->p_vita -= valore_attacco;
            if(((player->posizione)->nemico)->p_vita < 1 || ((player->posizione)->nemico)->p_vita > CONTROLLO_VITA_NEMICI){
                printf("\n\n" _INFO_GAME_ "Il nemico "_RED_ "%s" _WHITE_ " e' stato sconfitto!", ((player->posizione)->nemico)->nome_nemico);
                player->p_vita += 1;
                printf("\n" _INFO_GAME_ "Hai recuperato" _YELLOW_ " 1" _WHITE_ " punto vita!");
                sconfitta_jaffar = ((player->posizione)->nemico)->id == 3 ? true : false;
                free((player->posizione)->nemico);
                (player->posizione)->nemico = NULL;   
            }
        }else{
            printf(_INFO_GAME_ "Il nemico " _YELLOW_ "%s" _WHITE_" ha il ruolo di " _YELLOW_ "attaccante"_WHITE_"!\n", ((player->posizione)->nemico)->nome_nemico);
            printf(_INFO_GAME_ "Il giocatore " _YELLOW_ "%s" _WHITE_" ha il ruolo di " _YELLOW_ "difensore"_WHITE_"!\n\n", player->nome_giocatore);
            int risultati_attacco[((player->posizione)->nemico)->dadi_attacco];
            printf(_INFO_GAME_ "Il nemico ha lanciato " _YELLOW_ "%d" _WHITE_ " dadi! Risultati [ "_YELLOW_, ((player->posizione)->nemico)->dadi_attacco);
            for(int i=0; i<((player->posizione)->nemico)->dadi_attacco; i++){
                risultati_attacco[i] = (rand() % 6) +1;
                printf("%d ", risultati_attacco[i]);
                if(risultati_attacco[i] > 3 && risultati_attacco[i] < 6){
                    //Se quando lanci il dado esce 4 o 5 ottieni un colpo sicuro
                    valore_attacco += 1;
                }else if(risultati_attacco[i] > 5){
                    //Se invece lanci il dado ed esce 6 allorea ottieni due colpi sicuri
                    valore_attacco += 2;
                }else{
                    //Qua stiamo in tutti i restanti casi, visto che non e' possibile fare un punteggio maggiore di 6 significa
                    //che l'attuale punteggio al momento e' minore di 4 ovvero 0 colpi sicuri
                    valore_attacco += 0;
                }
            }
            puts(_WHITE_ "]");
            printf(_INFO_GAME_ "Il nemico ha lanciato " _YELLOW_ "%d" _WHITE_ " dadi! Risultati: [ " _YELLOW_, player->dadi_difesa);
            int risultati_difesa[player->dadi_difesa];
            for(int i=0; i<player->dadi_difesa; i++){
                risultati_difesa[i] = (rand() % 6) +1;
                printf("%d ", risultati_difesa[i]);
                if(risultati_difesa[i] > 3 && risultati_difesa[i] < 6){
                    //Se quando lanci il dado esce 4 o 5 ottieni un colpo difesa sicuro
                    valore_difesa += 1;
                }else if(risultati_difesa[i] > 5){
                    //Se invece lanci il dado ed esce 6 allorea ottieni due colpi difesa sicuri (neutralizza 2 dadi)
                    int colpi = 0;
                    for(int j=0; j<player->dadi_attacco; j++){
                        if(risultati_attacco[j] > 3 && risultati_attacco[j] < 6){
                            if(colpi <= 2){
                                valore_difesa += 1;
                                risultati_attacco[j] = 0;
                                colpi++;
                            }
                        }else if(risultati_attacco[j] > 5){
                            if(colpi <= 2){
                                valore_difesa += 2;
                                risultati_attacco[j] = 0;
                                colpi++;
                            }
                        }
                    }
                }else{
                    //Qua stiamo in tutti i restanti casi, visto che non e' possibile fare un punteggio maggiore di 6 significa
                    //che l'attuale punteggio al momento e' minore di 4 ovvero 0 colpi difesa sicuri
                    valore_difesa += 0;
                }
            }
            puts(_WHITE_ "]\n");
            //controllo quale parte vince o meno
            if(valore_attacco > valore_difesa){
                //il difensore subisce tutti danni che non para
                printf(_INFO_GAME_R "Il nemico ha inferto " _RED_ "%d" _WHITE_ " danni, mentre il giocatore ha ottenuto "_RED_"%d" _WHITE_" danni neutralizzati\n", valore_attacco, valore_difesa);
                valore_attacco -= valore_difesa;
                printf(_INFO_GAME_R "Il giocatore ha subito "_RED_"%d"_WHITE_" danni", valore_attacco);
            }else if(valore_attacco < valore_difesa){
                //l'attacante non tira colpi sicuri quindi il difensore para tutti i colpi
                printf(_INFO_GAME_R "Il nemico ha inferto " _RED_ "%d" _WHITE_ " danni, mentre il giocatore ha ottenuto "_RED_"%d" _WHITE_" danni neutralizzati\n", valore_attacco, valore_difesa);
                valore_difesa -= valore_attacco;
                printf(_INFO_GAME_R "Il giocatore non ha subito, ha neutralizzato tutti i colpi, [n. colpi che poteva ancora neutralizzare "_RED_"%d"_WHITE_"]", valore_difesa);
            }else{
                //il valore dei colpi e delle parate sono uguali nessuno subisce penalita
                printf(_INFO_GAME_R "Il nemico ha inferto " _RED_ "%d" _WHITE_ " danni, mentre il giocatore ha ottenuto "_RED_"%d" _WHITE_" danni neutralizzati\n", valore_attacco, valore_difesa);
                printf(_INFO_GAME_R "Il giocatore non ha subito, ha neutralizzato tutti i colpi");
                valore_attacco = 0;
            }

            player->p_vita -= valore_attacco;
            if(player->p_vita < 1 || player->p_vita > CONTROLLO_VITA_GIOCATORI){
                printf("\n\n" _INFO_GAME_ "Il giocatore "_RED_ "%s" _WHITE_ " e' stato sconfitto!\n", player->nome_giocatore);
                death_player(player);
                *avanzamento = true;
            }
        }
        puts("");
    }else{
        puts(_INFO_GAME_R "Non e' presente nessun nemico! Non puoi combattere da solo.");
    }
}

static void stampa_zona(Stanza *room){
    puts(_WHITE_ "\n[" _YELLOW_ "Informazioni della zona" _WHITE_ "]:");
    printf(_INFO_GAME_ "Stanza: " _YELLOW_ "%p\n" _WHITE_, room);
    switch (room->stanza){
        case 0:
            puts(_INFO_GAME_ "Tipologia della stanza: " _YELLOW_ "corridoio" _WHITE_);
            break;
        case 1:
            puts(_INFO_GAME_ "Tipologia della stanza: " _YELLOW_ "scala" _WHITE_);
            break;
        case 2:
            puts(_INFO_GAME_ "Tipologia della stanza: " _YELLOW_ "sala banchetto" _WHITE_);
            break;
        case 3:
            puts(_INFO_GAME_ "Tipologia della stanza: " _YELLOW_ "magazzino" _WHITE_);
            break;
        case 4:
            puts(_INFO_GAME_ "Tipologia della stanza: " _YELLOW_ "posto guardia" _WHITE_);
            break;
        case 5:
            puts(_INFO_GAME_ "Tipologia della stanza: " _YELLOW_ "prigione" _WHITE_);
            break;
        case 6:
            puts(_INFO_GAME_ "Tipologia della stanza: " _YELLOW_ "armeria" _WHITE_);
            break;
        case 7:
            puts(_INFO_GAME_ "Tipologia della stanza: " _YELLOW_ "moschea" _WHITE_);
            break;
        case 8:
            puts(_INFO_GAME_ "Tipologia della stanza: " _YELLOW_ "torre" _WHITE_);
            break;
        case 9:
            puts(_INFO_GAME_ "Tipologia della stanza: " _YELLOW_ "bagni" _WHITE_);
            break;
    }
    if(room->nemico != NULL){
        printf(_INFO_GAME_R "E' presente un nemico: " _RED_ "%s\n" _WHITE_, (room->nemico)->nome_nemico);
    }else{
        puts(_INFO_GAME_ "Non e' presente nessun nemico.");
    }

    if(room->tesoro > 0){
        puts(_INFO_GAME_ "E' presente un tesoro nella stanza");
    }else{
        puts(_INFO_GAME_ "Non e' presente nessun tesoro nella stanza");
    }

    switch (room->trabocchetto){
        case 0:
            puts(_INFO_GAME_ "Tipologia di trabocchetto: " _YELLOW_ "nessuno" _WHITE_);
            break;
        case 1:
            puts(_INFO_GAME_ "Tipologia di trabocchetto: " _YELLOW_ "tegola" _WHITE_);
            break;
        case 2:
            puts(_INFO_GAME_ "Tipologia di trabocchetto: " _YELLOW_ "lame" _WHITE_);
            break;
        case 3:
            puts(_INFO_GAME_ "Tipologia di trabocchetto: " _YELLOW_ "caduta" _WHITE_);
            break;
        case 4:
            puts(_INFO_GAME_ "Tipologia di trabocchetto: " _YELLOW_ "burrone" _WHITE_);
            break;
    }
}

static void stampa_giocatore(Giocatore *player){
    puts(_WHITE_ "\n[" _YELLOW_ "Informazioni del giocatore" _WHITE_ "]:");
    printf(_INFO_GAME_ "Nome: " _YELLOW_ "%s\n" _WHITE_, player->nome_giocatore);
    switch (player->classe_giocatore){
        case 0:
            puts(_INFO_GAME_ "Classe: " _YELLOW_ "principe" _WHITE_ );
            break;
        case 1:
            puts(_INFO_GAME_ "Classe: " _YELLOW_ "doppleganger" _WHITE_ );
            break;
    }
    printf(_INFO_GAME_ "Posizione: " _YELLOW_ "%p\n" _WHITE_, player->posizione);
    printf(_INFO_GAME_ "Vita massima:" _YELLOW_ " %d\n" _WHITE_, player->p_vita_max);
    printf(_INFO_GAME_ "Vita attuale:" _YELLOW_ " %d\n" _WHITE_, player->p_vita);
    printf(_INFO_GAME_ "Dadi attacco:" _YELLOW_ " %d\n" _WHITE_, player->dadi_attacco);
    printf(_INFO_GAME_ "Dadi difesa:" _YELLOW_ " %d\n" _WHITE_, player->dadi_difesa);
    printf(_INFO_GAME_ "N. volte per scappare:" _YELLOW_ " %d\n" _WHITE_, player->volte_scappa);
    printf(_INFO_GAME_ "Monete:" _YELLOW_ " %d\n" _WHITE_, player->monete);
    switch (player->weapon){
        case nessuna:
            printf(_INFO_GAME_ "Arma:" _YELLOW_ " nessuna\n" _WHITE_);
            break;
        case pugnale:
            printf(_INFO_GAME_ "Arma:" _YELLOW_ " pugnale\n" _WHITE_);
            break;
        case spada:
            printf(_INFO_GAME_ "Arma:" _YELLOW_ " spada\n" _WHITE_);
            break;
        case frusta:
            printf(_INFO_GAME_ "Arma:" _YELLOW_ " frusta\n" _WHITE_);
            break;
        case doppie_lame:
            printf(_INFO_GAME_ "Arma:" _YELLOW_ " doppie lame\n" _WHITE_);
            break;
        case mazza:
            printf(_INFO_GAME_ "Arma:" _YELLOW_ " mazza\n" _WHITE_);
            break;
    }
}

static boolean cerca_porta_segreta(Stanza *room, Giocatore *player){
    boolean risultato = true;
    boolean direzioni[4] = {false,false,false,false};
    direzioni[0] = (room->stanza_destra != NULL) ? true : false;
    direzioni[1] = (room->stanza_sinistra != NULL) ? true : false;
    direzioni[2] = (room->stanza_sopra != NULL) ? true : false;
    direzioni[3] = (room->stanza_sotto != NULL) ? true : false;
    
    int turno = 0;
    int prob = 33;

    for(int i=0; i<4; i++){
        if(!direzioni[i]){
            if((rand() % 100) < prob){
                Stanza *segreta = malloc(sizeof(Stanza));
                if(segreta == NULL){
                    puts(_ERROR_ "Non e' possibile creare la stanza segreta.");
                }else{
                    puts("\n");
                    switch (i){
                        case 0:
                            puts(_INFO_GAME_ "Hai trovato una stanza segreta a " _YELLOW_ "destra" _WHITE_);
                            break;
                        case 1:
                            puts(_INFO_GAME_ "Hai trovato una stanza segreta a " _YELLOW_ "sinistra" _WHITE_);
                            break;
                        case 2:
                            puts(_INFO_GAME_ "Hai trovato una stanza segreta a " _YELLOW_ "sopra" _WHITE_);
                            break;
                        case 3:
                            puts(_INFO_GAME_ "Hai trovato una stanza segreta a " _YELLOW_ "sotto" _WHITE_);
                            break;
                    }
                    
                    segreta->precedente = room;
                    segreta->nemico = NULL;
                    insert_info_random(segreta);
                    player->posizione = segreta;

                    stampa_zona(player->posizione);
                    if(segreta->tesoro != nessun_tesoro){
                        puts("\n"_INFO_GAME_ "Vuoi prendere il tesoro presente nella stanza segreta?\n"_INFO_GAME_"(1 prendi testo, 2 non prendi tesoro)");
                        puts("");
                        int prendi_tesoro_segreto = input_scan(1,2,"Inserisci " _YELLOW_ "> " _WHITE_);
                        if(prendi_tesoro_segreto == 1){
                            prendi_tesoro(player);
                        }else{
                            puts("Non hai preso il tesoro");
                        }
                        
                    }
                    
                    puts("\n" _INFO_GAME_ "Sei tornato nella stanza precedente.");
                    player->posizione = (player->posizione)->precedente;
                    free(segreta);
                    risultato = false;
                }
                break;
            }
            prob = turno == 0 ? 33 : (turno == 1) ? 20 : 15;
            turno ++;
        }
        if(i == 3){
            puts("\n" _INFO_GAME_ "Non e' stata travata nessuna stanza segreta.");
        }
    }

    return risultato;
}

static void prendi_tesoro(Giocatore *player){
    int monete_trovate = 0;
    puts(_WHITE_ "\n[" _YELLOW_ "Tesoro" _WHITE_ "]:");
    switch ((player->posizione)->tesoro){
        case 0:
            puts(_INFO_GAME_ "Non e' presente nessun tesoro nella stanza!");
            break;
        case 1:
            puts(_INFO_GAME_ "Tipologia di tesoro: " _YELLOW_ "verde veleno" _WHITE_);
            puts(_INFO_GAME_ "La tua vita e' diminuita di 1 punto.");
            player->p_vita -= 1;
            (player->posizione)->tesoro = nessun_tesoro;
            break;
        case 2:
            puts(_INFO_GAME_ "Tipologia di tesoro: " _YELLOW_ "blu guarigione" _WHITE_);
            if((player->p_vita) + 1 <= player->p_vita_max){
                puts(_INFO_GAME_ "La tua vita e' aumentata di 1 punto.");
                player->p_vita += 1;
            }else{
                puts(_INFO_GAME_ "Hai il numero massimo di vite, non e' possibile aumentarla");  
            }
            monete_trovate = (rand() % 2)+1;
            player->monete += monete_trovate;
            printf(_INFO_GAME_G "Hai trovato " _GREEN_ "%d" _WHITE_ " %s!\n", monete_trovate, monete_trovate != 1 ? "monete" : "moneta");
            (player->posizione)->tesoro = nessun_tesoro;
            break;
        case 3:
            puts(_INFO_GAME_ "Tipologia di tesoro: " _YELLOW_ "rosso aumenta vita" _WHITE_);
            player->p_vita_max += 1;
            player->p_vita = player->p_vita_max;
            puts(_INFO_GAME_ "La vita massima e' stata aumentata di 1 punto, hai recuperato tutta la vita!");
            monete_trovate = (rand() % 5)+1;
            player->monete += monete_trovate;
            printf(_INFO_GAME_G "Hai trovato " _GREEN_ "%d" _WHITE_ " %s!\n", monete_trovate, monete_trovate != 1 ? "monete" : "moneta");
            (player->posizione)->tesoro = nessun_tesoro;
            break;
        case 4:
            puts(_INFO_GAME_ "Tipologia di tesoro: " _YELLOW_ "spada tagliente" _WHITE_);
            puts(_INFO_GAME_ "Hai aumentato di 1 il numero di dadi attacco.");
            player->dadi_attacco += 1;
            monete_trovate = (rand() % 1)+1;
            player->monete += monete_trovate;
            printf(_INFO_GAME_G "Hai trovato " _GREEN_ "%d" _WHITE_ " %s!\n", monete_trovate, monete_trovate != 1 ? "monete" : "moneta");
            (player->posizione)->tesoro = nessun_tesoro;
            break;
        case 5:
            puts(_INFO_GAME_ "Tipologia di tesoro: " _YELLOW_ "scudo" _WHITE_);
            puts(_INFO_GAME_ "Hai aumentato di 1 il numero di dadi difesa.");
            player->dadi_difesa += 1;
            monete_trovate = (rand() % 2)+1;
            player->monete += monete_trovate;
            printf(_INFO_GAME_G "Hai trovato " _GREEN_ "%d" _WHITE_ " %s!\n", monete_trovate, monete_trovate != 1 ? "monete" : "moneta");
            (player->posizione)->tesoro = nessun_tesoro;
            break;
    }
}

static int passa(int value, Giocatore *player){
    printf("\n"_INFO_GAME_ "Hai passato il turno al prossimo giocatore!");
    value++;
    if((player->posizione)->nemico != NULL){
        if(((player->posizione)->nemico)->id != 3){
            free((player->posizione)->nemico);
            (player->posizione)->nemico = NULL;
            printf("\n"_INFO_GAME_R "Il nemico che era comparso ora e' sparito!");
        }
    }
    puts("");
    return value;
}

static void scappa(Giocatore *player){
    puts("");
    if(player != NULL){
        if((player->posizione)->nemico == NULL){
            puts(_INFO_GAME_ "Non e' presente nessun nemico nella stanza, non puoi scappare");
        }else{
            if(player->volte_scappa < 1){
                player->volte_scappa = 0;
                puts(_INFO_GAME_R "Non puoi piu' scappare!");
            }else{
                player->volte_scappa -= 1;
                printf(_INFO_GAME_ "Sei scappato! Puoi scappare ancora " _YELLOW_ "%d" _WHITE_ " %s.\n", player->volte_scappa, player->volte_scappa == 1 ? "volta" : "volte");
                Stanza *temp = player->posizione;
                player->posizione = (player->posizione)->precedente;
                printf(_INFO_GAME_ "Sei ritornato nella stanza precedente "_YELLOW_ "%p\n" _WHITE_, player->posizione);
                player->posizione = temp;
                printf(_TAB_ "per poi tornare nella stanza dove era presente il nemico " _YELLOW_"%p\n" _WHITE_, (player->posizione));

                if(((player->posizione)->nemico)->id != 3){
                    free((player->posizione)->nemico);
                    (player->posizione)->nemico = NULL;
                    printf("\n"_INFO_GAME_R "Il nemico che era presente nella stanza non c'e' piu'.\n");
                }else{
                    printf("\n"_INFO_GAME_R "Hai provato a scappare da Jaffar, ma lui rimarra' sempre nella stanza!\n");
                }
                
                if(player->posizione->trabocchetto != nessuno){
                    puts(_INFO_GAME_ "Hai gia' subito gli effetti di questo trabocchetto in questa stanza.\n" _TAB_ "Non subirai lo stesso effetto un'altra volta! (valido per la stanza attuale)");
                }
            }
        }
    }else{
        puts(_ERROR_ "Player invalido.");
    }
}

static void death_player(Giocatore *player){

    for(int i=0; i<num_player_in_game; i++){
        if(player == giocatori[i]){
            free(giocatori[i]);
            giocatori[i] = NULL;
        }
    }
    int indice = 0;
    for(int i=0; i<num_player_in_game; i++){
        if(giocatori[i] != NULL){
            giocatori[indice++] = giocatori[i];
        }   
    }

    for (int i = indice; i < num_player_in_game; i++) {
        giocatori[i] = NULL;
    }

    num_player_in_game--;
}

boolean termina_gioco(){
    print_title("Termina Gioco", _CYAN_);
    
    if(first_name != NULL){
        All_name *name_scan = first_name;
        puts("\nUn saluto a tutti i giocatori che hanno partecipato al gioco:\n");
        do{
            printf(_CYAN_ " > " _WHITE_ "%s\n", name_scan->name);
            name_scan = name_scan->next_name;
        }while(name_scan != NULL);
        name_scan = first_name;
        do{
            All_name *temp = name_scan->next_name;
            free(name_scan);
            name_scan = temp;
        }while(name_scan != NULL);
    }else{
        puts("\n" _CYAN_ " > " _WHITE_ "Nessuno ha giocato al gioco :(");
    }
    
    puts("");
    return false;
}

static void apparizione_mercante(Giocatore *player){
    if((rand() % 100) < 15){
        Mercante *nuovo_mercante = malloc(sizeof(Mercante));
        if(nuovo_mercante == NULL){
            puts(_ERROR_ "Non e' possibile creare un nuovo mercante");
        }else{
            puts(_GREEN_ "ATTENZIONE" _WHITE_ ": Hai trovato un mercante!");
            strcpy(nuovo_mercante->name, rand() % 2 ? "Saraya la Venditrice" : "Bahram il Commerciante");
            for(int i=0; i<3; i++){
                nuovo_mercante->catalogo_armi[i] = (rand() % 6)+1;
                nuovo_mercante->prezzo[i] = (rand() % 14)+5;
            }
            nuovo_mercante->catalogo_tesoro[0] = (rand() % 4) + 2;
            nuovo_mercante->prezzo[3] = (rand() % 19)+2;
            printf("\n" _INFO_GAME_ _YELLOW_ "[" _WHITE_ "%s" _YELLOW_ "]" _WHITE_ "\n", nuovo_mercante->name);
            puts(_INFO_GAME_ "Catalogo Armi:");
            for(int i=0; i<3; i++){
                switch (nuovo_mercante ->catalogo_armi[i]){
                    case nessuna:
                        printf(_TAB_ _INFO_GAME_ "Assolutamente niente, una semplice donazione.   Prezzo: %d\n", nuovo_mercante->prezzo[i]);
                        break;
                    case pugnale:
                        printf(_TAB_ _INFO_GAME_ "Pugnale   Prezzo: %d\n", nuovo_mercante->prezzo[i]);
                        break;
                    case spada:
                        printf(_TAB_ _INFO_GAME_ "Spada   Prezzo: %d\n", nuovo_mercante->prezzo[i]);
                        break;
                    case frusta:
                        printf(_TAB_ _INFO_GAME_ "Frusta   Prezzo: %d\n", nuovo_mercante->prezzo[i]);
                        break;
                    case doppie_lame:
                        printf(_TAB_ _INFO_GAME_ "Doppie lame   Prezzo: %d\n", nuovo_mercante->prezzo[i]);
                        break;
                    case mazza:
                        printf(_TAB_ _INFO_GAME_ "Mazza   Prezzo: %d\n", nuovo_mercante->prezzo[i]);
                        break;
                }
            }
            puts(_INFO_GAME_ "Catalogo tesori:");
            switch (nuovo_mercante ->catalogo_tesoro[0]){
                case 0:
                    puts(_TAB_ _INFO_GAME_ "Niente mi dispiace");
                    break;
                case 1:
                    puts(_TAB_ _INFO_GAME_ "Niente mi dispiace");
                    break;
                case 2:
                    printf(_TAB_ _INFO_GAME_ "blu guarigione   Prezzo: %d\n", nuovo_mercante->prezzo[3]);
                    break;
                case 3:
                    printf(_TAB_ _INFO_GAME_ "rosso aumenta vita   Prezzo: %d\n", nuovo_mercante->prezzo[3]);
                    break;
                case 4:
                    printf(_TAB_ _INFO_GAME_ "spada tagliente   Prezzo: %d\n", nuovo_mercante->prezzo[3]);
                    break;
                case 5:
                    printf(_TAB_ _INFO_GAME_ "scudo   Prezzo: %d\n", nuovo_mercante->prezzo[3]);
                    break;
            }
            printf("\n" _ATTENZIONE_ "Puoi acquistare solo un oggetto! Monete a tua disposizione: " _GREEN_ "%d" _WHITE_ "\n\n", player->monete);
            puts("Inserisci un valore da 1 a 3 per scegliere la tua arma, 4 per il tesoro, 5 per non comprare niente!");
            int choice = input_scan(1, 5, "Inserisci" _YELLOW_ " > " _WHITE_);
            switch (choice){
                case 1:
                    if(nuovo_mercante->prezzo[0] <= player->monete){
                        puts(_INFO_GAME_ "Hai scelto l'arma n. uno!");
                        player->weapon = nuovo_mercante->catalogo_armi[0];
                        player->monete -= nuovo_mercante->prezzo[0];
                    }else{
                        puts(_INFO_GAME_ "Mi dispiace, il mercante non fa affari con chi non ha monete");
                    }
                    break;
                case 2:
                    if(nuovo_mercante->prezzo[1] <= player->monete){
                        puts(_INFO_GAME_ "Hai scelto l'arma n. due!");
                        player->weapon = nuovo_mercante->catalogo_armi[1];
                        player->monete -= nuovo_mercante->prezzo[1];
                    }else{
                        puts(_INFO_GAME_ "Mi dispiace, il mercante non fa affari con chi non ha monete");
                    }
                    break;
                case 3:
                    if(nuovo_mercante->prezzo[2] <= player->monete){
                        puts(_INFO_GAME_ "Hai scelto l'arma n. due!");
                        player->weapon = nuovo_mercante->catalogo_armi[2];
                        player->monete -= nuovo_mercante->prezzo[2];
                    }else{
                        puts(_INFO_GAME_ "Mi dispiace, il mercante non fa affari con chi non ha monete");
                    }
                    break;
                case 4:
                    if(nuovo_mercante->prezzo[3] <= player->monete){
                        puts(_INFO_GAME_ "Hai scelto il tesoro!");
                        switch (nuovo_mercante->catalogo_tesoro[3]){
                            case 0:
                                puts(_INFO_GAME_ "Nessun tesoro!");
                                break;
                            case 1:
                                puts(_INFO_GAME_ "Nessun tesoro!");
                                break;
                            case 2:
                                puts(_INFO_GAME_ "Tipologia di tesoro: " _YELLOW_ "blu guarigione" _WHITE_);
                                if((player->p_vita) + 1 <= player->p_vita_max){
                                    puts(_INFO_GAME_ "La tua vita e' aumentata di 1 punto.");
                                    player->p_vita += 1;
                                }else{
                                    puts(_INFO_GAME_ "Hai il numero massimo di vite, non e' possibile aumentarla");  
                                }
                                break;
                            case 3:
                                puts(_INFO_GAME_ "Tipologia di tesoro: " _YELLOW_ "rosso aumenta vita" _WHITE_);
                                player->p_vita_max += 1;
                                player->p_vita = player->p_vita_max;
                                puts(_INFO_GAME_ "La vita massima e' stata aumentata di 1 punto, hai recuperato tutta la vita!");
                                break;
                            case 4:
                                puts(_INFO_GAME_ "Tipologia di tesoro: " _YELLOW_ "spada tagliente" _WHITE_);
                                puts(_INFO_GAME_ "Hai aumentato di 1 il numero di dadi attacco.");
                                player->dadi_attacco += 1;
                                break;
                            case 5:
                                puts(_INFO_GAME_ "Tipologia di tesoro: " _YELLOW_ "scudo" _WHITE_);
                                puts(_INFO_GAME_ "Hai aumentato di 1 il numero di dadi difesa.");
                                player->dadi_difesa += 1;
                                break;
                        }
                        player->monete -= nuovo_mercante->prezzo[3];
                    }else{
                        puts(_INFO_GAME_ "Mi dispiace, il mercante non fa affari con chi non ha monete");
                    }
                    break;
                case 5:
                    puts(_INFO_GAME_R "Il Mercante ti ha insultato per non aver comprato niente!");
                    if(player->weapon != nessuna){
                        int monete_rubate = (rand() % 2)+1;
                        if(monete_rubate <= player->monete){
                            printf(_INFO_GAME_R "Ha rubato la tua arma e' %d %s\n", player->monete, player->monete != 1 ? "monete" : "moneta");
                            player->monete -= monete_rubate;
                        }else{
                            puts(_INFO_GAME_R "Ha rubato la tua arma");
                        }
                        
                    }
                    break;
            }

            puts(_INFO_GAME_ "Il mercante e' fuggito!");
            free(nuovo_mercante);
        }
    }
    
}

boolean print_title(char *title, const char *color){
    boolean error = false;
    int length = strlen(_LINE_)/2 - strlen(title)/2 -1;
    char *num_space = calloc(1,length);

    if(num_space == NULL){
        puts(_ERROR_ "non e' stato possibile allocare memoria! (Spazio insufficente).");
        error = true;
    }

    for(int i=0; i<length; i++){
        num_space[i] = ' ';
    }

    puts(_WHITE_ _SCREEN_BELOW _LINE_);
    printf("%s%s[%s%s%s]%s\n", num_space, color, _WHITE_, title, color, _WHITE_);
    puts(_LINE_);

    free(num_space);

    return error;
}

static void name_players_member(Giocatore *player){
    All_name *new_name = malloc(sizeof(All_name));
    if(new_name == NULL){
        puts(_ERROR_ "Non e' possibile creare un nuovo nome");
    }else{
        strcpy(new_name->name, player->nome_giocatore);
        new_name->next_name = NULL;
        if(first_name == NULL){
            first_name = new_name;
            last_name = new_name;
        }else{
            last_name->next_name = new_name;
            last_name = new_name;
        }
    }
}

int input_scan(int limit_min, int limit_max, const char* command){
    int chose_options = 0;
    boolean error = false;
    
    do{
        printf("%s", command);
        int check_scan = scanf("%d", &chose_options); 
        // la funzione scanf restituisce 1 se legge in input un numero, 0 se non legge in input un numero
        if(check_scan != 1){
            puts(_ERROR_ "hai inserito un carattere al posto di un numero!");
            while (getchar() != '\n');
            error = true;
        }else{
            error = (chose_options < limit_min || chose_options > limit_max) ? true : false;
            if(error){
                puts(_ERROR_ "hai inserito un comando non valido! Riprova");
            }
        }
    }while(error);
    while (getchar() != '\n');

    return chose_options;
}

static void idle_menu(char *content, char *color){
    printf("\n%s%s" _WHITE_, color, content);
    char *temp = malloc(sizeof(char));
    fgets(temp, 0, stdin);
    while(getchar() != '\n');
    free(temp);
    puts("");
}

static void insert_info_random(Stanza *room){
    room->stanza = rand() % 10; //genero casualmente il tipo di stanza [0-9]

    int num_gen_trab = rand() % 100; //genero casualmente il tipo di trabocchetto, 65% niente, i restanti in ordine
                                //10%,9%,8%,8%
    if(num_gen_trab < 65){
        room->trabocchetto = nessuno;
    }else if(num_gen_trab < 75){
        room->trabocchetto = tegola;
    }else if(num_gen_trab < 84){
        room->trabocchetto = lame;
    }else if(num_gen_trab < 92){
        room->trabocchetto = caduta;
    }else{
        room->trabocchetto = burrone;
    }

    int num_gen_tes = rand() % 100; //genero casualmente il tipo di trabocchetto, 20% niente, i restanti in ordine
                                //20%,20%,15%,15%,10%
    if(num_gen_tes < 20){
        room->tesoro = nessun_tesoro;
    }else if(num_gen_tes < 40){
        room->tesoro = verde_veleno;
    }else if(num_gen_tes < 60){
        room->tesoro = blu_guarigione;
    }else if(num_gen_tes < 75){
        room->tesoro = rosso_aumenta_vita;
    }else if(num_gen_tes < 90){
        room->tesoro = spada_tagliente;
    }else{
        room->tesoro = scudo;
    }
}

boolean crediti(){
    boolean risultato = true;
    print_title("Crediti", _MAGENTA_);
    puts(_INFO_CREDITI_ "Creatore del gioco: " _MAGENTA_ "Sandu Davide Filippo" _WHITE_);

    puts("\nVincitori delle ultime tre partite: ");
    for(int i=0; i<3; i++){
        if(last_player_game[i] != NULL){
            printf(_INFO_CREDITI_ "%s\n", last_player_game[i]->nome_giocatore);
        }else{
            puts(_INFO_CREDITI_ "Nessun giocatore a vinto la partita");
        }
    }

    idle_menu("Premi invio per tornare al menu...", _MAGENTA_);

    return risultato;
}