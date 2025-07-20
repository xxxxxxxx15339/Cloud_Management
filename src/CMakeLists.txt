# 1. Regroupe tous les .cpp en une librairie statique
add_library(cloudsim
    Resource.cpp
    Container.cpp
    Pod.cpp
    Server.cpp
    KubernetesCluster.cpp
    CloudUtil.cpp
    # … ajoutez ici tous vos fichiers .cpp …
)

# 2. Indique où trouver les headers pour les projets qui lient cette librairie
target_include_directories(cloudsim
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}   # pour que tests/ puisse inclure "Resource.hpp"
)

# 3. (optionnel) ajouter d’autres dépendances ici
