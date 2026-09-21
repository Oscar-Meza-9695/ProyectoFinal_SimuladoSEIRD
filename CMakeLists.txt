cmake_minimum_required(VERSION 3.16)
project(MeaslesSimulator VERSION 1.0.0 LANGUAGES CXX)

# ── Estándar C++ ─────────────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── Flags de compilación ──────────────────────────────────────────────────────
if(MSVC)
    add_compile_options(/W4 /O2)
else()
    add_compile_options(-Wall -Wextra -O2)
endif()

# ── SFML ─────────────────────────────────────────────────────────────────────
set(SFML_DIR "C:/SFML-2.6.2/lib/cmake/SFML")
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

# ── Directorios de includes ───────────────────────────────────────────────────
include_directories(
    ${PROJECT_SOURCE_DIR}/Bibliotecas/motor
    ${PROJECT_SOURCE_DIR}/Bibliotecas
    C:/SFML-2.6.2/include
)

# ── Fuentes del motor ─────────────────────────────────────────────────────────
set(ENGINE_SOURCES
    src/motor/ModeloSEIRD.cpp
    src/motor/EDOResolve.cpp
    src/motor/SimulacionRunner.cpp
)

# ── Fuentes de la UI ──────────────────────────────────────────────────────────
set(UI_SOURCES
    src/ui/Slider.cpp
    src/ui/PanelParametros.cpp
    src/ui/PanelGrafica.cpp
    src/ui/PanelAgentes.cpp
    src/ui/PanelEstado.cpp
    src/ui/Ventana.cpp
)

# ── Ejecutable principal ──────────────────────────────────────────────────────
add_executable(measles_sim
    src/main.cpp
    ${ENGINE_SOURCES}
    ${UI_SOURCES}
)

# ── Enlazar SFML ─────────────────────────────────────────────────────────────
target_link_libraries(measles_sim
    sfml-graphics
    sfml-window
    sfml-system
)

# ── Copiar DLLs de SFML al directorio de build (necesario en Windows) ─────────
if(WIN32)
    add_custom_command(TARGET measles_sim POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "C:/SFML-2.6.2/bin/sfml-graphics-2.dll"
            "C:/SFML-2.6.2/bin/sfml-window-2.dll"
            "C:/SFML-2.6.2/bin/sfml-system-2.dll"
            $<TARGET_FILE_DIR:measles_sim>
    )
endif()

# ── Mensaje de configuración ──────────────────────────────────────────────────
message(STATUS "Proyecto: ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")