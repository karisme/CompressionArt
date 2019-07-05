
/**
 *
 * KDTree (pa3)
 * slight modification of a Kd tree of dimension 2.
 *
 */

#ifndef _TWODTREE_H_
#define _TWODTREE_H_

#include <utility>
#include "cs221util/PNG.h"
#include "cs221util/HSLAPixel.h"
#include "stats.h"
using namespace std;
using namespace cs221util;

/**
 * KDTree: This is a structure used in decomposing an image
 * into rectangles of similarly colored pixels.
 *
 * You should not remove anything from this class definition, but
 * you will find it helpful to add your own private helper functions to it.
 *
 * This file will be used for grading.
 */

class KDTree {
private:

   /**
    * The Node class is private to the tree class via the principle of
    * encapsulation---the end user does not need to know our node-based
    * implementation details.
    * given for PA3
    */
   class Node {
   public:
      Node(pair<int,int> ul, pair<int,int> lr, HSLAPixel a); // Node constructor

      pair<int,int> upLeft; 
      pair<int,int> lowRight;
      HSLAPixel avg;
      Node * left; // ptr to left subtree
      Node * right; // ptr to right subtree
      
   };
	
   
public:

   /* =============== start of given functions ====================*/

   /**
    * KDTree destructor.
    * Destroys all of the memory associated with the
    * current KDTree. This function should ensure that
    * memory does not leak on destruction of a KDTree.
    * 
    * @see KDTree.cpp
    */
   ~KDTree();

   /**
    * Copy constructor for a KDTree. GIVEN
    * Since KDTree allocate dynamic memory (i.e., they use "new", we
    * must define the Big Three). This depends on your implementation
    * of the copy funtion.
    * @see KDTree.cpp
    *
    * @param other The KDTree  we are copying.
    */
   KDTree(const KDTree & other);

   /**
    * Overloaded assignment operator for KDTree. 
    * Part of the Big Three that we must define because the class
    * allocates dynamic memory. This depends on your implementation
    * of the copy and clear funtions.
    *
    * @param rhs The right hand side of the assignment statement.
    */
   KDTree & operator=(const KDTree & rhs);


   /* =============== end of given functions ====================*/

   /* =============== public PA3 FUNCTIONS =========================*/

   /**
    * Constructor that builds a KDTree out of the given PNG.
    * Every leaf in the tree corresponds to a pixel in the PNG.
    * Every non-leaf node corresponds to a rectangle of pixels 
    * in the original PNG, represented by an (x,y) pair for the 
    * upper left corner of the rectangle and an (x,y) pair for 
    * lower right corner of the rectangle. In addition, the Node
    * stores a pixel representing the average color over the 
    * rectangle. 
    *
    * Every node's left and right children correspond to a partition
    * of the node's rectangle into two smaller rectangles. The node's
    * rectangle is split by the horizontal or vertical line that 
    * results in the two smaller rectangles whose sum of squared 
    * differences from their mean is as small as possible.
    *
    * The left child of the node will contain the upper left corner
    * of the node's rectangle, and the right child will contain the
    * lower right corner.
    *
   * This function will build the stats object used to score the 
   * splitting lines. It will also call helper function buildTree.
    */
   KDTree(PNG & imIn);

   /**
    * Render returns a PNG image consisting of the pixels
    * stored in the tree. may be used on pruned trees. Draws
    * every leaf node's rectangle onto a PNG canvas using the 
    * average color stored in the node.
    */
   PNG render();

   /*
    *  Prune function trims subtrees as high as possible in the tree.
    *  A subtree is pruned (cleared) if at least pct of its leaves are within 
    *  tol of the average color stored in the root of the subtree. 
    *  Pruning criteria should be evaluated on the original tree, not 
    *  on a pruned subtree. (we only expect that trees would be pruned once.)
    *  
   * You may want a recursive helper function for this one.
    */
   void prune(double pct, double tol);

   /* =============== end of public PA3 FUNCTIONS =========================*/

private:
   /*
    * Private member variables.
    *
    * You must use these as specified in the spec and may not rename them.
    * You may add more if you need them.
    */

   Node* root; // ptr to the root of the KDTree

   int height; // height of PNG represented by the tree
   int width; // width of PNG represented by the tree

   /* =================== private PA3 functions ============== */

   /**
    * Destroys all dynamically allocated memory associated with the
    * current KDTree class. Complete for PA3.
   * You may want a recursive helper functi
   * on for this one.
    */
   void clear();

   void pruneTraversal(Node * & curr, double pct, double tol);

   void deleteTraversal(Node * & curr);
   
   void copyTraversal(Node * & curr, Node * treeLoc);

   void renderTraversal(Node * & curr, PNG & pic);

   //pair<int,int> prunePermission(Node * & curr, double tol, pair<int,int> & rsf, HSLAPixel rootAvg);

   void leafCounterAll(Node * & curr, double tol, int & rsfLeaves, int & rsfLeavesCond, HSLAPixel rootAvg);

   //int leafCounterCond(Node * & curr, double tol, int & rsf, HSLAPixel rootAvg);

   bool isLeaf(Node * & curr);

   vector<pair<int,int>> findSplit(pair<int,int> ul, pair<int,int> lr, stats & s);

   /**
   * Copies the parameter other KDTree into the current KDTree.
   * Does not free any memory. Called by copy constructor and op=.
   * You may want a recursive helper function for this one.
   * @param other The KDTree to be copied.
   */
   void copy(const KDTree & other);

   /**
   * Private helper function for the constructor. Recursively builds
   * the tree according to the specification of the constructor.
   * @param s Contains the data used to split the rectangles
   * @param ul upper left point of current node's rectangle.
   * @param lr lower right point of current node's rectangle.
   */
   Node * buildTree(stats & s,pair<int,int> ul, pair<int,int> lr);

   void buildRecursiveTree(stats & s,pair<int,int> ul, pair<int,int> lr, Node * & curr);

   /* =================== end of private PA3 functions ============== */
};

#endif


