
# 🧬 Game of Life - Extended Version

Acest proiect implementează o versiune extinsă a celebrului **Game of Life** conceput de John Conway, utilizând liste, arbori binari și grafuri pentru a analiza evoluția celulelor vii și a calcula drumuri hamiltoniene între ele.

## 📁 Structura Proiectului

- `task1.c`: Implementare clasică Game of Life pe o matrice.
- `task2.c`: Păstrează un istoric al modificărilor celulelor folosind o stivă, permițând reconstruirea stării inițiale.
- `task3.c`: Construiește un arbore binar de generare cu două reguli de evoluție diferite.
- `task4.c` + `necessary.c`: Combină arbori binari și grafuri pentru a construi toate generațiile și a calcula drumul hamiltonian maxim în componentele conexe.

---

## ▶️ Cum se rulează

```
gcc task4.c necessary.c -o task4
./task4 input.txt output.txt
```

## 📥 Format fișier de intrare

```
3 N M G
<matrice N x M>
```

- `3` - identificator pentru acest tip de fișier.
- `N` și `M` - dimensiunile matricei.
- `G` - numărul de generații de procesat.
- `X` - celulă vie
- `+` - celulă moartă

### Exemplu:

```
3 3 3 2
+++
+X+
+X+
```

---

## ⚙️ Detalii Tehnice

### 🔁 Reguli de evoluție

- **Rule A** (Conway):
  - O celulă moartă cu exact 3 vecini vii → devine vie.
  - O celulă vie cu 2 sau 3 vecini → rămâne vie.
  - În rest, moare.

- **Rule B** (variantă personalizată):
  - O celulă vie rămâne vie oricum.
  - O celulă moartă cu exact 2 vecini → devine vie.

### 🌲 Arbore binar

La fiecare generație:
- Ramura stângă evoluează după **Rule B**.
- Ramura dreaptă evoluează după **Rule A**.

Se construiește recursiv un arbore binar de generații.

### 🔗 Grafuri și Drum Hamiltonian

- Se construiește un graf de adiacență între celulele vii vecine.
- Se determină **cel mai lung drum Hamiltonian** (care trece o singură dată prin fiecare celulă dintr-o componentă conexă).

---

## 📤 Format fișier de ieșire

Pentru fiecare generație:
- Se afișează matricea actuală.
- Se afișează lungimea celui mai lung drum Hamiltonian.
- Se listează coordonatele drumului.

### Exemplu:

```
+X+
X+X
+++

4
(0,1) (1,0) (1,2) (2,1)
```

---

## 💡 Bonus Task 2

În `task2.c`, este implementată funcționalitatea de a urmări modificările celulelor în timp, permițând reconstrucția **stării inițiale** pornind de la generația finală și lista de schimbări. Se folosește o **stivă de liste** pentru această revenire în timp.

---

## ✅ Cerințe

- Compilator C (GCC)
- Sistem POSIX (Linux/Mac), dar compatibil și cu Windows (cu mici ajustări)

---

## ✍️ Autori

- 📌 Proiect realizat ca parte din materia de [Algoritmi Fundamentali / Structuri de Date].
- 🎓 Universitatea ... (completare facultativă)
- 🧑‍💻 Autor: *Numele tău aici*
