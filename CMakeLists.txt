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
  include
  private_include
  )


# ===================================================================
#  マクロの定義
# ===================================================================


# ===================================================================
# サブディレクトリの設定
# ===================================================================

add_subdirectory ( tests/c++/udgraph )
add_subdirectory ( tests/c++/bigraph )
add_subdirectory ( tests/python )


# ===================================================================
#  ソースの設定
# ===================================================================

set ( udgraph_SOURCES
  c++-srcs/udgraph/UdGraph.cc
  )

set ( coloring_SOURCES
  c++-srcs/coloring/coloring.cc
  c++-srcs/coloring/ColGraph.cc
  c++-srcs/coloring/Dsatur.cc
  c++-srcs/coloring/IsCov.cc
  c++-srcs/coloring/Isx.cc
  c++-srcs/coloring/Isx2.cc
  c++-srcs/coloring/TabuCol.cc
  )

set ( indep_set_SOURCES
  c++-srcs/indep_set/indep_set.cc
  )

set ( max_clique_SOURCES
  c++-srcs/max_clique/max_clique.cc
  c++-srcs/max_clique/MclqSolver.cc
  c++-srcs/max_clique/MclqSolver_greedy.cc
  c++-srcs/max_clique/MclqSolver_exact.cc
  )

set ( max_matching_SOURCES
  c++-srcs/max_matching/max_matching.cc
  )

set ( bigraph_SOURCES
  c++-srcs/bigraph/BiGraph.cc
  c++-srcs/bigraph/max_matching.cc
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
