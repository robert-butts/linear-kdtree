/// this file exists to remove C++11 from CUDA, to support outdated nvcc compilers
#include "lkt.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>    
#include <assert.h>
#include <math.h>
//#include "mergesort.hh"
#include "tbb/tbb.h"

// x value ALONE is used for comparison, to create an xpack
bool operator<(const lqt_unified_node& rhs, const lqt_unified_node& lhs) {
  return rhs.location < lhs.location;
}


struct linear_quadtree_unified tbb_sortify_unified(struct linear_quadtree_unified lqt, const size_t threads) {
//  auto lowxpack = [](const struct rtree_point& rhs, const struct rtree_point& lhs) {
//    return rhs.x < rhs.y;
//  };
  tbb::task_scheduler_init init(threads);
  tbb::parallel_sort(lqt.nodes, lqt.nodes + lqt.length);
  return lqt;
}

/// does not block for GPU memory. Will fail, if GPU memory is insufficient.
struct linear_quadtree_unified lqt_create_heterogeneous(lqt_point* points, size_t len, 
                                                       ord_t xstart, ord_t xend, 
                                                       ord_t ystart, ord_t yend,
                                                       size_t* depth, const size_t threads) {
  return tbb_sortify_unified(lqt_nodify_cuda_unified(points, len, xstart, xend, ystart, yend, depth), threads);
}


/*
/// \param threads the number of threads to use when sorting. ONLY used in the 'sort' part of the algorithm
struct rtree cuda_create_rtree_heterogeneously_mergesort(struct rtree_point* points, const size_t len, const size_t threads) {
  struct rtree_leaf* leaves = cuda_create_leaves_together(parallel_mergesort(points, points + len, threads), len);
  const size_t leaves_len = DIV_CEIL(len, RTREE_NODE_SIZE);

  rtree_node* previous_level = (rtree_node*) leaves;
  size_t      previous_len = leaves_len;
  size_t      depth = 1; // leaf level is 0
  while(previous_len > RTREE_NODE_SIZE) {
    previous_level = cuda_create_level(previous_level, previous_len);
    previous_len = DIV_CEIL(previous_len, RTREE_NODE_SIZE);
    ++depth;
  }

  rtree_node* root = (rtree_node*) malloc(sizeof(rtree_node));
  init_boundary(&root->bounding_box);
  root->num = previous_len;
  root->children = previous_level;
  for(size_t i = 0, end = previous_len; i != end; ++i)
    update_boundary(&root->bounding_box, &root->children[i].bounding_box);
  ++depth;

  struct rtree tree = {depth, root};
  return tree;
}
*/
/*
/// SISD sort via single CPU core (for benchmarks)
struct rtree cuda_create_rtree_sisd(struct rtree_point* points, const size_t len) {
  std::sort(points, points + len);
  struct rtree_leaf* leaves = cuda_create_leaves_together(points, len);
  const size_t leaves_len = DIV_CEIL(len, RTREE_NODE_SIZE);

  rtree_node* previous_level = (rtree_node*) leaves;
  size_t      previous_len = leaves_len;
  size_t      depth = 1; // leaf level is 0
  while(previous_len > RTREE_NODE_SIZE) {
    previous_level = cuda_create_level(previous_level, previous_len);
    previous_len = DIV_CEIL(previous_len, RTREE_NODE_SIZE);
    ++depth;
  }

  rtree_node* root = (rtree_node*) malloc(sizeof(rtree_node));
  init_boundary(&root->bounding_box);
  root->num = previous_len;
  root->children = previous_level;
  for(size_t i = 0, end = previous_len; i != end; ++i)
    update_boundary(&root->bounding_box, &root->children[i].bounding_box);
  ++depth;

  struct rtree tree = {depth, root};
  return tree;
}
*/