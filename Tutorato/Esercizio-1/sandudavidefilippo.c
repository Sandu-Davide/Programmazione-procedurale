#include <stdio.h>
#include <math.h>
double a,b,c;
double soluzioni[2]; //Array per memorizzare le possibili soluzioni, il massimo numero di soluzioni che potrebbe verificarsi e 2.

void controlloCasi(double a, double b, double c); //Verifica tutti i casi possibili previsti dalla traccia e mostra anche la/e soluzione/i dell'equazione.
double calcoloDelta(double a,double b, double c); //Calcolo del delta
void calcoloSoluzioni(double delta,double a, double c, int sol); //il parametro sol. indica il numero di soluzioni che avra la nostra equazione.

int main(){
    int ripetizione=0;
    do{
    puts("----------------------------------------------");
    puts("INSERIMENTO DEI VALORI");
    puts("----------------------------------------------");
    printf("Inserisci il valore del coefficiente a: ");
    scanf("%lf", &a);
    printf("Inserisci il valore del coefficiente b: ");
    scanf("%lf", &b);
    printf("Inserisci il valore del coefficiente c: ");
    scanf("%lf", &c);
    puts("----------------------------------------------");
    controlloCasi(a,b,c);
    puts("----------------------------------------------");
    printf("Vuoi risolvere un'altra equazione? (Si=1 | No=0): ");
    scanf("%d", &ripetizione);
    } while(ripetizione==1);

    return 0;
}

void controlloCasi(double a, double b, double c){
    puts("CONTROLLO DEI CASI");
    puts("----------------------------------------------");
    if(a==0){
        if(b==0){
            puts("L'equazione ammette infinite soluzioni.");
        }else{
            puts("L'equazione e di primo grado.");
            printf("L'unica soluzione vale: %lf\n", (-c/b));
        }
        
    }else{
        puts("Equazione di secondo grado.");
        puts("----------------------------------------------");
        puts("SOLUZIONI");
        puts("----------------------------------------------");
        double valoreDelta = calcoloDelta(a,b,c);
        if(valoreDelta > 0){
            puts("Esistono due soluzioni.");
            calcoloSoluzioni(valoreDelta,a,b,2);
        }else if(valoreDelta < 0){
            puts("Non esistono soluzioni.");
            
        }else{
            puts("Esiste solo una soluzione.");
            calcoloSoluzioni(valoreDelta,a,b,1);
        }
    }
}

double calcoloDelta(double a, double b, double c){
    //Si puo evitare di memorizzare in una variabile il risultato del cacolo e dunque scrivere: return (b*b)-(4*a*c);
    double delta = (b*b)-(4*a*c);
    return delta;
}

void calcoloSoluzioni(double delta,double a, double b, int sol){
    if(sol == 2){
        soluzioni[0] = (-b + sqrt(delta))/(2*a);
        soluzioni[1] = (-b - sqrt(delta))/(2*a);
        printf("Le due soluzioni sono: x1=%lf, x2=%lf\n", soluzioni[0],soluzioni[1]);
    }else{
        soluzioni[0] = -b/(2*a);
        soluzioni[1] = NAN;
        printf("Le soluzione e: x=%lf\n", soluzioni[0]);
    }
}
