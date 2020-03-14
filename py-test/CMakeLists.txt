

# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  ${PROJECT_SOURCE_DIR}/ym-base/cython-src
  ${PROJECT_SOURCE_DIR}/ym-graph/cython-src
  )


# ===================================================================
#  マクロの定義
# ===================================================================


# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
#  cython 用のソースファイルの設定
# ===================================================================

set ( CYTHON_SOURCES
  pym_graph.pyx
  )

set_source_files_properties (
  ${CYTHON_SOURCES}
  PROPERTIES CYTHON_IS_CXX TRUE
  )


# ===================================================================
#  module ターゲットの設定
# ===================================================================

cython_add_module ( pym_graph
  ${CYTHON_SOURCES}
  $<TARGET_OBJECTS:ym_base_obj_d>
  $<TARGET_OBJECTS:ym_graph_obj_d>
  )

target_compile_options ( pym_graph
  PRIVATE "-g"
  )


target_link_libraries ( pym_graph
  ${YM_LIB_DEPENDS}
  )


# ===================================================================
#  テストの設定
# ===================================================================

add_test ( py_bigraph_test
  env PYTHONPATH=${PROJECT_BINARY_DIR}/ym-graph/py-test
  ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/bigraph_test.py
  )

add_test ( py_udgraph_max_matching_test
  env PYTHONPATH=${PROJECT_BINARY_DIR}/ym-graph/py-test
  ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/max_matching_test.py
  )
