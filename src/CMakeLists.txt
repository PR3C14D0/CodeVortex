# CMakeList.txt: proyecto de CMake para CodeVortex, incluya el origen y defina
# la lógica específica del proyecto aquí.
#

# Agregue un origen al ejecutable de este proyecto.
add_executable (CodeVortex "main.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CodeVortex PROPERTY CXX_STANDARD 20)
  set_property(TARGET CodeVortex PROPERTY WIN32_EXECUTABLE TRUE)
endif()

set("PUBLIC_INCLUDES" "${CMAKE_CURRENT_SOURCE_DIR}/../include")
set("IMGUI_FOLDER" "${PUBLIC_INCLUDES}/imgui")

target_include_directories(CodeVortex PUBLIC "${PUBLIC_INCLUDES}")

file(GLOB "IMGUI_SRC"
	"${IMGUI_FOLDER}/*.cpp"
	"${IMGUI_FOLDER}/*.h"
)

add_library("ImGui" STATIC "${IMGUI_SRC}")

target_link_libraries(CodeVortex ImGui)
target_link_libraries(CodeVortex "d3d11.lib;dxgi.lib")
