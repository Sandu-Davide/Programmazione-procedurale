#include <stdio.h>

void letterLower(char stringa[], int index);
void isLetter(char stringa[], int index);
int selectionCifrario();

typedef enum {A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z} alphabet;
typedef enum{Vigenere=1,Playfair} typeCifrario;

void cifrarioVigenere(char stringa[]);
void cifrarioPlayfair(char stringa[]);

int main(){
    int indexString = 24, indexKey= 8;
    char stringa[indexString], key[indexKey];
    printf("Inserisci il testo in chiaro: ");
    fgets(stringa, sizeof(stringa), stdin) ;

    printf("\nTESTO INSERITO: %s\n",stringa);

    letterLower(stringa,indexString);
    isLetter(stringa, indexString);
    printf("TESTO DOPO LAVORAZIONE: %s\n",stringa);

    typeCifrario cifrario = selectionCifrario();
    if(cifrario==1){
        cifrarioVigenere(stringa);
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
    int key=0; 

    do{
    printf("Inserisci Valore 1=Vigenere, 2=Playfair: ");
    scanf("%d", &key);
    
    if(key == 1){
        puts("Quello di Vigenere");
    }else if(key == 2){
        puts("Quello di Playfair");
    }else{
        puts("Errore non hai inserito un carattere valido!");
    }
    }while (key==0);

    return key;
}

void cifrarioVigenere(char stringa[]){
    alphabet a=0;
    
}