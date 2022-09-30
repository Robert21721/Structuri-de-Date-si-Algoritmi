 ========= TreeMap ==========
 
        /   /       \
       /   |    /  / \
    \ \/   \   \ \  / /
     \_\  _/ /  \/ /_/
       \_\/   |  /__/
        \_\    \/_/  
           \   /
            | |   O
            | |  /|\
            | |   |
            | |  / \

==============================


!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

AM FOLOSIT LINKUL DE END PENTRU O PARCURGERE MAI RAPIDA IN AMBELE SENSURI

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!



--- createTree ---

# aloc memorie pentru tree
# atribui functiile si setez dimensiunea cu 0
---------------------------------------------


--- search ---

# cauta un element apeland functia compare
# in functie de valoarea intoarsa in urma compararii arborele este
parcurs la stanga sau la dreapta
---------------------------------------------


--- minimum ---

# intoarce cel mai din stanga element
---------------------------------------------


--- maximum ---

# intoarce cel mai din dreapta element
---------------------------------------------


--- successor ---

# daca copilul drept este NULL parcurg pe linkuri de parinte pana gasesc
un element mai mare decat x
# in caz contrar se duce in dreapta si returneaza maximum(x)
---------------------------------------------


--- predecessor ---

# daca copilul stang este NULL parcurg pe linkuri de parinte pana gasesc
un element mai mic decat x
# in caz contrar se duce in stanga si returneaza minimum(x)
---------------------------------------------


--- avlRotateLeft ---

# efectuez rotatia la stanga conform cursului si laboratorului
---------------------------------------------


--- avlRotateRight ---

# efectuez rotatia la dreapta conform cursului si laboratorului
---------------------------------------------


--- avlGetBalance ---

# returneaza diferenta inaltimilor (stanga - dreapta)
---------------------------------------------


--- avlFixUp ---

# actualizez inaltimea
# in functie de valoare returnata de GetBalance efectuez rotirile corespunzatoare
de la nodul primit ca parametru pana la nodul radacina
---------------------------------------------


--- insert ---

# creez noul nod
# daca radacina este NULL, nodul devine radacina
# parcurg arborele pana gasesc locul de inserare
# daca valoarea este duplicat inserez in lista
# in caz contrar adaug nodul la finalul arborelui (ca frunza)
# in functie de valoarea nodului fac diferit inserarea in lista
# apelez functia de fixUp si incrementez dimensiunea
---------------------------------------------


--- List_insert_existed_node ---

# functie care insereaza un nod in lista daca este duplicat
---------------------------------------------


--- ist_insert_node_greater ---

# functie care insereaza un nod in lista daca valoarea este mai mare decat radacina
---------------------------------------------


--- List_insert_node_lower ---

# functie care insereaza un nod in lista daca valoarea este mai mica decat radacina
---------------------------------------------


--- delete ---

# cauta elementeul in arbore
# daca a gasit elementul decrementeaza dimensiunea si opreste cautarea
# daca exista duplicat refac legaturile si sterg ultimul nod duplicat din lista
# tratez separat cazurile cand unul dintre copii este NULL (sau amandoi) si cel 
in care nodul are 2 copii
# daca ambii copii sunt diferiti de NULL aflu succesorul
# actualizez legaturile in arbore pentru succesor
# aduc succesorul in locul nodului x
# actualizez legaturile in lista pentru succesor
---------------------------------------------


--- destroyTree ---

# sterg radacina cat timp inca am noduri in arbelui
---------------------------------------------



========= Cipher ==========

010101011010101010101010101
010101011011111010111101010
101010101111101010101110101
010111011000010010101010001
010010101010110101010101000

============================


--- buildTreeFromFile ---

# citesc cate un rand din fisier si creez noduri in arbore pentru fiecare cuvant
---------------------------------------------


--- inorderKeyQuery ---

# parcurg lista in ordine crescatoare si retin elementele in range
---------------------------------------------


--- levelKeyQuery ---

# aflu nodul cu numar maxim de aparitii
# arcurg lista de la inceput si daca nodul este pe nivelul care trebuie
retin valorile in range (cu tot cu duplicare)
---------------------------------------------


--- rangeKeyQuery ---

# inaitez in lista pana la primul element care apartine intervalului
# retin valorarea nodurilor din interiorul intervalului in range
---------------------------------------------
