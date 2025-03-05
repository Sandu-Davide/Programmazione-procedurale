[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/RzYJLA_u)
# Progetto-finale-2024-Prince-Inertia
Progetto finale Programmazione Procedurale UniPG Informatica

## Nome: Davide Filippo

## Cognome: Sandu

## Matricola: 376410

## Commenti/modifiche al progetto:
<ul>
  <li>Aggiunta struct Mercante: quando si avanza in una stanza c'è il 15% di probabilità che un mercante compaia. Esso permette di avere 4 prodotti, il giocatore può comprare solo un articolo.</li>
  <li>Aggiunta la possibilità di avere armi: ogni giocatore può trovare diverse armi (nessuna, pugnale, spada, frusta, doppie_lame, mazza), ogni arma permette di avere un potenziamento ai danni, facendo alcuni danni in più al nemico mentre si combatte.</li>
  <li>Aggiunta la possibilità di avere monete. Ogni giocatore può trovare delle monete nelle varie stanze e quando si trovano i tesori, le monete servono esclusivamente per comprare gli articoli dei mercanti.</li>
  <li>Aggiunta la possibila per i nemici di avere scudi, ogni scudo permette di diminuire alcuni danni inflitti dal giocatore tramite un arma. Ogni volta che si usa uno scudo quest'ultimo viene consumato.</li>
  <li>Ogni volta che si sceglie di invocare la funzione combatti() quest'ultima non finisce il combattimento in automatico, va chiamata più volte per fin quando il giocatore o il nemico non muore. Questo permette al giocatore di provare a scappare durante un turno se si trova in difficoltà, oppure di passare il turno. Se un nemico è presente e si decide di passare il turno il nemico scomparirà. Ogni giocatore può decidere quale azione eseguire per riuscire a sopravvivere il più possibile.</li>
  <li>Quando si trova Jaffar esso rimane nell'ultima stanza, se un giocatore lo ferisce è poi decide di passare il turno il giocatore successivo troverà Jaffar indebolito.</li> 
</ul>
