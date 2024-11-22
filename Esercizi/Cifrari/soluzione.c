#include <stdio.h>

void letterLower(char stringa[], int index); //Controlla tutti i caratteri presenti in stringa[] e' converte tutte le lettere maiuscole in minuscole
void isLetter(char stringa[], int index); //Controlla tutti i caratteri presenti in stringa[] e' toglie tutti i caratteri diversi dalle lettere minuscole
int selectionCifrario(); //Permette l'inserimento di un singolo valore tra (V,v | P,p) e' ritorna il valore della selezione
int repeatChoice(); //Permette l'inserimento di un singolo valore tra (Y,y | N,n) e' ritorna la scelta presa, serve per decidere se ripetere la cifratura o meno

typedef enum{Vigenere=1,Playfair} typeCifrario;

void cifrarioVigenere(char stringa[], int index);
void cifrarioPlayfair(char stringa[], int index);

void deleteInput(int sizeLength, const int lengthOfString); //Elimina i caratteri non utilizzati che rimangono nella memoria dell'input
int lengthString(const char string[]); //Controlla la lunghezza effettiva della stringa dopo la lavorazione
void inputString(char stringa[], int index); //Permette l'inserimento di caratteri grazie alla funzione fgets, controlla la sua lunghezza effettiva e' invoca eliminazioneInput

void puliziaScan(); //Pulisce la memoria dell'input se presente un '\n'

const int lastIndex = 1; //Serve per aumentare la dimensione dei array di uno per includere esattamente il num. di caratteri voluti piu' '\n'
//Vigenere
void repeatKey(char key[], int lengthKey, char newKey[], int lengthNewKey); //Ripete la chiave inserita fino alla lunghezza effettiva del plaintext
void cypherOfText(char stringa[],char key[], char cypherText[], int lengthCypher); //Cifra il il testo utilizzando il cifrario di Vigenere
//Playfair
void cypherTextPlayfair(int lengthDigrafi, int column, char digrafi[lengthDigrafi][column], int index, char stringa[]);
void findLocate(int lengthDigrafi, int column, char digrafi[lengthDigrafi][column], char cypherKey[5][5], int *row1, int *row2, int *column1, int *column2, int l);

int main(){
    puts("-------------------------------------------------------------");
    puts("-- CIFRARIO");
    puts("-------------------------------------------------------------");
    const int indexString = 24+lastIndex;
    char stringa[indexString];

    do{
        printf("Inserisci il testo in chiaro: ");
        inputString(stringa, indexString);
        
    }while(lengthString(stringa) == 1);

    letterLower(stringa,indexString);
    isLetter(stringa, indexString);
    
    int choice=0;
    do{
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
        choice = repeatChoice();
    }while (choice == 1);
    
    return 0;
}

void letterLower(char stringa[], int index){
    /*  essendo che char stringa[] e' un puntatore esso accedera'
        all'indirizzo di memoria della "stringa" passata e' cambiare il suo valore */
    for(int i=0; i < index; i++){
        // 'A'=65, 'Z'=90, 'a'=97, 'z'=122
        //aggiungo 32 al valore del carattere così da avere la stessa lettere minuscola (32 differenza tra 'A' e' 'a')
        if(stringa[i] >= 65 && stringa[i] <= 90){
            stringa[i] += 32;
        }
    }
}

void isLetter(char stringa[], int index){
    char changeString[index];
    int j=0; //questa variabile tiene traccia della posizione attuale del puntatore changeString, aumento il suo valore se trovo un carattere valido
    for(int i=0; i < index; i++){
        // '\0' e' il carattere di terminazione, se trovo tale carattere significa che ho controllato tutti
        // i caratteri che mi interessavano dunque fermo il ciclo così da non analizzare piu' niente
        if (stringa[i] == '\0') {
            break;
        }
        //visto che lavoro con i char potevo utilizzare al posto di 97 e 122: 'a', 'z'
        if(stringa[i] >= 97 && stringa[i] <= 122){
            changeString[j] = stringa[i];
            j++;
        }
        
    }
    changeString[j] = '\0'; //Inserisco il carattere di terminazione all'ultima posizione cosi' se stampero' (%s, changeString) visualizzero' tutti i caratteri correttamente

    for(int i=0; i <= j; i++){
        //entrambi gli array hanno la stessa grandezza quindi posso cambiare i valori dell'array stringa con quelli che mi interessano (cosi' succesivamente posso utilizzare sempre "stringa")
        stringa[i] = changeString[i];
    }

}

int selectionCifrario(){
    puts("\n-------------------------------------------------------------");
    puts("-- SCELTA DEL CIFRARIO");
    puts("-------------------------------------------------------------");
    char key=0; //andra' a contenre il carattere preso in input dall'utente
    int option=0; //andra' a contere il valore della scelta dell'utente per poi essere utilizzata con typeCifrario

    do{
    puts("Scegli il cifrario che vuoi utilizzare: \n1)Inserisci uno di questi valori (V,v) per usare Vigenere,\n2)Inserisci uno di questi valori (P,p) per usare Playfair ");
    printf("Inserisci il valore: ");
    scanf("%c", &key);
    
    if(key == 'V' || key == 'v'){
        option=1;
    }else if(key == 'P' || key == 'p'){
        option=2;
    }else{
        puts("\n---------------------------------------------");
        puts("ERRORE: non hai inserito un carattere valido!");
        puts("---------------------------------------------");
        option=0;
        deleteInput(1,1); //Mi assicuro di togliere dal buffer input qualsiasi altro carattere che potrebbe dare errori
    }
    }while (option==0);

    //uso puliziaScan per togliere l'ultimo \n rimasto nel buffer dell'input
    puliziaScan();
    return option;
}

int repeatChoice(){
    char choise=0; //andra' a contenre il carattere preso in input dall'utente
    int option=0; //andra' a contere il valore della scelta dell'utente per poi essere utilizzata per ripere o meno la cifratura
    
    puts("\n---------------------------------------------");
    puts("-- SCELTA RIPETIZIONE");
    puts("---------------------------------------------");
    do{
    puts("Vuoi ripetere la cifratura?: \n1)Inserisci uno di questi valori (Y,y) per cifrare nuovamente,\n2)Inserisci uno di questi valori (N,n) per terminare il programma. ");
    printf("Inserisci il valore: ");
    scanf("%c", &choise);
    
    if(choise == 'Y' || choise == 'y'){
        option=1;
    }else if(choise == 'N' || choise == 'n'){
        option=2;
    }else{
        puts("\n---------------------------------------------");
        puts("ERRORE: non hai inserito un carattere valido!");
        puts("---------------------------------------------");
        option=0;
        deleteInput(1,1); //Mi assicuro di togliere dal buffer input qualsiasi altro carattere che potrebbe dare errori
    }
    }while (option==0);

    //uso puliziaScan per togliere l'ultimo \n rimasto nel buffer dell'input
    puliziaScan();
    return option;
}

void cifrarioVigenere(char stringa[], int index){
    puts("\n-------------------------------------------------------------");
    puts("-- CIFRARIO DI VIGENERE");
    puts("-------------------------------------------------------------");

    const int keylength=8+lastIndex;
    char keyString[keylength]; //chiave preso in input

    char newKey[index]; //questo array conterra' la chiave inserita dall'utente della lunghezza del testo da cifrare e senza caratteri al di fuori delle lettere minuscole
    
    printf("Inserisci la chiave: ");
    inputString(keyString, keylength);
    
    letterLower(keyString, keylength);
    isLetter(keyString, keylength);

    int lengthOfText = lengthString(stringa); //Mi serve la lunghezza effettiva dell'array
    int lengthOfKey = lengthString(keyString);
    repeatKey(keyString, lengthOfKey, newKey, lengthOfText);

    printf("Testo inserito:  %s\n", stringa);
    printf("Chiave inserita: %s\n", newKey);

    char cypherText[lengthOfText]; //conterra' grazie alla funzione cypherOfText() il testo criptato
    cypherOfText(stringa,newKey,cypherText,lengthOfText);

    printf("Testo cifrato:   %s\n", cypherText);
    
}

void cifrarioPlayfair(char stringa[], int index){
    puts("-------------------------------------------------------------");
    puts("-- CIFRARIO DI PLAYFAIR");
    puts("-------------------------------------------------------------");

    //lenghtOfStringa conterra' la quantita di digrafi da creare, se il numero e' dispari allora aggiungo 1 così da includere tutte le coppie possibili 
    int lenghtOfStringa=0;
    if(lengthString(stringa) > 2){
        lenghtOfStringa = (lengthString(stringa)/2)%2==0 ? lengthString(stringa)/2 : (lengthString(stringa)/2)+1;
    }else{
        lenghtOfStringa = 1;
    }
    
    char digrafo[lenghtOfStringa][2]; //il valore 2 indica la quantita' di caratteri nel digrafo

    int j = 0;
    /*  REGOLE DIGRAFI:
        Se entrambe le lettere sono le stesse nel digrafo (o se la lettera è da sola),
        si aggiunga un "x" dopo la prima lettera. Altrimenti lascia la coppia cosi' com'e'.
        Cifrare la nuova coppia di lettere e continuare.*/
    for (int i = 0; i < lengthString(stringa); i++) {
        if(lenghtOfStringa == 1){
            digrafo[0][0] = stringa[0];
            digrafo[0][1] = 'x';
            break;
        }else{
            digrafo[j][0] = stringa[i];

            if (i + 1 < lengthString(stringa)) { //controllo che esista il carattere successivo
                if (stringa[i] == stringa[i + 1]) {
                    //non ci possono essere due lettere uguali, quindi aggiungo un 'x' dopo la prima lettera
                    digrafo[j][1] = 'x';
                } else {
                    digrafo[j][1] = stringa[i + 1];
                    i++; // il prossimo carattere è incluso nel digrafo quindi vado al prossimo
                }
            } else {
                digrafo[j][1] = 'x'; //E presente un solo carattere quindi il prossimo sara' 'x'
            }

            //printf("%c%c ", digrafo[j][0], digrafo[j][1]); //(debug)
            j++;
        }
    }

    cypherTextPlayfair(lenghtOfStringa, 2, digrafo, index, stringa);
    
}

void cypherTextPlayfair(int lengthDigrafi, int column, char digrafi[lengthDigrafi][column], int index, char stringa[]){
    char cypherKey[5][5]={
        {'e','s','m','p','i'},
        {'o','l','a','y','f'},
        {'r','b','c','d','g'},
        {'h','k','n','q','t'},
        {'u','v','w','x','z'},
    };
    /*Regole dicifratura: se le lettere sono sulla stessa riga: vengono cifrate con la prima lettera a destra
                          se le lettere sono sulla stessa colonna: vengono cifrate con le lettere sotto di esse
                          se le lettere formano un rettangono: vengono cifrate con i vertici opposti*/
    char cypherText[index];
    int row1=0, row2=0, column1=0, column2=0; //queste variabili conteranno' le coordinate delle lettere nel digrafo ('x','y')
    int rowOfDigrafo=0, h=0; //rowOfDigrafo indica la riga da volutare del digrafo mentre h e' la posizione dell'array

    for(int k=0; k<lengthDigrafi; k++){
        findLocate(lengthDigrafi, column, digrafi, cypherKey, &row1, &row2, &column1, &column2,rowOfDigrafo);
        if(row1 == row2){ //le due lettere si trovano nella stessa riga del cifrario
            if(column1+1 < 5){
                cypherText[h]= cypherKey[row1][column1+1];
            }else{
                cypherText[h]= cypherKey[row1][0];
            }
            h++;
            if(column2+1 < 5){
                cypherText[h]= cypherKey[row2][column2+1];
            }else{
                cypherText[h]= cypherKey[row2][0];
            }
            h++;
        }else if(column1 == column2){ //le due lettere si trovano nella stessa colonna del cifrario
            if(row1+1 < 5){
                cypherText[h]= cypherKey[row1+1][column1];
            }else{
                cypherText[h]= cypherKey[0][column1];
            }
            h++;
            if(row2+1 < 5){
                cypherText[h]= cypherKey[row2+1][column2];
            }else{
                cypherText[h]= cypherKey[0][column2];
            }
            h++;
        }else{ //le due lettere non si trovano ne nella stessa riga ne nella stessa colonna del cifrario
            cypherText[h]= cypherKey[row1][column2];
            h++;
            cypherText[h]= cypherKey[row2][column1];
            h++;
        }
        rowOfDigrafo++;
    }
    
    cypherText[h] = '\0';
    printf("Testo inserito: %s\n",stringa);
    printf("Testo cifrato:  %s\n",cypherText);

}

void findLocate(int lengthDigrafi, int column, char digrafi[lengthDigrafi][column], char cypherKey[5][5], int *row1, int *row2, int *column1, int *column2,int l){
    for(int i=0; i<5; i++){
        for(int j=0; j<5; j++){
            if(digrafi[l][0] == cypherKey[i][j]){
                *row1 = i;
                *column1 = j;
                //printf("%c, %d %d\n", digrafi[l][0], i,j); //(debug)
            }
            if(digrafi[l][1] == cypherKey[i][j]){
                *row2 = i;
                *column2 = j;
                //printf("%c, %d %d\n", digrafi[l][1], i,j); //debug
            }
        }
    }
    
}

void deleteInput(int sizeLength, const int lengthOfString){
    /*usiamo questa funzione quando abbiamo un problema con il buffer di lettura, se usiamo la funzione fgets e' inseriamo un numero
      di caratteri superiori alla capienza dell'array utilizzando successivamente la funzione scanf esso non ci permetteva di inserire
      tramite input qualsiasi tipo di carattere poiche' prendeva in considerazione i caratteri in eccesso dal fgets*/
    if(lengthOfString >= sizeLength-1){
        while(getchar() != '\n');
    }
}

int lengthString(const char string[]){
    //Scorriamo tutto l'array e' aggiorniamo la sua lunghezza fino alla sua terminazione (\0)
    int length=0;

    while(string[length] != '\0'){
        length++;
    }

    return length;
}

void inputString(char stringa[], int index){
    //prendo in input una serie di caratteri con fgets() e successivamente pulisco il buffer input
    fgets(stringa, index, stdin);
    int lengthOfStringa = lengthString(stringa);

    deleteInput(index, lengthOfStringa);
}

void puliziaScan(){
    //elimino il \n rimasto in input
    getchar();
}

void repeatKey(char key[], int lengthKey, char newKey[], int lengthNewKey){
    //inserisco ogni carattere nell'array newKey[], quando la chiave inserita dall'utente sara' terminata riparto dalla posizione 0
    int j=0;
    for(int i=0; i<lengthNewKey; i++){
        newKey[i] = key[j];
        j++;
        if(j == lengthKey){
            j=0;
        }
    }
    newKey[lengthNewKey] = '\0'; //aggiungo il carattere di terminazione per non avere problemi di output, etc... successivamente
}

void cypherOfText(char stringa[],char key[], char cypherText[], int lengthCypher){
    /* cifro il testo dato dall'utente utilizzando il cifrario di Vigenere quindi con l'uso di una chiave
        Funzionamento: sommo i due valori dei caratteri, se la somma e maggiore della somma di 'a'+'z'
        dobbiamo far tornare il valore ad 'a' quindi sottraiamo il valore di 'z' altrimenti sottraiamo il valore
        di 'a'.
    */
    for(int i=0; i<lengthCypher; i++){
        int valore = stringa[i] + key[i];
        if(valore > 219){
            valore -= 123;
        }else{
            valore -= 97;
        }
        //printf("%d %c, %d %c, %d %c\n", stringa[i],stringa[i], newKey[i],newKey[i],valore,valore); (debug)
        cypherText[i] = valore;
    }
    cypherText[lengthCypher] = '\0';
}