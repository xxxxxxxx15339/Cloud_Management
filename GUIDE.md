## 🗓️ Day 0 (Prep)

1. **Repo & toolchain**

   * Create a new GitHub repo and clone it.
   * Add a `LICENSE` (MIT) and initial empty `README.md`.
   * Scaffold a CMake project:

     ```
     mkdir build && cd build
     cmake ..
     ```
   * Ensure you can `cmake --build .` and run a dummy `main()`.

2. **Dependencies**

   * Install SQLite dev headers (`libsqlite3-dev`), GoogleTest, Python (for Pandas).
   * Add a `requirements.txt` for your notebook:

     ```txt
     pandas
     matplotlib
     sqlite3
     jupyter
     ```

---

## 📐 Day 1 – Steps 1–3: Core types & Resource abstraction

1. **`Resource` (abstract)**

   * Header `Resource.hpp`:

     ```cpp
     class Resource {
     protected:
       Resource(std::string id, double cpu, double mem);
       virtual ~Resource();
     public:
       virtual void start() = 0;
       virtual void stop()  = 0;
       virtual std::string getMetrics() const = 0;
     };
     ```
   * Implement ctor/dtor, store `id_`, `cpu_`, `mem_`, `active_`.

2. **`Container`**

   * Inherit from `Resource`. Add `image_`.
   * Override `start()`, `stop()`.
   * `getMetrics()` returns `"Container[id] cpu:… mem:… active:…"`
   * Overload `operator<<` for streams.

3. **`Pod` skeleton**

   * Class `Pod` holds `std::vector<std::unique_ptr<Container>>` and `std::unordered_map<std::string,std::string>` labels.
   * Methods `addContainer()`, `startAll()`, `stopAll()`, `getMetrics()`.

4. **Write unit tests** (GoogleTest) for each of the above:

   * Create an empty container, start it, inspect metrics string.
   * Pod with 2 containers → `getMetrics()` contains both.

---

## 🖥️ Day 2 – Steps 4–5: Server & Scheduler

1. **`Server`**

   * Inherit from `Resource`. Add `available_cpu_`, `available_mem_`.
   * Method `bool allocate(double cpu, double mem)`

     * If enough free → subtract & return `true`, else throw `AllocationException`.
   * `getMetrics()` reports free vs total.

2. **`KubernetesCluster`**

   * Holds `std::vector<std::shared_ptr<Server>> nodes_;` and `std::vector<std::unique_ptr<Pod>> pods_;`
   * Implement `schedulePod(std::unique_ptr<Pod> p)`:

     * Loop nodes with first-fit; on success call `p->startAll()`, store pod.
     * On failure of all, throw `AllocationException`.

3. **Tests**

   * Create 2 servers (4 CPU each), a pod with two containers (2+3 CPU) → first goes on server1, second on server2.
   * Assert exceptions thrown when oversubscribed.

---

## 📊 Day 3 – Steps 6–7: CLI & demo

1. **`CloudUtil`**

   * Static `void display(const KubernetesCluster&)` prints metrics.
   * `void deployPods(KubernetesCluster&, std::vector<std::unique_ptr<Pod>>& pods)` loops through all, catches exceptions and continues.

2. **`main.cpp`**

   * Parse a simple JSON or hard-coded list of pod specs.
   * Call `deployPods()`, then `display()`.
   * Wire up `argc/argv` for an input file path (stretch).

3. **Acceptance tests**

   * “Smoke test” script:

     ```sh
     ./cloudsim pods.json | grep “Server[0-9]” 
     ```
   * Ensure exit code `0`.

---

## 🚨 Day 4 – Steps 8–9: Exceptions

1. **Define**

   ```cpp
   struct CloudException : std::exception { ... };
   struct AllocationException : CloudException { ... };
   struct FileException       : CloudException { ... };
   ```

2. **Throw & catch**

   * In `Server::allocate()`, throw `AllocationException("…")`.
   * In `KubernetesCluster::schedulePod`, wrap with try/catch to rethrow or log.
   * In `CloudUtil::deployPods`, catch `AllocationException`, log and continue.

3. **Unit tests**

   * Verify correct exception types propagate and are caught.

---

## 💾 Day 5 – Steps 10–12: Metrics export & formatting

1. **`saveClusterMetrics(cluster, "file.txt")`**

   * Open `std::ofstream`; on failure throw `FileException`.
   * Write each server and pod metrics line by line.

2. **Robust `deployPods`**

   * After catching an exception, clear the pod vector so you don’t retry the same pod.

3. **Aligned output**

   * Use `std::ostringstream`, `std::setw()`, `std::left` to align columns in both console and file.

4. **Manual test**

   * Run, open `file.txt`, visually verify neat columns.

---

## 🧩 Day 6 – Steps 13–14: Templates & Lambdas

1. **`MetricLogger<T>` template**

   * In a header-only file:

     ```cpp
     template<class T>
     struct MetricLogger {
       std::ostream& os_;
       MetricLogger(std::ostream& os): os_(os) {}
       void log(const T& obj) { os_ << obj.getMetrics() << "\n"; }
     };
     ```
   * Test with `Container`, `Server`, `Pod`.

2. **Utility lambdas**

   * `auto getFilteredServers = [&](double cpuThreshold){…};` returns vector of servers above threshold.
   * `auto forEachContainer = [&](auto&& fn){…};` invokes `fn(container)` for all containers in cluster.
   * Use `std::sort` and lambdas to sort pods by total CPU usage.

3. **Mini-demo** in `main.cpp` showing these utilities in action.

---

## 📚 Day 7 – SQLite & Pandas notebook

1. **SQLite schema**

   * Create `cluster.db` and run the DDL from §5.1 to make `servers`, `pods`, `containers` tables.

2. **Export into DB**

   * Write a function `void persistCluster(const KubernetesCluster&, sqlite3* db)`

     * Insert all servers, pods, containers.
     * Wrap errors in `FileException`.

3. **Jupyter notebook**

   * In `analysis.ipynb`, connect with

     ```python
     import sqlite3, pandas as pd
     con = sqlite3.connect('cluster.db')
     servers = pd.read_sql('SELECT * FROM servers', con)
     ```
   * Reproduce the example heat-map / time-series plots.

4. **Final polish**

   * Add diagrams (PlantUML) to `README.md`: class and sequence diagrams.
   * Configure **GitHub Actions**: build, test, coverage, lint.
   * Add badges (build | tests | coverage | license).

---

### ✅ After Week: Final Checklist

* [ ] All 14 steps implemented, tested, and documented.
* [ ] CMake build passes on Linux & Windows.
* [ ] GitHub Actions green.
* [ ] `cluster.db` + `analysis.ipynb` in repo.
* [ ] README with quickstart, diagrams, badges.
* [ ] Blog post link or demo video (optional but highly recommended).

Bonne chance ! 🚀
