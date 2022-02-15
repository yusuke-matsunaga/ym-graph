
/// @file UdGraph.cc
/// @brief UdGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015, 2018, 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ym/UdGraph.h"
#include "ym/Range.h"
#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_UDGRAPH

//////////////////////////////////////////////////////////////////////
// クラス UdGraph
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] node_num ノード数
// @param[in] edge_list 枝のリスト
UdGraph::UdGraph(SizeType node_num,
		 const vector<Edge>& edge_list) :
  mNodeNum{node_num},
  mEdgeList{edge_list}
{
  // mEdgeList の内容を正規化する．
  for ( auto& edge: mEdgeList ) {
    if ( edge.id1 > edge.id2 ) {
      std::swap(edge.id1, edge.id2);
    }
  }
}

// @brief 反射の時に true を返す．
//
// 反射とはすべてのノードに自己ループがあること
bool
UdGraph::is_reflective() const
{
  vector<bool> mark(node_num(), false);
  for ( const auto& edge: edge_list() ) {
    int id1 = edge.id1;
    int id2 = edge.id2;
    if ( id1 == id2 ) {
      mark[id1] = true;
    }
  }

  for ( auto id: Range(node_num()) ) {
    if ( !mark[id] ) {
      return false;
    }
  }
  return true;
}

BEGIN_NONAMESPACE

// 文字列を空白で区切る
void
split(const string& src_str,
      vector<string>& str_list)
{
  string tmp_str(src_str);
  for ( ; ; ) {
    string::size_type p = tmp_str.find_first_of(' ');
    string tmp = tmp_str.substr(0, p);
    str_list.push_back(tmp);
    if ( p == string::npos ) {
      break;
    }
    tmp_str = tmp_str.substr(p + 1, string::npos);
  }
}

void
syntax_error(int line)
{
  ostringstream err;
  err << "Line " << line << ": Syntax error";
  MsgMgr::put_msg(__FILE__, __LINE__,
		  MsgType::Error,
		  "DIMACS002",
		  err.str());
}

END_NONAMESPACE


// @brief DIMACS 形式のファイルを読み込む．
// @param[in] filename 入力のファイル名
// @return 読み込んだグラフを返す．
UdGraph
UdGraph::read_dimacs(const string& filename)
{
  ifstream s(filename);
  if ( !s ) {
    ostringstream err;
    err << filename << ": No such file";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    MsgType::Error,
		    "DIMACS005",
		    err.str());
    return UdGraph();
  }
  return read_dimacs(s);
}

// @brief DIMACS 形式のファイルを読み込む．
// @param[in] s 入力のストリーム
// @return 読み込んだグラフを返す．
UdGraph
UdGraph::read_dimacs(istream& s)
{
  bool first = true;
  int line = 1;
  string buff;
  int node_num = 0;
  int edge_num = 0;
  int max_node_id = 0;

  vector<Edge> edge_list;

  // ファイルをスキャンする．
  // - 'p' 行から node_num, edge_num を得る．
  // - 'e' 行の内容を edge_list に入れる．
  // - 'e' 行に現れるノード番号の最大値を max_node_id に入れる．
  while ( getline(s, buff) ) {
    if ( buff[0] == 'c' ) {
      // コメント行は読み飛ばす
      continue;
    }

    vector<string> str_list;
    split(buff, str_list);
    if ( str_list.empty() ) {
      syntax_error(line);
      goto error_exit;
    }

    if ( str_list[0] == "p" ) {
      if ( !first ) {
	ostringstream err;
	err << "Line " << line
	    << ": 'p' line is allowed only once";
	MsgMgr::put_msg(__FILE__, __LINE__,
			MsgType::Error,
			"DIMACS001",
			err.str());
	goto error_exit;
      }

      if ( str_list.size() != 4 || str_list[1] != "edge" ) {
	syntax_error(line);
	goto error_exit;
      }
      node_num = atoi(str_list[2].c_str());
      edge_num = atoi(str_list[3].c_str());
      edge_list.reserve(edge_num);
    }
    else if ( str_list[0] == "e" ) {
      if ( str_list.size() != 3 ) {
	syntax_error(line);
	goto error_exit;
      }
      int id1 = atoi(str_list[1].c_str()) - 1;
      int id2 = atoi(str_list[2].c_str()) - 1;
      if ( max_node_id < id1 ) {
	max_node_id = id1;
      }
      if ( max_node_id < id2 ) {
	max_node_id = id2;
      }
      edge_list.push_back({id1, id2});
    }
    else {
      syntax_error(line);
      goto error_exit;
    }
    ++ line;
  }

  ++ max_node_id;
  if ( node_num < max_node_id ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    MsgType::Warning,
		    "DIMACS003",
		    "# of nodes corrected");
    node_num = max_node_id;
  }
  if ( edge_num != edge_list.size() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    MsgType::Warning,
		    "DIMACS004",
		    "# of edges corrected");
    // 実は edge_num は使わない．
  }

  return UdGraph(node_num, edge_list);

 error_exit:
  return UdGraph();
}

// @brief 内容を DIMACS 形式で出力する．
// @param[in] filename ファイル名
void
UdGraph::write_dimacs(const string& filename) const
{
  ofstream s(filename);
  if ( !s ) {
    ostringstream err;
    err << filename << ": Could not create file";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    MsgType::Error,
		    "DIMACS006",
		    err.str());
    return;
  }
  write_dimacs(s);
}

// @brief 内容を DIMACS 形式で出力する．
// @param[in] s 出力のストリーム
void
UdGraph::write_dimacs(ostream& s) const
{
  s << "p edge " << node_num() << " " << edge_num() << endl;
  for ( const auto& edge: edge_list() ) {
    int id1 = edge.id1 + 1;
    int id2 = edge.id2 + 1;
    s << "e " << id1 << " " << id2 << endl;
  }
}

// @brief 独自形式のファイルを読み込む．
// @param[in] s 入力のストリーム
//
// dump() で出力した形式に対応している．
UdGraph
UdGraph::restore(istream& s)
{
  bool first = true;
  int line = 1;
  string buff;
  int node_num = 0;
  int edge_num = 0;
  int max_node_id = 0;

  vector<Edge> edge_list;

  // ファイルをスキャンする．
  // - 'pw' 行から node_num, edge_num を得る．
  // - 'ew' 行の内容を edge_list に入れる．
  // - 'ew' 行に現れるノード番号の最大値を max_node_id に入れる．
  while ( getline(s, buff) ) {
    if ( buff[0] == 'c' ) {
      // コメント行は読み飛ばす
      continue;
    }

    vector<string> str_list;
    split(buff, str_list);
    if ( str_list.empty() ) {
      syntax_error(line);
      goto error_exit;
    }

    if ( str_list[0] == "pw" ) {
      if ( !first ) {
	ostringstream err;
	err << "Line " << line
	    << ": 'pw' line is allowed only once";
	MsgMgr::put_msg(__FILE__, __LINE__,
			MsgType::Error,
			"DIMACS011",
			err.str());
	goto error_exit;
      }

      if ( str_list.size() != 4 || str_list[1] != "edge" ) {
	syntax_error(line);
	goto error_exit;
      }
      node_num = atoi(str_list[2].c_str());
      edge_num = atoi(str_list[3].c_str());
      edge_list.reserve(edge_num);
    }
    else if ( str_list[0] == "ew" ) {
      if ( str_list.size() != 3 ) {
	syntax_error(line);
	goto error_exit;
      }
      int id1 = atoi(str_list[1].c_str()) - 1;
      int id2 = atoi(str_list[2].c_str()) - 1;
      if ( max_node_id < id1 ) {
	max_node_id = id1;
      }
      if ( max_node_id < id2 ) {
	max_node_id = id2;
      }
      edge_list.push_back({id1, id2});
    }
    else {
      syntax_error(line);
      goto error_exit;
    }
    ++ line;
  }

  ++ max_node_id;
  if ( node_num < max_node_id ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    MsgType::Warning,
		    "DIMACS013",
		    "# of nodes corrected");
    node_num = max_node_id;
  }
  if ( edge_num != edge_list.size() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    MsgType::Warning,
		    "DIMACS014",
		    "# of edges corrected");
    // 実は edge_num は使わない．
  }

  return UdGraph(node_num, edge_list);

 error_exit:
  return UdGraph();
}

// @brief 独自形式のファイルを読み込む．
// @param[in] filename 入力ファイル名
//
// dump() で出力した形式に対応している．
UdGraph
UdGraph::restore(const string& filename)
{
  ifstream s(filename);
  if ( !s ) {
    ostringstream err;
    err << filename << ": No such file";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    MsgType::Error,
		    "DIMACS015",
		    err.str());
    return UdGraph();
  }
  return restore(s);
}

// @brief 独自形式でファイルに出力する．
// @param[in] s 出力のストリーム
void
UdGraph::dump(ostream& s) const
{
  s << "pw edge " << node_num() << " " << edge_num() << endl;
  for ( const auto& edge: edge_list() ) {
    int id1 = edge.id1 + 1;
    int id2 = edge.id2 + 1;
    int w = edge.weight;
    s << "ew " << id1 << " " << id2 << " " << w << endl;
  }
}

// @brief 独自形式でファイルに出力する．
// @param[in] filename 出力ファイル名
void
UdGraph::dump(const string& filename) const
{
  ofstream s(filename);
  if ( !s ) {
    ostringstream err;
    err << filename << ": Could not create file";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    MsgType::Error,
		    "DIMACS016",
		    err.str());
    return;
  }
  dump(s);
}

END_NAMESPACE_YM_UDGRAPH
