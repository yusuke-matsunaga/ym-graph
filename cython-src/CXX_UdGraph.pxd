
### @file CXX_UdGraph.pxd
### @brief CXX_UdGraph 用の pxd ファイル
### @author Yusuke Matsunaga (松永 裕介)
###
### Copyright (C) 2018 Yusuke Matsunaga
### All rights reserved.

from libcpp cimport bool
from libcpp.vector cimport vector
from libcpp.pair cimport pair
from libcpp.string cimport string


cdef extern from "ym/UdGraph.h" namespace "nsYm" :

    # UdGraph クラスの cython バージョン
    cdef cppclass UdGraph :
        UdGraph()
        void resize(int)
        void add_edge(int, int, int)
        int node_num()
        int edge_num()
        bool is_reflective()
        int edge_id1(int)
        int edge_id2(int)
        int edge_weight(int)

        @staticmethod
        UdGraph read_dimacs(string&)
        void write_dimacs(string&)
        @staticmethod
        UdGraph restore(string&)
        void dump(string&)

        pair[int, vector[int]] coloring(string&)
        vector[int] independent_set(string&)
        vector[int] max_clique(string&)
        vector[int] max_matching(string&)
