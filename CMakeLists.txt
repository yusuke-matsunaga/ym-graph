# ===================================================================
# ym-graph のための CMakeLists.txt
# ===================================================================


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  ${PROJECT_BINARY_DIR}/include
  ${PROJECT_SOURCE_DIR}/ym-common/include
  ${PROJECT_SOURCE_DIR}/ym-base/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/private_include
  )


# ===================================================================
#  マクロの定義
# ===================================================================


# ===================================================================
# サブディレクトリの設定
# ===================================================================

add_subdirectory ( c++-test/udgraph )
add_subdirectory ( c++-test/bigraph )
add_subdirectory ( py-test )


# ===================================================================
#  ソースの設定
# ===================================================================

set ( udgraph_SOURCES
  c++-src/udgraph/UdGraph.cc
  )

set ( coloring_SOURCES
  c++-src/coloring/coloring.cc
  c++-src/coloring/ColGraph.cc
  c++-src/coloring/Dsatur.cc
  c++-src/coloring/IsCov.cc
  c++-src/coloring/Isx.cc
  c++-src/coloring/Isx2.cc
  c++-src/coloring/TabuCol.cc
  )

set ( indep_set_SOURCES
  c++-src/indep_set/indep_set.cc
  )

set ( max_clique_SOURCES
  c++-src/max_clique/max_clique.cc
  c++-src/max_clique/MclqSolver.cc
  c++-src/max_clique/MclqSolver_greedy.cc
  c++-src/max_clique/MclqSolver_exact.cc
  )

set ( max_matching_SOURCES
  c++-src/max_matching/max_matching.cc
  )

set ( bigraph_SOURCES
  c++-src/bigraph/BiGraph.cc
  c++-src/bigraph/max_matching.cc
  )

set ( ym_graph_SOURCES
  ${udgraph_SOURCES}
  ${coloring_SOURCES}
  ${indep_set_SOURCES}
  ${max_clique_SOURCES}
  ${max_matching_SOURCES}
  ${bigraph_SOURCES}
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================

ym_add_object_library ( ym_graph
  ${ym_graph_SOURCES}
  )
