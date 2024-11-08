#include <stdio.h>
#include <math.h>

double a,b,c;
typedef enum tipiTriangolo{EQUILATERO, ISOSCELE, SCALENO, RETTANGOLO, NESSUNO, NONRETTANGOLO} TipiTriangolo;
typedef enum booleano {FALSO,VERO} Boolean;

Boolean controlloValidita(double a, double b, double c); //controllera' i valori dei lati del triangolo per valutare se e' possibile rappresentare un triangolo.
TipiTriangolo definizioneTriangolo(double a, double b, double c); //verifica della tipologia del triangolo EQUILATERO, ISOSCELE, SCALENO
TipiTriangolo controlloRettangolo(double a, double b, double c); //verifica del triangolo: e' anche rettangolo?

int main(){
    Boolean valoreControllo;
    do{
        puts("--------------------------------------------------------------");
        puts("INSERIMENTO VALORI");
        puts("--------------------------------------------------------------");

        puts("\nInserisci i valori delle lunghezze dei tre lati del triangolo.");
        printf("Inserisci il valore del lato A: ");
        scanf("%lf",&a);
        printf("Inserisci il valore del lato B: ");
        scanf("%lf",&b);
        printf("Inserisci il valore del lato C: ");
        scanf("%lf",&c);
        valoreControllo = controlloValidita(a,b,c);
    } while(valoreControllo != 1);

    TipiTriangolo tipologiaDelTriangolo = definizioneTriangolo(a,b,c);
    TipiTriangolo triangoloRettangolo = controlloRettangolo(a,b,c);

    puts("TIPOLOGIA TRIANGOLO");
    puts("--------------------------------------------------------------");
    switch (tipologiaDelTriangolo)
    {
    case EQUILATERO:
        puts("Il triangolo e': equilatero");
        break;
    case ISOSCELE:
        if(triangoloRettangolo == 3){
            puts("Il triangolo e': isoscele, rettangolo");
        }else{
            puts("Il triangolo e': isoscele");
        }
        break;
    case SCALENO:
        if(triangoloRettangolo == 3){
            puts("Il triangolo e': scaleno, rettangolo");
        }else{
            puts("Il triangolo e': scaleno");
        }
        break;
    
    default:
        break;
    }

    return 0;
}

/*per rappresentare correttamente un triangolo bisogna verificare se: 
    tutti i lati devono essere positivi
    ogni lato deve essere minore della somma degli altri due
    ogni lato deve essere maggiore della differenza degli altri due*/

Boolean controlloValidita(double a, double b, double c){
    puts("--------------------------------------------------------------");
    puts("CONTROLLO VALORI");
    puts("--------------------------------------------------------------");
    Boolean stato = FALSO;
    if(a>0 && b>0 && c>0){
        if(a < b+c && b<a+c && c<a+b){
            if((a > fabs(b - c)) && (b > fabs(a - c)) && (c > fabs(a - b))){
                stato = VERO;
            }else{
                stato = FALSO;
            }
        }else{
            stato = FALSO;
        }
    }else{
        stato = FALSO;
    }

    printf("%s\n", stato == 0 ? "ERRORE: cambia i valori!" : "Valori corretti!");
    puts("--------------------------------------------------------------");
    return stato;
}

TipiTriangolo definizioneTriangolo(double a, double b, double c){
    TipiTriangolo tipo = NESSUNO;

    if(a==b && b==c){
        tipo = EQUILATERO;
    }else if(a == b || b == c || a == c){
        tipo = ISOSCELE;
    }else{
        tipo = SCALENO;
    }

    return tipo;
}

TipiTriangolo controlloRettangolo(double a, double b, double c){
    return (a*a == b*b + c*c || b*b == a*a + c*c || c*c == b*b + a*a) ? RETTANGOLO : NONRETTANGOLO;
}