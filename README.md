
# 🧬 Game of Life 

Acest proiect implementează o versiune extinsă a celebrului **Game of Life** conceput de John Conway, utilizând liste, arbori binari și grafuri pentru a analiza evoluția celulelor vii și moarte de-a lungul a mai multor generații.

## 📁 Structura Proiectului

- `task1.c`: Implementare clasică Game of Life pe o matrice.
- `task2.c`: Constă în păstrarea unui istoric al modificărilor celulelor pe k generații folosind o stivă și matricea inițială.
-  ### 💡 Bonus - Reconstrucția stării inițiale având la dispoziție numai stiva cu schimbări și matricea finală (bonus.c)
- `task3.c`: Constă în construirea unui arbore binar de generare cu două reguli de evoluție diferite. Pe dreapta, nodul fiu va urma regula clasică, iar pe stânga se va urmări o altfel de regulă, și anume: orice două celule 
- `task4.c`: Combină arbori binari și grafuri pentru a construi toate generațiile și a calcula drumul hamiltonian maxim în componentele conexe.
---
## ⚙️ Detalii Tehnice
## ▶️ Cum se rulează

```
gcc task4.c necessary.c -o task4.out
./task4.out input output
```
❗Datele vor fi găsite în fișierele de intrare din InputData cu referințe în RefData, câte 5 pe fiecare proiect.

### 🔁 Reguli de evoluție

- **Rule A** (Conway):  
1️⃣ Subpopulare – Orice celulă vie cu mai puțin de 2 vecini moare ☠️.  
2️⃣ Supraviețuire – Orice celulă vie cu 2 sau 3 vecini rămâne în viață 🌱.   
3️⃣ Suprapopulare – Orice celulă vie cu mai mult de 3 vecini moare ☠️.  
4️⃣ Reproducere – Orice celulă moartă cu exact 3 vecini revine la viață 🔄.  

## 📥 Format fișier de intrare 
## Task 1
✅ Cerință: Se vor implementa regulile din Game of Life astfel încât, pornind de la o matrice de dimensiune N × M, reprezentând generația inițială, să se obțină următoarele K generații, salvate de asemenea ca matrici.
```
TEST 
N M 
K 
+ + + + + + 
+ + + + X + 
+ + X X + + 
+ + + X + + 
```
- `TEST` - identificator pentru numarul testului.
- `N` și `M` - dimensiuni matrice.
- `K` - numărul de generații de procesat.
- `X` - celulă vie
- `+` - celulă moartă

### Exemplu:
```
1 
6 6 
2
+ + + + + + 
+ + + + X + 
+ + X X + + 
+ + + X + + 
+ X + + + + 
+ + + + + +
```
Această primă parte a proiectului implementează regulile de bază ale Jocului Vieții folosind o reprezentare matricială. 
Starea grilei (celule vii 'X' sau moarte '+') este stocată într-o matrice 2D. Simulația progresează pe parcursul unui număr specificat de generații, 
aplicând regulile de tranziție celulă cu celulă. Pentru a simplifica calculul vecinilor, matricea este extinsă cu un "cadru" de celule moarte ('+') în jurul ei.
```
void readMatrix(const char *filename, char ***matrix, int *n, int *m, int *nrgen) {
    // ... deschidere fișier ...
    fscanf(file, "%d %d %d", n, m, nrgen);

    *n = *n + 2; // Adaugă rânduri pentru bordură
    *m = *m + 2; // Adaugă coloane pentru bordură

    *matrix = (char **)malloc(*n * sizeof(char *));
    // ... alocare memorie și verificare erori ...

    for (i = 0; i < *n; i++) {
        (*matrix)[i] = (char *)malloc(*m * sizeof(char));
        // ... alocare memorie și verificare erori ...
        for (j = 0; j < *m; j++) {
            if (i == 0 || i == (*n) - 1 || j == 0 || j == (*m) - 1) {
                (*matrix)[i][j] = '+'; // Inițializează bordura cu celule moarte
            } else {
                fscanf(file, " %c", &(*matrix)[i][j]); // Citește celulele reale
            }
        }
    }
    fclose(file);
}
```
Această funcție este responsabilă pentru citirea configurației inițiale dintr-un fișier. 
Un aspect cheie este alocarea dinamică a memoriei pentru matrice și adăugarea automată a unui cadru (padding) de celule moarte ('+') în jurul grilei reale. 
Acest padding simplifică logica de numărare a vecinilor, eliminând necesitatea de a verifica limitele matricei în funcția NrOfNeighbours.

```
int NrOfNeighbours(char **matrix, int i, int j) {
    int nr = 0;
    if (matrix[i-1][j-1] == 'X') nr++;
    if (matrix[i-1][j] == 'X') nr++;
    // ... verifică toți cei 8 vecini ...
    if (matrix[i+1][j-1] == 'X') nr++;
    return nr;
}
```
functia `NrOfNeighbours()` calculează numărul de vecini vii pentru o celulă dată la coordonatele (i, j). 
Datorită bordurii adăugate în readMatrix, această funcție poate verifica direct toate cele 8 poziții 
adiacente fără a se îngrijora de accesarea memoriei în afara limitelor.
Bucla principală din main iterează prin fiecare generație. În cadrul fiecărei generații, se parcurg celulele 
reale ale grilei (excluzând bordura), se numără vecinii vii și se aplică regulile Jocului Vieții pentru a determina 
starea celulei în generația următoare. Rezultatul este stocat temporar într-o matrice new și apoi copiat înapoi în matrix pentru următoarea iterație.

---
## Task 2
✅ Cerință: Se va creea o stivă în care fiecare element corespunde câte unei generații. Fiecare astfel de generație va fi reprezentată printr-o listă de celule ce și-au schimbat starea față de generația anterioară. Prin urmare, se va construi o stivă de liste.
A doua abordare optimizează stocarea stărilor grilei, în loc să memoreze întreaga matrice pentru 
fiecare generație, se înregistrează doar schimbările de stare (celulele care trec de la vie la moartă sau invers) 
folosind o structură de date bazată pe o stivă de liste înlănțuite.
```
int IsCellAliveOrNot(int linie, int coloana, SNode *top, char cell) {
    int count = 0;
    SNode *temp = top;
    while(temp) { // Parcurge stiva
        List *p = temp->head;
        while(p) { // Parcurge lista de schimbări din generația curentă a stivei
            if (p->val[0] + 1 == linie && p->val[1] + 1 == coloana)
                count++; // Incrementăm dacă celula a fost afectată
            p = p->next;
        }
        temp = temp->next;
    }

    if(cell == '+') // Dacă celula inițială era moartă
        return(count % 2 ? 1 : 0); // Este vie dacă a schimbat starea un număr impar de ori
    else if (cell == 'X') // Dacă celula inițială era vie
        return(count % 2 ? 0 : 1); // Este moartă dacă a schimbat starea un număr impar de ori
    else {
        // ... gestionare erori ...
        return -1;
    }
}
```
Aceasta este una dintre cele mai importante funcții din Task 2. Pentru a determina starea unei celule la o generație curentă,
funcția parcurge întreaga stivă de modificări (top) și numără de câte ori a fost afectată celula la coordonatele (linie, coloana). 
Starea finală (vie sau moartă) este apoi dedusă pe baza stării inițiale a celulei (cell) și parității numărului de modificări (count % 2).
```
List* BuildFirstListGeneration(const char *filename, int *gen_number, int n, int m, char **init_matrix) {
    List *head = NULL, *tail = NULL;
    // ... citire fișier și pregătire rânduri (similar Task 1 padding) ...
    for (i = 1; i < m; i++) { // Iterăm rândurile reale
        for (j = 1; j < n-1; j++) { // Iterăm coloanele reale
            vecini = NumberOfLiveCells(previous, middle, next, j, i);
            if (((vecini < 2 || vecini > 3) && middle[j]=='X') || (vecini == 3 && middle[j] == '+')) {
                // Dacă starea celulei se schimbă conform regulilor Game of Life
                List *node = (List *)malloc(sizeof(List));
                node->val[0] = i; // Coordonata rândului
                node->val[1] = j-1; // Coordonata coloanei (ajustată pentru bordură)
                node->next = NULL;
                if (head == NULL) {
                    head = tail = node;
                } else {
                    tail->next = node;
                    tail = node;
                }
            }
        }
        // ... actualizare previous, middle, next ...
    }
    // ... eliberare memorie temporară ...
    return head; // Returnează lista de celule care și-au schimbat starea
}
```
Această funcție calculează prima generație de modificări. Spre deosebire de Task 1, unde se construia o nouă matrice,
aici se construiește o listă înlănțuită ce conține doar celulele care își schimbă starea de la generația 0 la generația 1. 
Această listă este apoi "pushed" în stivă.
```
List* NextGenerations(const char *filename, SNode *top, int n, int m, char **matrix) {
    // ... inițializări ...
    for (i = 1; i <= m; i++) { // Iterăm rândurile
        for(j = 1; j <= n; j++) { // Iterăm coloanele
            live_neighbours = 0;
            // Calculează vecinii vii folosind IsCellAliveOrNot pentru a obține starea curentă a vecinilor
            live_neighbours += IsCellAliveOrNot(i-1, j-1, top, matrix[i-1][j-1]);
            // ... pentru toți cei 8 vecini ...
            current_cell = IsCellAliveOrNot(i, j, top, matrix[i][j]); // Starea celulei curente

            if ((current_cell && (live_neighbours < 2 || live_neighbours > 3)) || (!current_cell && live_neighbours == 3)) {
                // Dacă starea celulei se schimbă, adaugă la lista curentei generații de modificări
                List *node = (List *)malloc(sizeof(List));
                // ... inițializare nod și adăugare la listă ...
            }
        }
    }
    // ... închidere fișier ...
    return head; // Returnează lista de modificări pentru această generație
}
```
Functia `NextGenerations()` este responsabilă pentru calcularea modificărilor pentru generațiile ulterioare (de la generația k-1 la generația k). Starea celulelor și a vecinilor
lor este determinată dinamic folosind funcția IsCellAliveOrNot și stiva de modificări acumulate (top). 
Dacă o celulă își schimbă starea conform regulilor Game Of Life, coordonatele sale sunt adăugate la o nouă listă, 
care va fi ulterior adăugată în stivă.

Formatul fișierelor de intrare este același ca la Task 1
- `TEST` - identificator pentru numarul testului.
- `N` și `M` - dimensiuni matrice.
- `K` - numărul de generații de procesat.
  
### Exemplu:
```
Generația 0:     Generația 1:     Generația 2:

 + + + + +        + + + + +        + + + + + 
 + X + + +        + X X + +        + X X + + 
 + X X + +        + X X + +        + + + X +
 + + + X +        + + X + +        + X X + +
 + + + + +        + + + + +        + + + + +
```
## 📦 Conținut stivă la generația 1:
1: (1,2); (3,2); (3,3)

## 📦 Conținut stivă la generația 2:
1: (1 ,2); (3 ,2); (3 ,3)

2: (2 ,1); (2 ,2); (2 ,3); (3 ,1)
### Bonus
📌 Dându-se conținutul stivei pentru K generații și matricea corespunzătoare generației K, se va scrie într-un fișier matricea corespunzătoare inițială corespunzătoare generației 0.

## Task 3
✅ Cerință: Dându-se o inițializare (rădăcina arborelui), se vor creea doi fii: 
Cel din stânga reprezintă generația 1 obținută cu regula nouă, B.
Cel din dreapta reprezintă generația 1 obținută cu regulile obișnuite.
Repetați operația pentru toate nodurile astfel create până la generația K, dată.  
###  **Rule B** (variantă personalizată - pentru task 3 si 4):  
 ### 5️⃣ Orice celulă cu exact doi vecini vii → devine vie.  
### 📌 Formatul fișierelor de intrare este același ca la task-urile anterioare
### 🌲 Arbore binar
Arborele se ramifică din fiecare nod, generând două posibile stări viitoare ale grilei, 
fiecare aplicând o regulă diferită de tranziție (Rule A și Rule B) pentru celule. 
Această abordare permite explorarea simultană a două evoluții paralele ale jocului.
`RuleA` implementează regulile standard ale Game of Life (celula moartă devine vie dacă are exact 3 vecini vii, 
celula vie supraviețuiește dacă are 2 sau 3 vecini vii). Funcția iterează prin fiecare celulă a grilei și, pe 
baza numărului de vecini vii (determinat cu `FindCoord` din lista root), decide dacă celula este vie în generația 
următoare și o adaugă la noua listă (head).
`RuleB` introduce o variație a regulilor. Conform acestui snippet (bazat pe Task 4, 
unde `CountNeighbours` este disponibil), o celulă moartă (current_cell == 0) devine vie dacă
are exact 2 vecini vii (live_neighbours == 2). Celulele vii (current_cell == 1) rămân vii indiferent de numărul de vecini. 
```
void BuildTree(Node **root, int n, int m, int generations) {
    if(generations == 0 || *root == NULL) return; // Condiția de bază pentru recursivitate

    Node *leftChild = malloc(sizeof(Node));
    Node *rightChild = malloc(sizeof(Node));
    // ... verificări de alocare ...

    leftChild->node = RuleB((*root)->node, n, m); // Aplică RuleB pentru ramura stângă
    leftChild->left = leftChild->right = NULL;

    rightChild->node = RuleA((*root)->node, n, m); // Aplică RuleA pentru ramura dreaptă
    rightChild->left = rightChild->right = NULL;

    (*root)->left = leftChild;
    (*root)->right = rightChild;

    BuildTree(&leftChild, n, m, generations - 1); // Recursivitate pentru ramura stângă
    BuildTree(&rightChild, n, m, generations - 1); // Recursivitate pentru ramura dreaptă
}
```
Această funcție construiește arborele binar recursiv. Pentru fiecare nod (generație),
se creează doi copii: un nod stâng (care aplică `RuleB` pentru a calcula starea generației următoare) și un nod drept (care aplică `RuleA`). 
Procesul continuă recursiv până la atingerea numărului specificat de generații.
![image](https://github.com/user-attachments/assets/185d6a91-771e-460b-8e1d-4345ee623d79)

La fiecare generație:
- Ramura stângă evoluează după **Rule B**.
- Ramura dreaptă evoluează după **Rule A**.   

## Task 4
✅ Cerință:  Se va determina, pentru fiecare nod al arborelui obținut la Taskul 3, până la o generație dată K, cel mai lung lanț Hamiltonian.
Se construiește recursiv un arbore binar de generații.
```
typedef struct Graph {
    int  n; // numărul de noduri posibile (n * m)
    List **adj; // lista de adiacență
} Graph;

Graph* CreateGraph(List *head, int n, int m) {
    Graph *graph = malloc(sizeof(Graph));
    // ... verificări de alocare ...
    graph->n = n*m; // Numărul total de celule în grilă
    graph->adj = (List **)calloc(graph->n, sizeof(List *)); // Alocare pentru lista de adiacență

    List *temp = head; // Parcurge lista de celule vii (nodurile grafului)
    while(temp != NULL) {
        int i = temp->i;
        int j = temp->j;
        int index = i * m + j; // Indexul celulei curente în lista de adiacență

        // Adaugă vecinii vii în lista de adiacență
        if(FindCoord(head, i-1, j-1, n, m))
            BuildList(&graph->adj[index], i-1, j-1);
        // ... pentru toți ceilalți 7 vecini ...
        temp = temp->next;
    }
    return graph;
}
```
Structura `Graph` este folosită pentru a reprezenta grila ca un graf, unde celulele vii sunt noduri și
vecinii vii sunt conectate prin muchii. Funcția `CreateGraph()` construiește lista de adiacență a grafului. 
Pentru fiecare celulă vie din lista head, se verifică vecinii săi și, dacă aceștia sunt de asemenea vii,
se adaugă o muchie (prin adăugarea vecinului în lista de adiacență a celulei curente).
```
void BacktrackHamiltonian(Graph *graph, int entry, int *visited_path, int depth, int *curr_path, int *max_len, int *best_path, int m) {
    visited_path[entry] = 1; // Marchează nodul curent ca vizitat
    curr_path[depth - 1] = entry; // Adaugă nodul la calea curentă

    if (depth > *max_len) { // Dacă s-a găsit un drum mai lung
        *max_len = depth;
        // Salvează calea curentă ca fiind cea mai bună cale găsită până acum
        // (Aici ar trebui să existe o copiere a curr_path în best_path)
    }

    List *temp = graph->adj[entry]; // Parcurge vecinii nodului curent
    while (temp) {
        int v_idx = temp->i * m + temp->j; // Indexul vecinului
        if (!visited_path[v_idx]) { // Dacă vecinul nu a fost vizitat în calea curentă
            BacktrackHamiltonian(graph, v_idx, visited_path, depth + 1, curr_path, max_len, best_path, m); // Recursivitate
        }
        temp = temp->next;
    }
    visited_path[entry] = 0; // Backtracking: marchează nodul ca nevizitat pentru a explora alte căi
}
```
Această funcție implementează un algoritm de backtracking pentru a găsi cel mai lung drum Hamiltonian într-un graf. 
Funcția explorează recursiv toate căile posibile, marcând nodurile vizitate pentru a evita ciclurile. 
Când se găsește o cale mai lungă, aceasta este actualizată. 
`Codul nu este complet...`
### 📌 Formatul fișierelor de intrare este același ca la task-urile anterioare

📌 Reguli pentru alegerea lanțului Hamiltonian   
✅ Dacă într-o componentă conexă există mai multe lanțuri de aceeași dimensiune:   
   -> Se alege lanțul care pornește dintr-un nod de pe linia cu indicele cel mai mic.  
✅Dacă există mai multe lanțuri care pleacă din aceeași linie, se alege lanțul în care vârful are indicele de coloană cel mai mic.  
   -> Același principiu se aplică pentru întreaga succesiune de vârfuri din lanț.   
✅ Dacă într-un graf există mai multe componente conexe cu lanțuri Hamiltoniene:   
    -> Se ia în considerare componenta cu lanțul cel mai lung.   
❌ Dacă în graf nu există niciun lanț Hamiltonian:  
    -> Afișați lungimea -1.  

--- 
## ✍️ Autori

- 📌 Proiect realizat ca parte din materia de [Proiectarea Algoritmilor].
- 🎓 Universitatea Națională de Știință și Tehnologie Politehnica București - Automatică și Calculatoare - Ingineria Sistemelor
- 🧑‍💻 Autor: *Goșoiu Denisa Ioana*
