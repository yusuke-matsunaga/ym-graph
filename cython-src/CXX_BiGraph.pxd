
### @file CXX_BiGraph.pxd
### @brief CXX_BiGraph 用の pxd ファイル
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2020 Yusuke Matsunaga
### All rights reserved.

from libcpp cimport bool
from libcpp.vector cimport vector
from libcpp.string cimport string


cdef extern from "ym/BiGraph.h" namespace "nsYm" :

    # BiGraph クラスの cython バージョン
    cdef cppclass BiGraph :
        BiGraph()
        void resize(int, int)
        void add_edge(int, int, int)
        int node1_num()
        int node2_num()
        int edge_num()
        int edge_id1(int)
        int edge_id2(int)
        int edge_weight(int)

        @staticmethod
        BiGraph read(string&)
        void write(string&)

        vector[int] max_matching()
