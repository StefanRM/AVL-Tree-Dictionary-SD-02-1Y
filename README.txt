.___________. _______ .___  ___.      ___          ___  
|           ||   ____||   \/   |     /   \        |__ \ 
`---|  |----`|  |__   |  \  /  |    /  ^  \          ) |
    |  |     |   __|  |  |\/|  |   /  /_\  \        / / 
    |  |     |  |____ |  |  |  |  /  _____  \      / /_ 
    |__|     |_______||__|  |__| /__/     \__\    |____|


=== Cerinta 1 ===

	In AVLTree.h avem urmatoarele functii pentru a manipula un arbore de
cautare (echilibrat) AVL, care stocheaza duplicatele intr-o lista:

	1) avlCreateTree() - aloca memorie pentru un astfel de arbore si
			  se efectueaza initializarile necesare;

	2) avlIsEmpty() - returneaza 1 sau 0 daca arborele este gol sau nu;

	3) avlNewNode() - aloca memorie unui nou nod care va fi folosit in
			arbore, efectuandu-se initializarile necesare;

	4) avlDeleteNode() - dezaloca memoria pentru un nod, distrugandu-se
			informatia din elem si info;

	5) avlRightRotate() - efectueaza o rotatie dreapta pentru a reechilibra
			arborele;

	6) avlLeftRotate() - efectueaza o rotatie stanga pentru a reechilibra
			arborele;

	7) avlGetBalance() - returneaza 1 sau 0 daca arborele este balansat sau nu;

	8) avlMinimum() - returneaza minimul din arbore;

	9) avlMaximum() - returneaza maximul din arbore;

	10) avlSuccessor() - returneaza succesorul unui nod din arbore;

	11) avlPredecessor() - returneaza predecesorul unui nod in arbore;

	12) avlinsertFixUp() - rebalanseaza arborele dupa inserare (4 cazuri);

	13) avlInsert() - insereaza un nou nod in arbore (fie el si duplicat,
			datorita listei);

	14) avlExactQuery() - returneaza un nod din arbore cautat dupa campul elem,
			in cazul in care nu se gaseste se returneaza NULL;

	15) avlDeleteFixUp() - rebalanseaza arborele dupa stergere (4 cazuri);

	16) avlDelete() - sterge un nod din arbore;

	17) avlDestroyTreeHelper() - dezaloca memoria alocata pentru toate nodurile
				din arbore;

	18) avlDestroyTree() - dezaloca memoria alocata santinelelor si arborelui.



=== Cerinta 2 ===

	Functia buildTreeFromFile() creeaza un arbore AVL pe baza unui fisier text.
Nodurile arborelui stocheaza in campul Item1 elem primele trei litere ale cuvintelor
si in campul Info info indexul din fisier aferent acestora.
	In aceasta functie am aflat cuvintele delimitate de spatiu (' '), si am extras
primele trei litere din fiecare si pozitia unde se aflau in cadrul textului.

=== Cerinta 3 ===

	Pentru cerintele 3, 4 si 5 am creat 2 functii auxiliare:

	1) makeRange() - initializeaza range-ul;
	2) insertRange() - insereaza elemente in range;

	Functia singleKeyRangeQuery() foloseste o strategie de cautare prin intermediul
careia se obtin toti indecsii ai caror chei incep cu cheia de cautare (indecsii vor fi
inserati in range). Cautarea se face dupa o cheie de cautare, nu neaparat exacta.



=== Cerinta 4 ===

	Functia multiKeyRangeQuery() foloseste o strategie de cautare prin intermediul
careia se obtin toti indecsii ai caror chei se afla in intervalul inchis al cheilor de
cautare primite de functie (indecsii vor fi inserati in range).



=== Cerinta 5 ===

	Functia printWordsInRange primeste un range si un fisier asociat, si afiseaza
cuvintele de la indecsii stocati in range din fisier.
