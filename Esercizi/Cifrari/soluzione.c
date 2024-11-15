#include <stdio.h>

void letterLower(char stringa[], int index); //Controlla tutti i caratteri presenti in stringa[] e' converte tutte le lettere maiuscole in minuscole
void isLetter(char stringa[], int index); //Controlla tutti i caratteri presenti in stringa[] e' toglie tutti i caratteri diversi dalle lettere minuscole
int selectionCifrario(); //Permette l'inserimento di un singolo valore tra (V,v | P,p) e' ritorna il valore della selezione

typedef enum {A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z} alphabet;
typedef enum{Vigenere=1,Playfair} typeCifrario;

void cifrarioVigenere(char stringa[], int index);
void cifrarioPlayfair(char stringa[], int index);

void eliminazioneInput(char stringa[], int sizeLength, const int lengthOfString); //Elimina i caratteri non utilizzati che rimangono nella memoria dell'input
int lengthString(const char string[], const int index); //Controlla la lunghezza effettiva della stringa dopo la lavorazione
void inputString(char stringa[], int index); //Permette l'inserimento di caratteri grazie alla funzione fgets, controlla la sua lunghezza effettiva e' invoca eliminazioneInput

void puliziaScan(); //Pulisce la memoria dell'input se presente un '\n'

int main(){
    puts("-------------------------------------------------------------");
    puts("-- CIFRARIO");
    puts("-------------------------------------------------------------");
    const int indexString = 24;
    char stringa[indexString];
    printf("Inserisci il testo in chiaro: ");
    
    inputString(stringa, indexString);

    printf("TESTO INSERITO: %s\n",stringa);

    letterLower(stringa,indexString);
    isLetter(stringa, indexString);
    printf("TESTO DOPO LAVORAZIONE: %s\n",stringa);

    typeCifrario cifrario = selectionCifrario();

    switch (cifrario)
    {
    case Vigenere:
        cifrarioVigenere(stringa,indexString);
        break;
    case Playfair:
        cifrarioPlayfair(stringa,indexString);
        break;
    default:
        break;
    }

    return 0;
}

void letterLower(char stringa[], int index){
    /*  essendo che char stringa[] e' un puntatore esso accedera'
        all'indirizzo di memoria della stringa passata e' cambiare il suo valore */
    
    for(int i=0; i < index; i++){
        // 'A'=65, 'Z'=90
        if(stringa[i] >= 65 && stringa[i] <= 90){
            stringa[i] += 32;
        }
    }
}

void isLetter(char stringa[], int index){
    char changeString[index];
    int j=0;
    for(int i=0; i < index; i++){
        if (stringa[i] == '\0') {
            break;
        }

        if(stringa[i] >= 97 && stringa[i] <= 122){
            changeString[j] = stringa[i];
            j++;
        }
        
    }
    changeString[j] = '\0';

    for(int i=0; i <= j; i++){
        stringa[i] = changeString[i];
    }

}

int selectionCifrario(){
    puts("\n-------------------------------------------------------------");
    puts("-- SCELTA DEL CIFRARIO");
    puts("-------------------------------------------------------------");
    char key=0;
    int option=0;

    do{
    puts("Scegli il cifrario che vuoi utilizzare inserendo uno \ndi questi valori, (V,v) Vigenere, (P,p) Playfair");
    printf("Inserisci il valore: ");
    scanf("%c", &key);
    
    if(key == 'V' || key == 'v'){
        option=1;
    }else if(key == 'P' || key == 'p'){
        option=2;
    }else{
        puts("Errore non hai inserito un carattere valido!");
        option=0;
    }
    }while (option==0);

    puliziaScan();
    return option;
}

void cifrarioVigenere(char stringa[], int index){
    puts("\n-------------------------------------------------------------");
    puts("-- CIFRARIO DI VIGENERE");
    puts("-------------------------------------------------------------");

    alphabet a=0;
    const int keylength=8;
    char keyString[8];
    
    printf("Inserisci la chiave: ");
    inputString(keyString, keylength);

    letterLower(keyString, keylength);
    isLetter(keyString, keylength);
    printf("CHIAVE: %s", keyString);
    
}

void cifrarioPlayfair(char stringa[], int index){
    puts("-------------------------------------------------------------");
    puts("-- CIFRARIO DI PLAYFAIR");
    puts("-------------------------------------------------------------");
}

void eliminazioneInput(char stringa[], int sizeLength, const int lengthOfString){
    puts("Entrato Eliminato");
    if(lengthOfString >= sizeLength-1){
        puts("Entra? Non credo proprio");
        while(getchar() != '\n');
    }
}

int lengthString(const char string[], const int index){
    //Scorriamo tutto l'array e' aggiorniamo la sua lunghezza fino alla sua terminazione (\0)
    int length=0;

    while(string[length] != '\0'){
        length++;
    }

    printf("Lunghezza: %d\n", length);
    return length;
}

void inputString(char stringa[], int index){

    fgets(stringa, index, stdin);
    int lengthOfStringa = lengthString(stringa, index);

    eliminazioneInput(stringa, index, lengthOfStringa);
}

void puliziaScan(){
    getchar();
}