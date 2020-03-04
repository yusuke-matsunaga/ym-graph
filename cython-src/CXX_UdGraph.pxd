
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
        UdGraph(int node_num, vector[pair[int, int]]&)
        void resize(int)
        void add_edge(int, int)
        int node_num()
        int edge_num()
        bool is_reflective()
        pair[int, int]& edge(int)
        vector[pair[int, int]]& edge_list()

        @staticmethod
        UdGraph read_dimacs(string&)
        void write_dimacs(string&)

        pair[int, vector[int]] coloring(string&)
        vector[int] independent_set(string&)
        vector[int] max_clique(string&)
