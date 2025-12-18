# 1. Test de la Máquina de Estados
# ------------------------------------------
add_executable(test_statemachine test_statemachine.cpp)

target_link_libraries(test_statemachine PRIVATE GameLogic ${RAYLIB_LIB})

add_test(NAME StateMachineTests COMMAND test_statemachine)


# 2. Test de los Estados del Juego
# ------------------------------------------
add_executable(test_states test_states.cpp)


target_link_libraries(test_states PRIVATE GameLogic ${RAYLIB_LIB})

add_test(NAME GameStateTests COMMAND test_states)