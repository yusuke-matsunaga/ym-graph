
/// @file BiGraph.cc
/// @brief BiGraph の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020 Yusuke Matsunaga
/// All rights reserved.


#include "ym/BiGraph.h"
#include "ym/MsgMgr.h"


BEGIN_NAMESPACE_YM_BIGRAPH

//////////////////////////////////////////////////////////////////////
// クラス BiGraph
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] node1_num 頂点集合1の要素数
// @param[in] node2_num 頂点集合2の要素数
// @param[in] edge_list 枝のリスト
BiGraph::BiGraph(int node1_num,
		 int node2_num,
		 const vector<Edge>& edge_list) :
  mNode1Num{node1_num},
  mNode2Num{node2_num},
  mEdgeList{edge_list}
{
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

// @brief ストリームから読み込む．
// @param[in] s 入力ストリーム
// @return 結果のグラフを返す．
BiGraph
BiGraph::read(istream& s)
{
  bool first = true;
  int line = 1;
  string buff;
  int node1_num = 0;
  int node2_num = 0;
  int edge_num = 0;
  int max_node1_id = 0;
  int max_node2_id = 0;

  vector<Edge> edge_list;

  // ファイルをスキャンする．
  // - 'b' 行から node1_num, node2_num, edge_num を得る．
  // - 'e' 行の内容を edge_list に入れる．
  // - 'e' 行に現れるノード番号の最大値を max_node1_id, max_node2_id に入れる．
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

    if ( str_list[0] == "b" ) {
      if ( !first ) {
	ostringstream err;
	err << "Line " << line
	    << ": 'b' line is allowed only once";
	MsgMgr::put_msg(__FILE__, __LINE__,
			MsgType::Error,
			"BIGRAPH001",
			err.str());
	goto error_exit;
      }

      if ( str_list.size() != 4 ) {
	syntax_error(line);
	goto error_exit;
      }
      node1_num = atoi(str_list[1].c_str());
      node2_num = atoi(str_list[2].c_str());
      edge_num = atoi(str_list[3].c_str());
      edge_list.reserve(edge_num);
    }
    else if ( str_list[0] == "e" ) {
      if ( str_list.size() != 4 ) {
	syntax_error(line);
	goto error_exit;
      }
      int id1 = atoi(str_list[1].c_str()) - 1;
      int id2 = atoi(str_list[2].c_str()) - 1;
      int w = atoi(str_list[3].c_str());
      if ( max_node1_id < id1 ) {
	max_node1_id = id1;
      }
      if ( max_node2_id < id2 ) {
	max_node2_id = id2;
      }
      edge_list.push_back({id1, id2, w});
    }
    else {
      syntax_error(line);
      goto error_exit;
    }
    ++ line;
  }

  ++ max_node1_id;
  if ( node1_num < max_node1_id ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    MsgType::Warning,
		    "BIGRAPH003",
		    "# of node1s corrected");
    node1_num = max_node1_id;
  }

  ++ max_node2_id;
  if ( node2_num < max_node2_id ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    MsgType::Warning,
		    "BIGRAPH004",
		    "# of node2s corrected");
    node2_num = max_node2_id;
  }

  if ( edge_num != edge_list.size() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    MsgType::Warning,
		    "DIMACS005",
		    "# of edges corrected");
    // 実は edge_num は使わない．
  }

  return BiGraph(node1_num, node2_num, edge_list);

 error_exit:
  return BiGraph();
}

// @brief ファイルから読み込む．
// @param[in] filename ファイル名
// @return 結果のグラフを返す．
BiGraph
BiGraph::read(const string& filename)
{
  ifstream s(filename);
  if ( !s ) {
    ostringstream err;
    err << filename << ": No such file";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    MsgType::Error,
		    "BIGRAPH006",
		    err.str());
    return BiGraph();
  }
  return read(s);
}

// @brief ストリームに書き出す．
// @param[in] s 出力ストリーム
void
BiGraph::write(ostream& s) const
{
  s << "b " << node1_num() << " " << node2_num() << " " << edge_num() << endl;
  for ( const auto& edge: edge_list() ) {
    int id1 = edge.id1 + 1;
    int id2 = edge.id2 + 1;
    int w = edge.weight;
    s << "e " << id1 << " " << id2 << " " << w << endl;
  }
}

// @brief ファイルに書き出す．
// @param[in] filename ファイル名
void
BiGraph::write(const string& filename)
{
  ofstream s(filename);
  if ( !s ) {
    ostringstream err;
    err << filename << ": Could not create file";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    MsgType::Error,
		    "BIGRAPH007",
		    err.str());
    return;
  }
  write(s);
}

END_NAMESPACE_YM_BIGRAPH
