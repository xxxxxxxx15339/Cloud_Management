# 🌩️ Mini-Projet C++ — Simulateur d’Infrastructure Cloud Minimaliste

*“Tout comprendre, tout construire, tout analyser… en **1 semaine**”*

---

## 1 · Contexte général

La consigne universitaire (Imad Kissami, avril 2025) demande de **modéliser une plate-forme cloud simplifiée** : serveurs → pods → conteneurs, gérés par un scheduler façon *Kubernetes*, avec métriques exportables et gestion d’erreurs robuste.
Le prolongement “Suite” ajoute exceptions personnalisées, export fichier, templates et lambdas.

### Pourquoi ce mini-projet vaut de l’or ?

| Compétence              | Ce que vous pratiquerez                                | Utilité CV                  |
| ----------------------- | ------------------------------------------------------ | --------------------------- |
| **Modern C++ 17/20**    | RAII, smart pointers, exceptions, templates, STL algos | Entretien dev système       |
| **Design OOP**          | Architecture multi-niveaux, polymorphisme              | Questions “design a system” |
| **SQL + SQLite**        | Schéma relationnel, requêtes analytiques               | Postes data/BI              |
| **Pandas + Matplotlib** | Dashboard ressources, heat-maps                        | Stages data-science         |

---

## 2 · Modèle conceptuel

```
Resource (abstract)
│  id_, cpu_, memory_, active_
├─ Container  (image_)
├─ Server     (available_cpu_, available_mem_)
└─ (Pod is not a Resource but owns Containers)

KubernetesCluster
├─ nodes_ : vector<shared_ptr<Server>>
└─ pods_  : vector<unique_ptr<Pod>>
```

* Toute ressource possède `start()`, `stop()` et `getMetrics()` (étape 1).
* **Pod** agrège des `Container`, déploie l’ensemble, expose ses métriques (étape 3).
* **Server** gère un pool CPU/RAM, alloue via `allocate()` et peut être actif ou inactif (étape 4).
* **KubernetesCluster** choisit un serveur via `schedulePod()` et orchestre tout (étape 5).

---

## 3 · Parcours détaillé des 14 étapes obligatoires

| #      | Tâche                       | Points d’attention                                                               |
| ------ | --------------------------- | -------------------------------------------------------------------------------- |
| **1**  | `Resource` abstraite        | Constructeur protégé, destructeur virtuel, *pas d’état statique*                 |
| **2**  | `Container`                 | Surcharger `<<` et `getMetrics()` (même format)                                  |
| **3**  | `Pod`                       | Stocker `unique_ptr<Container>`, labels `unordered_map`                          |
| **4**  | `Server`                    | `allocate()` retourne *bool* ; maj des champs disponibles ; exceptions (étape 9) |
| **5**  | `KubernetesCluster`         | Implémenter `schedulePod()` (first-fit), `getMetrics()` agrégé                   |
| **6**  | `Cloud_Util`                | `display()` + `deployPods()` (boucle + messages)                                 |
| **7**  | `main.cpp` démo & tests 0-3 | Script complet fourni dans le PDF                                                |
| **8**  | Exceptions perso            | `CloudException → AllocationException / FileException`                           |
| **9**  | Exceptions intégrées        | Lever/catcher `AllocationException` dans `allocate` et `deployPod`               |
| **10** | Export métriques            | `saveClusterMetrics(cluster,"file.txt")` + `FileException`                       |
| **11** | `deployPods` robuste        | Continue après exception, vide le vecteur                                        |
| **12** | Affichage aligné            | `ostringstream`, `setw`, `left` dans tous les `getMetrics()`                     |
| **13** | `MetricLogger<T>`           | Template écrivant `obj.getMetrics()` vers n’importe quel flux                    |
| **14** | Lambdas utilitaires         | `getFilteredServers`, `forEachContainer`, tri des pods avec `std::sort`          |

---

## 4 · Feuille de route **1 semaine**

| Jour  | Livrable                    | Focus C++                           | Focus Data              |
| ----- | --------------------------- | ----------------------------------- | ----------------------- |
| **1** | Étapes 1–3                  | RAII, smart pointers                | —                       |
| **2** | Étapes 4–5                  | Algorithmes STL, polymorphisme      | —                       |
| **3** | Étapes 6–7 + tests PDF      | I/O stream, `ostream<<`             | —                       |
| **4** | Exceptions (8-9)            | `try/catch`, hiérarchie d’erreurs   | —                       |
| **5** | Export (10-11-12)           | `fstream`, formatage                | CSV généré              |
| **6** | Templates & lambdas (13-14) | `template<class T>`, `<functional>` | Début notebook Pandas   |
| **7** | SQLite + notebook           | API C `sqlite3`, schéma + INSERT    | Visualisation, heat-map |

---

## 5 · Approfondissements pour **SQL** et **Pandas**

### 5.1 Schéma de persistance

```sql
CREATE TABLE servers(
  id TEXT PRIMARY KEY,
  total_cpu REAL, total_mem REAL,
  free_cpu  REAL, free_mem  REAL,
  active INTEGER
);

CREATE TABLE pods(
  name TEXT PRIMARY KEY,
  server_id TEXT,
  FOREIGN KEY(server_id) REFERENCES servers(id)
);

CREATE TABLE containers(
  id TEXT PRIMARY KEY,
  pod_name TEXT,
  cpu REAL, mem REAL, image TEXT,
  FOREIGN KEY(pod_name) REFERENCES pods(name)
);
```

### 5.2 Requêtes utiles

**CPU > 80 %**

```sql
SELECT id,
       100*(1 - free_cpu/total_cpu) AS cpu_usage
FROM   servers
WHERE  cpu_usage > 80;
```

**Images les plus gourmandes**

```sql
SELECT image,
       SUM(mem) AS mem_total
FROM   containers
GROUP  BY image
ORDER  BY mem_total DESC
LIMIT  3;
```

### 5.3 Analyse Pandas

```python
import pandas as pd, matplotlib.pyplot as plt, sqlite3

con = sqlite3.connect('data/cluster.db')
df  = pd.read_sql('SELECT * FROM servers', con, parse_dates=['timestamp'])
df['cpu_used'] = df.total_cpu - df.free_cpu

(df.pivot_table(index='timestamp',
                columns='id',
                values='cpu_used')
   .plot(title='CPU Used per Server'))

plt.show()
```

---

## 6 · Conseils d’implémentation approfondis

| Sujet                | Bonnes pratiques                                                        |
| -------------------- | ----------------------------------------------------------------------- |
| **Ownership**        | `unique_ptr` pour conteneurs, `shared_ptr` pour serveurs                |
| **Scheduler**        | *First-fit* : premier serveur actif suffisant                           |
| **Exception safety** | Utiliser `make_unique` / `make_shared`, catcher par référence constante |
| **MetricLogger**     | Header-only :<br>`template<class T> struct MetricLogger { … }`          |
| **Tests**            | GoogleTest ou `assert` ; reproduire les tests du PDF                    |

---

## 7 · Stretch-Goals (facultatifs mais formateurs)

1. **Thread-Pool** : planifier `deployPod` en parallèle avec `std::jthread`.
2. **JSON export** : remplacer CSV par `nlohmann::json`.
3. **REST API** : exposer `GET /metrics` via Pistache ou Crow.
4. **Heat-map mémoire** Pandas : visualiser `mem_used` par serveur & timestamp.

---

## 8 · Checklist finale avant rendu

* [ ] Compilation via CMake + make sans warnings.
* [ ] `main.cpp` reproduit **exactement** les sorties d’exemple.
* [ ] `cluster1_metrics.txt` ou `cluster.db` généré.
* [ ] README, diagrammes PlantUML, notebook `.ipynb` committés.
* [ ] GitHub Actions CI vert (build + tests).

> **Mot de la fin:** en 7 jours, vous passerez d’un squelette OOP à une mini-plate-forme cloud instrumentée et analysable. Gardez le périmètre strict, écrivez des tests chaque soir, et ajoutez les bonus uniquement lorsque les 14 étapes sont **100 % stables**.

Bon code ! 🚀
