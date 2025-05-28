
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
---
## Task 2
✅ Cerință: Se va creea o stivă în care fiecare element corespunde câte unei generații. Fiecare astfel de generație va fi reprezentată printr-o listă de celule ce și-au schimbat starea față de generația anterioară. Prin urmare, se va construi o stivă de liste.
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

## Task 3
✅ Cerință: Dându-se o inițializare (rădăcina arborelui), se vor creea doi fii: 
Cel din stânga reprezintă generația 1 obținută cu regula nouă, B.
Cel din dreapta reprezintă generația 1 obținută cu regulile obișnuite.
Repetați operația pentru toate nodurile astfel create până la generația K, dată.  
###  **Rule B** (variantă personalizată - pentru task 3 si 4):  
 ### 5️⃣ Orice celulă cu exact doi vecini vii → devine vie.  
### 📌 Formatul fișierelor de intrare este același ca la task-urile anterioare
### 🌲 Arbore binar

![image](https://github.com/user-attachments/assets/185d6a91-771e-460b-8e1d-4345ee623d79)

La fiecare generație:
- Ramura stângă evoluează după **Rule B**.
- Ramura dreaptă evoluează după **Rule A**.   

## Task 4
✅ Cerință:  Se va determina, pentru fiecare nod al arborelui obținut la Taskul 3, până la o generație dată K, cel mai lung lanț Hamiltonian.
Se construiește recursiv un arbore binar de generații.

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
