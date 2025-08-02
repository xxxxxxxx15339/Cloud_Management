# 1. Regroupe tous les .cpp en une librairie statique
add_library(cloudsim_lib
    Resource.cpp
    Container.cpp
    Pod.cpp
    Server.cpp
    KubernetesCluster.cpp
    CloudUtil.cpp
    Exceptions.cpp
)

# 2. Indique où trouver les headers pour les projets qui lient cette librairie
target_include_directories(cloudsim_lib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}   # pour que tests/ puisse inclure "Resource.hpp"
)

# 3. Create main executable
add_executable(cloudsim main.cpp)
target_link_libraries(cloudsim cloudsim_lib)

# 4. Add JSON library manually (header-only)
target_include_directories(cloudsim
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../external/json/include
)
