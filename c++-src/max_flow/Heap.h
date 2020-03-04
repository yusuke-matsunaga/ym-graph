#ifndef HEAP_H
#define HEAP_H

/// @file Heap.h
/// @brief Heap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2020 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


class Heap
{
public:

  Heap(int max_size);

  ~Heap();


public:

  void
  put(RgNode* node);

  RgNode*
  pop_top();

  int
  num() const;

  void
  dump(ostream& s) const;


private:

  void
  move_down(RgNode* cell);

  void
  move_up(Cell* cell);

  void
  locate(Cell* cell,
	     int index);

  void
  sanity_check();


private:

  int mNum;

  vector<Cell*> mMinHeap;

  vector<Cell*> mMaxHeap;

};

Heap::Heap(int max_heap) :
  mNum{0},
  mMinHeap(max_heap),
  mMaxHeap(max_heap)
{
}

Heap::~Heap()
{
}

void
Heap::put(Cell* cell)
{
  if ( debug_heap) {
    cout << "before put(" << cell->val << ")" << endl;
    dump(cout);
  }

  min_locate(cell, mNum);
  max_locate(cell, mNum);
  ++ mNum;

  min_move_up(cell);
  max_move_up(cell);

  if ( debug_heap ) {
    sanity_check();
  }
}

void
Heap::del(Cell* cell)
{
  if ( debug_heap ) {
    cout << "before del(" << cell->val << ")" << endl;
    dump(cout);
  }

  auto p_mincell = mMinHeap[mNum - 1];
  if ( cell->min_index < mNum - 1) {
    min_locate(p_mincell, cell->min_index);
    min_move_down(p_mincell);
    min_move_up(p_mincell);
  }
  mMinHeap[mNum] = nullptr;

  auto p_maxcell = mMaxHeap[mNum - 1];
  if ( cell->max_index < mNum - 1) {
    max_locate(p_maxcell, cell->max_index);
    max_move_down(p_maxcell);
    max_move_up(p_maxcell);
  }
  mMaxHeap[mNum] = nullptr;

  -- mNum;

  if ( debug_heap ) {
    sanity_check();
  }
}

int
Heap::min() const
{
  const auto cell = mMinHeap[0];
  return cell->val;
}

int
Heap::max() const
{
  const auto cell = mMaxHeap[0];
  return cell->val;
}

void
Heap::min_move_down(Cell* cell)
{
  int pos = cell->min_index;
  while ( true ) {
    int l_pos = pos * 2 + 1;
    int r_pos = pos * 2 + 2;
    if ( l_pos >= mNum ) {
      // 子供を持たない場合
      break;
    }
    auto cell = mMinHeap[pos];
    auto l_cell = mMinHeap[l_pos];
    if ( r_pos >= mNum ) {
      // 左の子供のみを持つ場合
      if ( l_cell->val < cell->val ) {
	min_locate(cell, l_pos);
	min_locate(l_cell, pos);
      }
      break;
    }
    else {
      // 両方の子供を持つ場合
      auto r_cell = mMinHeap[r_pos];
      if ( l_cell->val < r_cell->val ) {
	if ( l_cell->val < cell->val ) {
	  min_locate(cell, l_pos);
	  min_locate(l_cell, pos);
	  pos = l_pos;
	}
	else {
	  break;
	}
      }
      else {
	if ( r_cell->val < cell->val ) {
	  min_locate(cell, r_pos);
	  min_locate(r_cell, pos);
	  pos = r_pos;
	}
	else {
	  break;
	}
      }
    }
  }
}

void
Heap::min_move_up(Cell* cell)
{
  int pos = cell->min_index;
  while ( pos > 0 ) {
    int p_pos = (pos - 1) / 2;
    auto cell = mMinHeap[pos];
    auto p_cell = mMinHeap[p_pos];
    if ( cell->val < p_cell->val ) {
      min_locate(cell, p_pos);
      min_locate(p_cell, pos);
      pos = p_pos;
    }
    else {
      break;
    }
  }
}

void
Heap::min_locate(Cell* cell,
		 int index)
{
  mMinHeap[index] = cell;
  cell->min_index = index;
}

void
Heap::max_move_down(Cell* cell)
{
  int pos = cell->max_index;
  while ( true ) {
    int l_pos = pos * 2 + 1;
    int r_pos = pos * 2 + 2;
    if ( l_pos >= mNum ) {
      // 子供を持たない場合
      break;
    }
    auto cell = mMaxHeap[pos];
    auto l_cell = mMaxHeap[l_pos];
    if ( r_pos >= mNum ) {
      // 左の子供のみを持つ場合
      if ( l_cell->val > cell->val ) {
	max_locate(cell, l_pos);
	max_locate(l_cell, pos);
      }
      break;
    }
    else {
      // 両方の子供を持つ場合
      auto r_cell = mMaxHeap[r_pos];
      if ( l_cell->val > r_cell->val ) {
	if ( l_cell->val > cell->val ) {
	  max_locate(cell, l_pos);
	  max_locate(l_cell, pos);
	  pos = l_pos;
	}
	else {
	  break;
	}
      }
      else {
	if ( r_cell->val > cell->val ) {
	  max_locate(cell, r_pos);
	  max_locate(r_cell, pos);
	  pos = r_pos;
	}
	else {
	  break;
	}
      }
    }
  }
}

void
Heap::max_move_up(Cell* cell)
{
  int pos = cell->max_index;
  while ( pos > 0 ) {
    int p_pos = (pos - 1) / 2;
    auto cell = mMaxHeap[pos];
    auto p_cell = mMaxHeap[p_pos];
    if ( cell->val > p_cell->val ) {
      max_locate(cell, p_pos);
      max_locate(p_cell, pos);
      pos = p_pos;
    }
    else {
      break;
    }
  }
}

void
Heap::max_locate(Cell* cell,
		 int index)
{
  mMaxHeap[index] = cell;
  cell->max_index = index;
}

void
Heap::sanity_check()
{
  bool error = false;
  for ( int pos = 1; pos < mNum; ++ pos ) {
    int p_pos = (pos - 1) / 2;
    auto cell = mMinHeap[pos];
    auto p_cell = mMinHeap[p_pos];
    if ( cell->val < p_cell->val ) {
      cout << "ERROR in mMinHeap at " << pos << endl;
      cout << "cell->val = " << cell->val
	   << ", p_cell->val = " << p_cell->val << endl;
      error = true;
    }
  }
  for ( int pos = 1; pos < mNum; ++ pos ) {
    int p_pos = (pos - 1) / 2;
    auto cell = mMaxHeap[pos];
    auto p_cell = mMaxHeap[p_pos];
    if ( cell->val > p_cell->val ) {
      cout << "ERROR in mMaxHeap at " << pos << endl;
      cout << "cell->val = " << cell->val
	   << ", p_cell->val = " << p_cell->val << endl;
      error = true;
    }
  }
  if ( error ) {
    dump(cout);
    abort();
  }
}

void
Heap::dump(ostream& s) const
{
  dump_minheap(s);
  dump_maxheap(s);
  s << endl;
}

void
Heap::dump_minheap(ostream& s) const
{
  s << "mMinHeap" << endl;
  for ( int i = 0; i < mNum; ++ i ) {
    s << " " << mMinHeap[i]->val;
  }
  s << endl;
}

void
Heap::dump_maxheap(ostream& s) const
{
  s << "mMaxHeap" << endl;
  for ( int i = 0; i < mNum; ++ i ) {
    s << " " << mMaxHeap[i]->val;
  }
  s << endl;
}


#endif // HEAP_H
