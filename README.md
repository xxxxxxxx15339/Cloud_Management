<div align="center">
<pre>
 ██████╗██╗      ██████╗ ██╗   ██╗██████╗     ███╗   ███╗ █████╗ ███╗   ██╗ █████╗  ██████╗ ███████╗███╗   ███╗███████╗███╗   ██╗████████╗
██╔════╝██║     ██╔═══██╗██║   ██║██╔══██╗    ████╗ ████║██╔══██╗████╗  ██║██╔══██╗██╔════╝ ██╔════╝████╗ ████║██╔════╝████╗  ██║╚══██╔══╝
██║     ██║     ██║   ██║██║   ██║██║  ██║    ██╔████╔██║███████║██╔██╗ ██║███████║██║  ███╗█████╗  ██╔████╔██║█████╗  ██╔██╗ ██║   ██║   
██║     ██║     ██║   ██║██║   ██║██║  ██║    ██║╚██╔╝██║██╔══██║██║╚██╗██║██╔══██║██║   ██║██╔══╝  ██║╚██╔╝██║██╔══╝  ██║╚██╗██║   ██║   
╚██████╗███████╗╚██████╔╝╚██████╔╝██████╔╝    ██║ ╚═╝ ██║██║  ██║██║ ╚████║██║  ██║╚██████╔╝███████╗██║ ╚═╝ ██║███████╗██║ ╚████║   ██║   
 ╚═════╝╚══════╝ ╚═════╝  ╚═════╝ ╚═════╝     ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚═╝     ╚═╝╚══════╝╚═╝  ╚═══╝   ╚═╝                                                                                                                                                                                    </pre>

![Version: 1.0](https://img.shields.io/badge/version-1.0-blue.svg)
![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![License: MIT](https://img.shields.io/badge/license-MIT-yellow.svg)
![pandas](https://img.shields.io/badge/pandas-✔-blue.svg?logo=pandas&logoColor=white)
![matplotlib](https://img.shields.io/badge/matplotlib-✔-orange.svg?logo=matplotlib&logoColor=white)
![sqlite3](https://img.shields.io/badge/sqlite3-✔-yellow.svg?logo=sqlite&logoColor=white)
![jupyter](https://img.shields.io/badge/jupyter-✔-f37626.svg?logo=jupyter&logoColor=white)


</div>

# Cloud Management System

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](./LICENSE)
[![C++ Version](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)
[![CMake Version](https://img.shields.io/badge/CMake-3.14+-green.svg)](https://cmake.org/)

<div align="center">

**C++ Kubernetes-style cluster simulator for resource management & pod scheduling**

</div>

Is cloud resource management too complex? Want something that actually works? Try **Cloud Management System**—a C++ application that simulates Kubernetes-style pod scheduling, resource allocation, and cluster metrics visualization.

## Installation

Clone this repo and build with CMake.  
*(Requires C++17, CMake 3.14+)*

```sh
git clone https://github.com/yourusername/Cloud_Management.git
cd Cloud_Management
mkdir build && cd build
cmake ..
make
```

## Usage

1. **Build the project** in the build directory:

   ```sh
   cd build
   make
   ```

2. **Run the simulator**:

   ```sh
   ./src/cloudsim
   ```

3. **Follow the output** to see:

   * **Server resources** (CPU and Memory allocation)
   * **Pod deployment** status
   * **Cluster metrics** with detailed resource usage
   * **Container information** for each deployed pod

The application will then:

1. Create hard-coded pods with containers and resource requirements
2. Deploy pods to available servers based on resource availability
3. Display comprehensive cluster metrics showing resource allocation
4. Show successful deployments and any failed allocations

### Example Session

```txt
$ ./src/cloudsim
=== Creating hard-coded pods ===
=== Deploying pods to cluster ===
Server resources:
  Server1: 4 CPU, 8 Memory
  Server2: 4 CPU, 8 Memory
  Server3: 3 CPU, 4 Memory
Pod requirements:
  web-pod:
    web-container-1: 1 CPU, 0.5 Memory
    web-sidecar-1: 0.5 CPU, 0.25 Memory
  db-pod:
    db-container-1: 2 CPU, 2 Memory
    db-backup-1: 0.5 CPU, 0.5 Memory
    db-monitor-1: 0.25 CPU, 0.125 Memory
  api-pod:
    api-container-1: 1.5 CPU, 1 Memory
    api-cache-1: 0.5 CPU, 0.25 Memory

=== Cluster Metrics ===
Cluster Metrics:
Servers:
[Server: Server1: 4.000000 Initial Cpu, 8.000000 Initial Memory, 2.000000Available Cpu,6.750000Available Mem ]
[Server: Server2: 4.000000 Initial Cpu, 8.000000 Initial Memory, 1.250000Available Cpu,5.375000Available Mem ]
[Server: Server3: 3.000000 Initial Cpu, 4.000000 Initial Memory, 3.000000Available Cpu,4.000000Available Mem ]
Pods:
Pod=[ labels={tier:frontend,app:web} Containers={ [Container: web-container-1: 1.000000 CPU, 0.500000 Memory, nginx:latest, active:true] [Container: web-sidecar-1: 0.500000 CPU, 0.250000 Memory, fluentd:latest, active:true]} ]
Pod=[ labels={tier:backend,app:database} Containers={ [Container: db-container-1: 2.000000 CPU, 2.000000 Memory, mysql:8, active:true] [Container: db-backup-1: 0.500000 CPU, 0.500000 Memory, mysql-backup:latest, active:true] [Container: db-monitor-1: 0.250000 CPU, 0.125000 Memory, prometheus:latest, active:true]} ]
Pod=[ labels={tier:backend,app:api} Containers={ [Container: api-container-1: 1.500000 CPU, 1.000000 Memory, node:16, active:true] [Container: api-cache-1: 0.500000 CPU, 0.250000 Memory, redis:alpine, active:true]} ]

=== Deployment completed successfully! ===
```

## Configuration

* **Server resources**
  Modify server specifications in `src/main.cpp` to change CPU and memory allocation:

  ```cpp
  cluster.addServer(std::make_shared<Server>("Server1", 4.0, 8.0));  // CPU, Memory (GB)
  cluster.addServer(std::make_shared<Server>("Server2", 4.0, 8.0));
  cluster.addServer(std::make_shared<Server>("Server3", 3.0, 4.0));
  ```

* **Pod specifications**
  Edit `data/pods.JSON` for custom pod definitions or modify hard-coded pods in `src/main.cpp`:

  ```cpp
  auto pod1 = std::make_unique<Pod>("web-pod");
  pod1->addContainer(std::make_unique<Container>("web-container-1", 1.0, 0.5, "nginx:latest"));
  pod1->setLabel("app", "web");
  pod1->setLabel("tier", "frontend");
  ```

## Testing

Run the comprehensive test suite:

```sh
cd build
make test
```

Or run individual test components:

```sh
./tests/test_Cluster
./tests/test_Server
./tests/test_Pod
./tests/test_Container
./tests/test_Resource
```

### Test Coverage

- **Resource Allocation**: CPU/memory allocation and boundary testing
- **Exception Handling**: Allocation failures and error conditions  
- **Pod Scheduling**: Successful deployments and failure scenarios
- **Cluster Management**: Server addition and metrics generation
- **Container Operations**: Container creation and resource management

## Development

1. Fork this repo
2. Create a feature branch

   ```sh
   git checkout -b feature/my-change
   ```
3. Install dependencies and build

   ```sh
   mkdir build && cd build
   cmake ..
   make
   ```
4. Make your changes & commit

   ```sh
   git commit -am "Add awesome feature"
   ```
5. Push & open a Pull Request

   ```sh
   git push origin feature/my-change
   ```

## Project Structure

```
Cloud_Management/
├── src/                    # Main source code
│   ├── main.cpp           # Entry point and pod creation
│   ├── CloudUtil.cpp      # Utility functions (display, deployPods)
│   ├── KubernetesCluster.cpp  # Cluster management and scheduling
│   ├── Pod.cpp            # Pod implementation and container management
│   ├── Container.cpp      # Container resource handling
│   ├── Server.cpp         # Server resource allocation
│   ├── Resource.cpp       # Base resource class
│   └── Exceptions.cpp     # Custom exception classes
├── tests/                 # Unit tests
│   ├── test_Cluster.cpp   # KubernetesCluster tests
│   ├── test_Server.cpp    # Server allocation tests
│   ├── test_Pod.cpp       # Pod management tests
│   ├── test_Container.cpp # Container tests
│   └── test_Resource.cpp  # Resource base class tests
├── data/                  # Configuration files
│   └── pods.JSON          # Pod specifications in JSON format
├── external/              # External dependencies
│   └── googletest/        # Google Test framework
└── docs/                  # Documentation and architecture
```

## Dependencies

- **C++17**: Modern C++ features and smart pointers
- **CMake 3.14+**: Build system configuration
- **Google Test**: Unit testing framework
- **nlohmann/json**: JSON parsing library (optional)

## License

Distributed under the MIT License. See [LICENSE](./LICENSE) for details.

## Author

**Your Name** – [GitHub](https://github.com/yourusername)



