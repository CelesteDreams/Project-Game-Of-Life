
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
📦 Conținut stivă la generația 1:
1: (1,2); (3,2); (3,3)

📦 Conținut stivă la generația 2:
1: (1 ,2); (3 ,2); (3 ,3)

2: (2 ,1); (2 ,2); (2 ,3); (3 ,1)

### 🌲 Arbore binar

La fiecare generație:
- Ramura stângă evoluează după **Rule B**.
- Ramura dreaptă evoluează după **Rule A**. 
  - **Rule B** (variantă personalizată - pentru task 3 si 4):  
  5️⃣ Orice celulă cu exact doi vecini vii → devine vie.   


Se construiește recursiv un arbore binar de generații.
Pentru fiecare generație:
- Se afișează matricea actuală.
- Se afișează lungimea celui mai lung drum Hamiltonian.
- Se listează coordonatele drumului.
---

## 💡 Bonus Task 2

În `task2.c`, este implementată funcționalitatea de a urmări modificările celulelor în timp, permițând reconstrucția **stării inițiale** pornind de la generația finală și lista de schimbări. Se folosește o **stivă de liste** pentru această revenire în timp.

---
##TASK 3
##TASK 4
## 


- Compilator C (GCC)
- Sistem POSIX (Linux/Mac), dar compatibil și cu Windows (cu mici ajustări)

---

## ✍️ Autori

- 📌 Proiect realizat ca parte din materia de [Algoritmi Fundamentali / Structuri de Date].
- 🎓 Universitatea ... (completare facultativă)
- 🧑‍💻 Autor: *Numele tău aici*
