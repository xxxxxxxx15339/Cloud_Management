# 1. Google Test est déjà configuré dans le CMakeLists.txt principal
# Pas besoin de find_package car nous utilisons add_subdirectory

# 2. Liste des fichiers de tests
set(TEST_SOURCES
    test_Resource.cpp
    test_Container.cpp
    test_Pod.cpp
    test_Server.cpp
    # test_Cluster.cpp etc.
)

# 3. Pour chaque fichier de test, on crée un exécutable
foreach(src_file IN LISTS TEST_SOURCES)
    # ex: src_file = test_Container.cpp  => test_name = test_Container
    get_filename_component(test_name ${src_file} NAME_WE)
    add_executable(${test_name} ${src_file})
    
    # 4. On lie à la fois cloudsim et GoogleTest
    target_link_libraries(${test_name}
        PRIVATE
            cloudsim
            gtest
            gtest_main
    )
    
    # 5. On enregistre le test pour ctest
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()
