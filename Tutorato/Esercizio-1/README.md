<h1>Esercizio: equazioni di secondo grado</h1>
<h2>https://github.com/ProgrammazioneI-UniPG/Esercizio1TutoratoGizzi</h2>
<hr>

<h2>Obiettivo:</h2>
<p>Si realizzi un programma in linguaggio C per risolvere equazioni di secondo grado. In particolare, data una generica equazione di secondo grado nella forma: <b>ax2 + bx + c = 0</b>
dove a, b, c sono coefficienti reali noti (valori di tipo double passati da tastiera con <u>scanf()</u>) e x rappresenta l’incognita, il programma determini le due radici x1 ed x2 dell’equazione 
data, ove esse esistano. Si identifichino tutti i casi particolari <br><b>(a == 0, ∆ == 0, ∆ < 0, ∆ > 0 come sotto specificato)</b> e si stampino gli opportuni messaggi informativi. 
<b>∆=b2÷4ac</b>, <b>x=(−b ±√∆)÷2a</b>. <br><br>
Includere la libreria math.h per poter utilizzare la funzione radice quadrata sqrt(). Quando si
compila con gcc aggiungere “-lm” per linkare la libreria matematica (esempio, gcc -o programma
programma.c -lm)

<ul>
  <li>Se a == 0 allora e’ una equazione di primo grado.</li>
  <ul>
    <li> Se anche b == 0 allora l’equazione ammette infinite soluzioni (stampare questo messaggio).</li>
    <li> Se b! = 0, l’unica soluzione vale −c ÷ b</li>
  </ul>
</ul><br>
<ul>
  <li>Se ∆ == 0 esiste una sola soluzione.</li>
  <li>Se ∆ > 0 esistono due soluzioni.</li>
  <li>Se ∆ < 0 non esistono soluzioni in campo reale (qui basta solo stampare questo messaggio)</li>
</ul>
</p>

