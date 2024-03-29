CPMAddPackage(
  NAME cpp-tree-sitter
  GIT_REPOSITORY https://github.com/nsumner/cpp-tree-sitter.git
  GIT_TAG v0.0.2
)

add_grammar_from_repo(tree-sitter-socialgaming
  https://github.com/nsumner/tree-sitter-socialgaming
  0.0.1
)

add_library(gameConfigLoader
  src/GameConfigLoader.cpp
  src/GameState.cpp
  src/GameStateLoader.cpp
  src/GameSetup.cpp
  src/GameSetupLoader.cpp
  src/Translator.cpp
  src/Evaluator.cpp
)
target_include_directories(gameConfigLoader
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(gameConfigLoader
  tree-sitter-socialgaming
  cpp-tree-sitter
  expression
  userManager
)

install(TARGETS
  ARCHIVE DESTINATION lib
)