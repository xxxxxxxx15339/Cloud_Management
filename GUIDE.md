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

## 🗓️ Day 8 – Expose a REST/gRPC Control API

**Goal:**
Give external clients a clear, machine‑readable way to list servers/pods, schedule new pods, and query metrics.

**Technologies Used:**

* **OpenAPI (YAML):** Standard format to describe RESTful HTTP APIs.
* **gRPC & Protocol Buffers:** High‑performance RPC framework with strongly‑typed messages.
* **swagger-cli:** Validates & bundles OpenAPI specs.
* **protoc:** Compiles `.proto` files into C++ stubs.
* **cpp-httplib / Pistache:** Header‑only C++ HTTP server libraries.
* **gRPC C++ library:** Official gRPC implementation in C++.
* **Dredd / Schemathesis:** Contract‑testing tools for REST.
* **grpcurl:** CLI to exercise gRPC endpoints.

**Step‑by‑Step Guide:**

1. **Design your API**

   * REST: create `api/openapi.yaml` with paths `/servers` (GET), `/schedule` (POST), response schemas.
   * gRPC: create `api/control.proto` with service methods:

     ```proto
     syntax = "proto3";
     service Control {
       rpc ListServers(Empty) returns (ServerList);
       rpc SchedulePod(PodSpec) returns (ScheduleResult);
     }
     ```

2. **Validate & generate code**

   * REST:

     ```bash
     npm install -g @apidevtools/swagger-cli
     swagger-cli validate api/openapi.yaml
     ```
   * gRPC:

     ```bash
     protoc \
       --proto_path=api \
       --cpp_out=gen \
       --grpc_out=gen \
       --plugin=protoc-gen-grpc=$(which grpc_cpp_plugin) \
       api/control.proto
     ```

3. **Add server library**

   * REST (`cpp-httplib`):

     ```bash
     git submodule add https://github.com/yhirose/cpp-httplib.git extern/httplib
     ```

     In `CMakeLists.txt`: `add_subdirectory(extern/httplib)` and link `httplib::httplib`.
   * gRPC:

     ```cmake
     find_package(gRPC CONFIG REQUIRED)
     find_package(Protobuf CONFIG REQUIRED)
     ```

     Link `grpc++` and `protobuf::libprotobuf`.

4. **Implement handlers**

   * **REST example** (`src/api.cpp`):

     ```cpp
     httplib::Server svr;
     svr.Get("/servers", [&](auto&, auto& res){
       res.set_content(cluster.getAllMetrics(), "text/plain");
     });
     svr.Post("/schedule", [&](auto& req, auto& res){
       PodSpec spec = parseJson(req.body);
       auto result = cluster.schedulePod(spec);
       res.set_content(result.toJson(), "application/json");
     });
     svr.listen("0.0.0.0", 8080);
     ```
   * **gRPC example**:

     ```cpp
     class ControlServiceImpl final : public Control::Service {
       Status ListServers(ServerContext*, const Empty*, ServerList* out) override {
         *out = cluster.toProto();
         return Status::OK;
       }
       // ...
     };
     ```

5. **Build & run**

   ```bash
   cd build && cmake .. && make
   ./cloudsim_api
   ```

   Test with:

   ```bash
   curl http://localhost:8080/servers
   grpcurl -plaintext localhost:50051 Control.ListServers
   ```

6. **Write contract tests**

   * **REST:** create `dredd.yml` pointing to your spec and `http://localhost:8080`, then `dredd`.
   * **gRPC:** script calls each RPC via `grpcurl` and checks for `OK`.

---

## 🗓️ Day 9 – Containerize & Kubernetes Deployment

**Goal:**
Package your app in Docker and run it in a local Kubernetes cluster to demonstrate cloud‑native deployment.

**Technologies Used:**

* **Docker CE:** Build container images.
* **kind / k3s:** Lightweight local Kubernetes cluster.
* **Kubernetes manifests:** YAML for Deployment & Service.
* **Helm:** Templating engine for Kubernetes resources.

**Step‑by‑Step Guide:**

1. **Write a multi‑stage Dockerfile**

   ```dockerfile
   # Builder stage
   FROM ubuntu:24.04 AS builder
   RUN apt update && apt install -y build-essential cmake libsqlite3-dev
   WORKDIR /app
   COPY . .
   RUN mkdir build && cd build && cmake .. && make

   # Runtime stage
   FROM ubuntu:24.04
   RUN apt update && apt install -y libsqlite3-0
   COPY --from=builder /app/build/cloudsim_api /usr/local/bin/cloudsim_api
   EXPOSE 8080
   ENTRYPOINT ["cloudsim_api"]
   ```

2. **Build & test image**

   ```bash
   docker build -t cloudsim:latest .
   docker run --rm -p 8080:8080 cloudsim:latest
   curl http://localhost:8080/servers
   ```

3. **Install kind**

   ```bash
   curl -Lo kind https://kind.sigs.k8s.io/dl/v0.21.0/kind-linux-amd64
   chmod +x kind && mv kind /usr/local/bin/
   kind create cluster --name cloudsim-cluster
   ```

4. **Write Kubernetes manifests** (`k8s/deployment.yaml`, `k8s/service.yaml`):

   ```yaml
   # deployment.yaml
   apiVersion: apps/v1
   kind: Deployment
   metadata: { name: cloudsim }
   spec:
     replicas: 1
     selector: { matchLabels: { app: cloudsim } }
     template:
       metadata: { labels: { app: cloudsim } }
       spec:
         containers:
           - name: cloudsim
             image: cloudsim:latest
             ports: [{ containerPort: 8080 }]
   ```

   ```yaml
   # service.yaml
   apiVersion: v1
   kind: Service
   metadata: { name: cloudsim-svc }
   spec:
     type: NodePort
     selector: { app: cloudsim }
     ports: [{ port: 8080, targetPort: 8080 }]
   ```

5. **Deploy to kind**

   ```bash
   kubectl apply -f k8s/deployment.yaml
   kubectl apply -f k8s/service.yaml
   kubectl get pods
   ```

6. **(Optional) Helm packaging**

   ```bash
   helm create cloudsim-chart
   ```

   Move your YAML into `cloudsim-chart/templates/` and configure `values.yaml`, then:

   ```bash
   helm install cloudsim ./cloudsim-chart
   ```

---

## 🗓️ Day 10 – Observability & Monitoring

**Goal:**
Instrument your service so you can collect real‑time metrics and visualize them, proving you understand production‑grade observability.

**Technologies Used:**

* **prometheus-cpp:** C++ client to expose Prometheus metrics.
* **Prometheus:** Metrics collection & alerting.
* **Grafana:** Dashboarding & visualization.
* **Alertmanager:** Handle and route alerts.

**Step‑by‑Step Guide:**

1. **Add prometheus-cpp**

   ```bash
   git submodule add https://github.com/jupp0r/prometheus-cpp.git extern/prom-cpp
   ```

   In CMakeLists, add and link `prometheus-cpp`.

2. **Instrument code**

   ```cpp
   auto registry = std::make_shared<prometheus::Registry>();
   prometheus::Exposer exposer{"0.0.0.0:9100"};
   exposer.RegisterCollectable(registry);

   auto& cpu_gauge = prometheus::BuildGauge()
     .Name("server_cpu_free")
     .Register(*registry)
     .Add({{"server", id_}});
   cpu_gauge.Set(available_cpu_);
   ```

3. **Run Prometheus**

   ```bash
   # prometheus.yml scrapes http://localhost:9100/metrics
   docker run -d --name prometheus -p 9090:9090 \
     -v $(pwd)/prometheus.yml:/etc/prometheus/prometheus.yml \
     prom/prometheus
   ```

4. **Run Grafana**

   ```bash
   docker run -d --name grafana -p 3000:3000 grafana/grafana
   ```

   * Log in (admin/admin).
   * Add Prometheus as data source.
   * Build a dashboard plotting `server_cpu_free`.

5. **Configure alerts**
   In `prometheus.yml`:

   ```yaml
   rule_files:
     - "alert.rules.yml"
   ```

   In `alert.rules.yml`:

   ```yaml
   groups:
     - name: allocation.rules
       rules:
         - alert: AllocationFailures
           expr: increase(allocation_failures[5m]) > 0
           for: 1m
   ```

---

## 🗓️ Day 11 – Performance Testing & Benchmarking

**Goal:**
Measure and guarantee performance of core operations, and prevent regressions.

**Technologies Used:**

* **Google Benchmark:** Micro‑benchmarking C++ code.
* **k6 OSS:** Load‑testing HTTP endpoints.
* **GitHub Actions:** Automate benchmarks in CI.

**Step‑by‑Step Guide:**

1. **Add Google Benchmark**

   ```bash
   git submodule add https://github.com/google/benchmark.git extern/benchmark
   ```

   In CMakeLists, link `benchmark` and `benchmark_main`.

2. **Write a benchmark** (`benchmarks/allocate_benchmark.cpp`):

   ```cpp
   static void BM_Allocate(benchmark::State& st) {
     Server s("s1", 100, 100);
     for (auto _ : st) s.allocate(1,1);
   }
   BENCHMARK(BM_Allocate);
   BENCHMARK_MAIN();
   ```

3. **Build & run**

   ```bash
   cd build && cmake .. && make benchmarks
   ./benchmarks --benchmark_report_aggregates_only=true
   ```

4. **Load test with k6**

   ```bash
   npm install -g k6
   ```

   Create `load_test.js`:

   ```js
   import http from 'k6/http';
   export default () => { http.get('http://localhost:8080/servers'); };
   ```

   Run:

   ```bash
   k6 run --vus 50 --duration 30s load_test.js
   ```

5. **CI integration**
   In `.github/workflows/ci.yml`, add steps to build & run `./benchmarks`, and fail if average latency exceeds your threshold.

---

## 🗓️ Day 12 – Security & Robustness

**Goal:**
Harden your code with fuzzing, sanitizers, and static analysis to catch bugs early.

**Technologies Used:**

* **libFuzzer / AFL++:** Automated fuzz testing.
* **Address & Undefined Behavior Sanitizers:** Catch memory errors at runtime.
* **clang‑tidy & cppcheck:** Static code analysis.

**Step‑by‑Step Guide:**

1. **Enable sanitizers** in your `CMakeLists.txt`:

   ```cmake
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined -fno-omit-frame-pointer")
   ```

2. **Write a fuzz harness** (`fuzz/fuzz_schedule.cpp`):

   ```cpp
   extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
     std::string s((char*)data, size);
     try { cluster.schedulePod(parsePodSpec(s)); } catch(...) {}
     return 0;
   }
   ```

   Compile with `-fsanitize=fuzzer,address`.

3. **Run static analysis**

   ```bash
   clang-tidy src/*.cpp -- -std=c++17
   cppcheck --enable=all src/
   ```

4. **Guard against malformed input**
   Wrap your JSON parsing:

   ```cpp
   try {
     auto j = nlohmann::json::parse(input);
   } catch (nlohmann::json::exception& e) {
     // log error and return HTTP 400 / gRPC error
   }
   ```

---

## 🗓️ Day 13 – Infra‑as‑Code & Cloud Deployment

**Goal:**
Provision and manage your entire infrastructure (K8s cluster, database, storage) as code.

**Technologies Used:**

* **Terraform OSS:** Declarative IaC to spin up resources.
* **Helm Provider / Kubernetes Provider:** Terraform plugins for K8s.
* **GitHub Actions:** Automate `terraform apply` & deployments.

**Step‑by‑Step Guide:**

1. **Install Terraform CLI** (v1.5+).

2. **Create `infra/main.tf`**:

   ```hcl
   provider "kubernetes" { /* use kubeconfig to point at your cluster */ }

   resource "kubernetes_namespace" "cloudsim" {
     metadata { name = "cloudsim" }
   }

   resource "kubernetes_deployment" "app" {
     metadata { name = "cloudsim"; namespace = kubernetes_namespace.cloudsim.metadata[0].name }
     spec { /* copy from k8s/deployment.yaml */ }
   }
   ```

3. **Initialize & apply**:

   ```bash
   cd infra
   terraform init
   terraform apply -auto-approve
   ```

4. **Migrate to Postgres (optional)**

   * Use Terraform’s **Helm provider** to install the official Postgres chart.
   * Update your code to use `libpq` (C++) or `psycopg2` (Python).

5. **GitHub Actions automation** (`.github/workflows/infra.yml`):

   ```yaml
   on: push
   jobs:
     terraform:
       runs-on: ubuntu-latest
       steps:
         - uses: actions/checkout@v3
         - uses: hashicorp/setup-terraform@v2
           with: terraform_version: 1.5.0
         - run: terraform init
         - run: terraform apply -auto-approve
   ```

---

## 🗓️ Day 14 – Technical Writing & Demo

**Goal:**
Document your architecture decisions, diagrams, and produce a polished demo to showcase end‑to‑end mastery.

**Technologies Used:**

* **ADR (Markdown):** Simple records of architecture decisions.
* **PlantUML:** Text‑based UML diagrams.
* **GitHub Pages & Reveal.js:** Free hosting for slide decks.
* **OBS Studio:** Open‑source screen recording.

**Step‑by‑Step Guide:**

1. **Create ADRs** under `docs/adr/` using this template:

   ```markdown
   # ADR 0001: Choose cpp-httplib for REST
   Date: 2025‑07‑21

   ## Context
   …

   ## Decision
   …

   ## Consequences
   …
   ```

2. **Draw PlantUML diagrams**

   * `docs/diagrams/class.puml`:

     ```puml
     @startuml
     class Server {
       - available_cpu_: double
       + allocate(cpu,mem)
     }
     @enduml
     ```
   * Generate PNG: `plantuml docs/diagrams/class.puml`

3. **Publish slides**

   * Initialize a Reveal.js template in `slides/`.
   * In repo settings, enable GitHub Pages from `gh-pages` branch.
   * Push slides; they’ll be live at `https://<your‑user>.github.io/<repo>/`.

4. **Record a 30 s demo**

   * Install **OBS Studio**.
   * Capture:

     1. `kubectl apply …` or `helm install …`
     2. `curl` or `grpcurl` output
     3. Grafana dashboard
   * Export MP4 and link it in your README under **Demo Video**.

---


Bonne chance ! 🚀
