Server u C++ programskom jeziku koristeći sockete.

Aplikacija za jednostavan forum.

## Pokretanje

Za izgradnju aplikacije u njezinom korijenskom direktoriju, potrebno je pokrenuti na-
redbu ./compile.sh, koja će izgraditi program koji se može pokrenuti. U slučaju da se ne
može pokrenuti, potrebno je prije upisati chmod u+x compile.sh, što će korisniku dati prava
za izvršavanje nad tom datotekom. Nakon što se program izgradi, potrebno je u konzolu upisati
./program [port], gdje port može biti bilo koji slobodan port, npr. ./program 1234. Ako
je port zauzet, aplikacija se ne može pokrenuti. Aplikacija se tada pokreće na localhost adresi
i moguće ju je koristiti.
