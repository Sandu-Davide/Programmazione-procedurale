// Dichiarazione delle funzioni da chiamare in main.c
// Definizione dei tipi necessari alla libreria

#define _RED_     "\033[31m"  
#define _GREEN_   "\033[32m"
#define _YELLOW_  "\033[33m"
#define _BLUE_    "\033[34m" 
#define _MAGENTA_ "\033[35m" 
#define _CYAN_    "\033[36m" 
#define _WHITE_   "\033[37m"

#define _SCREEN_BELOW "\e[1;1H\e[2J"

#define _ERROR_ _RED_"ERRORE" _WHITE_ ": "
#define _ATTENZIONE_ _RED_"ATTENZIONE" _WHITE_ ": "
#define _MENU_ _YELLOW_"Menu > "_WHITE_
#define _LINE_ "--------------------------------------------"

typedef enum Tipo_giocatore {principe, doppleganger} Tipo_giocatore;
typedef enum Tipo_stanza {corridoio, scala, sala_banchetto, magazzino, posto_guardia, prigione, armeria, moschea, torre, bagni} Tipo_stanza;
typedef enum Tipo_trabocchetto {nessuno, tegola, lame, caduta, burrone} Tipo_trabocchetto;
typedef enum Tipo_tesoro {nessun_tesoro, verde_veleno, blu_guarigione, rosso_aumenta_vita, spada_tagliente, scudo} Tipo_tesoro;
typedef enum Tipo_arma {nessuna = 1, pugnale, spada, frusta, doppie_lame, mazza} Tipo_arma;

typedef enum {false, true} boolean;

struct Enemy{
    char nome_nemico[25];
    unsigned char id;
    unsigned char p_vita;
    unsigned char dadi_attacco;
    unsigned char dadi_difesa;
    unsigned char scudi;
};

struct Mercante{
    char name[25];
    Tipo_arma catalogo_armi[3];
    Tipo_tesoro catalogo_tesoro[1];
    unsigned char prezzo[4];
};

struct Stanza{
    struct Stanza* stanza_destra;
    struct Stanza* stanza_sinistra; 
    struct Stanza* stanza_sopra; 
    struct Stanza* stanza_sotto; 
    struct Stanza* precedente;
    struct Enemy *nemico;
    Tipo_stanza stanza;
    Tipo_trabocchetto trabocchetto;
    Tipo_tesoro tesoro;
};

struct Giocatore{
    char nome_giocatore[25];
    Tipo_giocatore classe_giocatore;
    Tipo_arma weapon;
    unsigned int monete;
    struct Stanza *posizione;
    unsigned char p_vita_max;
    unsigned char p_vita;
    unsigned char dadi_attacco;
    unsigned char dadi_difesa;
    unsigned char volte_scappa;
};

struct All_name{
    char name[25];
    struct All_name *next_name;
};

typedef struct Mercante Mercante;
typedef struct All_name All_name;
typedef struct Giocatore Giocatore;
typedef struct Stanza Stanza;
typedef struct Enemy Enemy;

boolean imposta_gioco();
boolean gioca();
boolean termina_gioco();
boolean crediti();

boolean print_title(char *title, const char *color);
int input_scan(int limit_min, int limit_max, const char* command);

