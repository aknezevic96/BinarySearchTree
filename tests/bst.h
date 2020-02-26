#ifndef _BST_H
#define _BST_H

#include <iostream>
#include <vector>

template <typename T>
class bst {

  private: 
    typedef struct bst_node {
      T      val;
      bst_node *left;
      bst_node *right;
	  int numLeft, numRight, height;
	  
      bst_node ( const T & _val = T{}, bst_node * l = nullptr, bst_node *r = nullptr, int i = 0, int j = 0, int k = 1)
        : val { _val },  left { l }, right {r}, numLeft {i}, numRight {j}, height {k}
      { }
    } Node;
	
	int sizeN;
	T maxN, minN;
	
	
  public:
    // constructor:  initializes an empty tree
    bst(){
      root = nullptr;
	  sizeN = 0;
	  maxN = 0;
	  minN = 0;
    }

  private:
    // helper function which recursively deallocates nodes
    //   in a tree.
    static void delete_nodes(Node *r){
      if(r==nullptr) return;
      delete_nodes(r->left);
      delete_nodes(r->right);
      delete r;
    }

  public:
    // destructor
    ~bst() {
      delete_nodes(root);
    }

  private:
  
  //size balancing functions
  static int get_height(bst_node *r) 
	{	 
    	if (r == nullptr) 
			return 0; 
		
    	return r->height; 
	}
	
  static int get_max(int i, int j) //returns max of 2 ints
	{ 
    	if(i > j)
			return i;
		else
			return j;
	}
	
	static Node * rotate_left(Node *r) {
		Node *t = r->right; 
    	Node *s = t->left; 
  
   		t->left = r; 
    	r->right = s; 
  
    	r->height = get_max(get_height(r->left), get_height(r->right)) + 1; 
    	t->height = get_max(get_height(t->left), get_height(t->right)) + 1; 
  
    	return t; 
	}
	
	static Node * rotate_right(Node *r) { 
    	Node *t = r->left; 
    	Node *s = t->right; 
  
    	t->right = r; 
    	r->left = s; 
  	
   		r->height = get_max(get_height(r->left), get_height(r->right)) + 1; 
    	t->height = get_max(get_height(t->left), get_height(t->right)) + 1; 
  	
    	return t; 
	} 
	
	static int getBal(Node *r) //determines if a tree needs to be balanced
	{ 
  	  if (r == nullptr) 
  	  	   return 0; 
	   
  	  return get_height(r->left) - get_height(r->right);
	}
	
	//end size balancing functions

/**
 * function:  insert()
 * desc:      recursive helper function inserting x into
 *            binary search tree rooted  at r.
 *
 * returns:   pointer to root of tree after insertion.
 *
 * notes:     if x is already in tree, no modifications are made.
 */
    static Node * _insert(Node *r, T & x, bool &success){
      if(r == nullptr){
        success = true;
        return new Node(x, nullptr, nullptr);
      }

      if(r->val == x){
        success = false;
        return r;
      }
      if(x < r->val){
        r->left = _insert(r->left, x, success);
		r->numLeft++; //update num nodes
      }
      else {
        r->right = _insert(r->right, x, success);
		r->numRight++; //update num nodes
      }
	  
	  r->height = get_max(get_height(r->left), get_height(r->right)) + 1; //update height
	  
	  int bal = getBal(r);
	  
	  if (bal < -1 && x > r->right->val) //right-right
		return rotate_left(r); 
	
	  if (bal < -1 && x < r->right->val) { //right-left
        r->right = rotate_right(r->right); 
        return rotate_left(r); 
	  }
	  
	  if (bal > 1 && x < r->left->val) //left-left
        return rotate_right(r);
	
	  if (bal > 1 && x > r->left->val) { //left-right
        r->left =  rotate_left(r->left); 
        return rotate_right(r); 
	  }
	  
	  return r; //no balancing needed
    }


  public:
  /**
   * function:  insert
   * desc:      inserts x into BST given by t.  Note that
   *            a BST stores a SET -- no duplicates.  Thus,
   *            if x is already in t when call made, no 
   *            modifications to tree result.
   *
   * note:      helper function does most of the work.
   *
   */
   bool insert(T & x){
      bool success;
      root = _insert(root, x, success);
	  sizeN++;
	  if(x > maxN)
		  maxN = x;
	  else if(x < minN)
		  minN = x;
      return success;
   }

/**
 * function:  contains()
 * desc:      returns true or false depending on whether x is an
 *            element of BST (calling object)
 *
 */
    bool contains(const T & x){
      Node *p = root;

      while(p != nullptr){

        if(p->val == x)
          return true;
        if(x < p->val){
          p = p->left;
        }
        else
          p = p->right;
      }
      return false;  
    }

  private:
    // returns pointer to node containing
    //   smallest value in tree rooted at r
    static Node * _min_node(Node *r ){
      if(r==nullptr)
        return nullptr; // should never happen!
      while(r->left != nullptr)
        r = r->left;
      return r;
    }

    // returns pointer to node containing
    //   largest value in tree rooted at r
    static Node * _max_node(Node *r ){
      if(r==nullptr)
        return nullptr; // should never happen!
      while(r->right != nullptr)
        r = r->right;
      return r;
    }

    // recursive helper function for node removal
    //   returns root of resulting tree after removal.
    static Node * _remove(Node *r, T & x, bool &success){
      Node *tmp;
      bool sanity;

      if(r==nullptr){
        success = false;
        return nullptr;
      }
      if(r->val == x){
        success = true;

        if(r->left == nullptr){
          tmp = r->right;
          delete r;
          return tmp;
        }
        if(r->right == nullptr){
          tmp = r->left;
          delete r;
          return tmp;
        }
        // if we get here, r has two children
        r->val = _min_node(r->right)->val;
        r->right = _remove(r->right, r->val, sanity);
		r->numRight--;
        if(!sanity)
          std::cerr << "ERROR:  remove() failed to delete promoted value?\n";
        return r;
      }
      if(x < r->val){
        r->left = _remove(r->left, x, success);
		r->numLeft--;
      }
      else {
        r->right = _remove(r->right, x, success);
		r->numRight--;
      }
      return r;

    }

  public:

    bool remove(T & x){
      bool success;
      root = _remove(root, x, success);
	  if(success) {
		  sizeN--;
		  if(x == maxN)
			  maxN = _max_node(root)->val;
		  else if(x == minN)
			  minN = _min_node(root)->val;
	  }
	  
      return success;
    }
	
	
	
    // recursive helper function to compute size of
    //   tree rooted at r
    static int _size(Node *r){
      if(r==nullptr) return 0;
      return _size(r->left) + _size(r->right) + 1;
    }

  public: 
    int size() {
      return _size(root);
    }

  private:

    static int _height(Node *r){
      int l_h, r_h;

      if(r==nullptr) return -1;
      l_h = _height(r->left);
      r_h = _height(r->right);
      return 1 + (l_h > r_h ? l_h : r_h);
    }

  public:

    int height() {
      return _height(root);
    }

    bool min(T & answer) {
      if(root == nullptr){
        return false;
      }
      answer = _min_node(root)->val;
      return true;
    }

    T max() {
      return _max_node(root)->val;
    }

    /******************************************
     *
     * "stubs" for assigned TODO functions below 
     *
     *****************************************/

    // TODO
private: 

	void fillVector(Node *r, std::vector<T> * v) {
		if(r == nullptr)
			return;
		
		fillVector(r->left, v);
		
		v->push_back(r->val);
		
		fillVector(r->right, v);
	}
public:

    std::vector<T> * to_vector() {
		std::vector<T> * vr = new std::vector<T>;
		fillVector(root, vr);
		
		return vr;
    }


    /* TODO
     * Function:  get_ith
     * Description:  determines the ith smallest element in t and
     *    "passes it back" to the caller via the reference parameter x.  
     *    i ranges from 1..n where n is the number of elements in the 
     *    tree.
     *
     *    If i is outside this range, false is returned.
     *    Otherwise, true is returned (indicating "success").
     *
     * Runtime:  O(h) where h is the tree height
     */
private:
	T get_smallest(Node *r, int i) {
		if(r == nullptr)
			return -1;
		
		if(r->numLeft + 1 == i)
			return r->val;
		else if(i <= r->numLeft)
			return get_smallest(r->left, i);
		else
			return get_smallest(r->right, i - 1 -r->numLeft);
	}
public:

    bool get_ith(int i, T &x) {
		if(i < 1 || i > sizeN || root == nullptr) {
			std::cerr << "i must range from 1 to the max number of nodes in the tree!\n";
			return false;
		}
		
		x = get_smallest(root, i);
		return true;
    }


    bool get_ith_SLOW(int i, T &x) {
      int n = size();
      int sofar=0;

      if(i < 1 || i > n) 
        return false;

      _get_ith_SLOW(root, i, x, sofar);
      return true;
    }


    /* TODO
     * Function:  num_geq
     * Description:  returns the number of elements in tree which are 
     *       greater than or equal to x.
     *
     * Runtime:  O(h) where h is the tree height
     */
private:
	void n_geq(Node *r, T x, int &count) {
		if(r == nullptr)
			return;
		if(r->val == x) {
			count++;
			return n_geq(r->right, x, count);
		}
		else if(r->val > x) {
			count += r->numRight + 1;
			return n_geq(r->left, x, count);
		}
		
		return n_geq(r->right, x, count);
	}
public:

    int num_geq(const T & x) {
		int count = 0;
		if(x > maxN)
			return 0;
		else if(x <= minN)
			return sizeN;
		
		n_geq(root, x, count);
		return count;
    }

    /*
     * function:     num_geq_SLOW
     * description:  same functionality as num_geq but sloooow (linear time)
     *
     */
    int num_geq_SLOW(const T & x) {
      return _num_geq_SLOW(root, x);
    }
    

    /* TODO
     * Function:  num_leq
     * Description:  returns the number of elements in tree which are less
     *      than or equal to x.
     *
     * Runtime:  O(h) where h is the tree height
     *
     **/
private:
	void n_leq(Node *r, T x, int &count) {
		if(r == nullptr)
			return;
		if(r->val == x) {
			count++;
			return n_leq(r->left, x, count);
		}
		else if(r->val < x) {
			count += r->numLeft + 1;
			return n_leq(r->right, x, count);
		}
		
		return n_leq(r->left, x, count);
	}
public: 
    int num_leq(const T &x) {
		int count = 0;
		if(x < minN)
			return 0;
		else if(x >= maxN)
			return sizeN;
		
		n_leq(root, x, count);
		return count;
    }

    /*
     * function:     num_leq_SLOW
     * description:  same functionality as num_leq but sloooow (linear time)
     *
     */
    int num_leq_SLOW(const T & x) {
      return _num_leq_SLOW(root, x);
    }

    /* TODO
     * Function:  num_range
     * Description:  returns the number of elements in tree which are
     *       between min and max (inclusive).
     *
     * Runtime:  O(h) where h is the tree height
     *
     **/
    int num_range(const T & min, const T & max) {
		int count = 0;
		if(min < minN || max > maxN)
			return 0;
		
		return num_geq(min) - num_geq(max);
    }

    /*
     * function:     num_range_SLOW
     * description:  same functionality as num_range but sloooow (linear time)
     *
     */
    int num_range_SLOW(const T & min, const T & max) {
      return _num_range_SLOW(root, min, max);
    }

  private:


    static void _get_ith_SLOW(Node *t, int i, T &x, int &sofar) {
      if(t==nullptr) 
        return;
      _get_ith_SLOW(t->left, i, x, sofar);

      if(sofar==i) 
        return;
      sofar++;
      if(sofar==i) {
        x = t->val;
        return;
      }
      _get_ith_SLOW(t->right, i, x, sofar);
    }

    static int _num_geq_SLOW(Node * t, const T & x) {
      int total;

      if(t==nullptr) return 0;
      total =_num_geq_SLOW(t->left, x) + _num_geq_SLOW(t->right, x);

      if(t->val >= x)
        total++;
      return total;
    }

    static int _num_leq_SLOW(Node *t, const T &x) {
      int total;

      if(t==nullptr) return 0;
      total =_num_leq_SLOW(t->left, x) + _num_leq_SLOW(t->right, x);

      if(t->val <= x)
        total++;
      return total;
    }

    static int _num_range_SLOW(Node *t, const T &min, const T &max) {
      int total;

      if(t==nullptr) return 0;
      total =_num_range_SLOW(t->left, min, max) + 
                _num_range_SLOW(t->right, min, max);

      if(t->val >= min && t->val <= max)
        total++;
      return total;
    }

	
  private:
    static void indent(int m){
      int i;
      for(i=0; i<m; i++)
        std::cout << "-";
    }

    static void _inorder(Node *r){
      if(r==nullptr) return;
      _inorder(r->left);
      std::cout << "[ " << r->val << " ]\n";
      _inorder(r->right);
    }

    static void _preorder(Node *r, int margin){
      if(r==nullptr) {
        indent(margin);
        std::cout << " nullptr \n";
      } 
      else {
        indent(margin);
        std::cout << "[ " << r->val << " ]\n";
        _preorder(r->left, margin+3);
        _preorder(r->right, margin+3);
      }
    }

    /* 
     * TODO:
     * Complete the (recursive) helper function for the post-order traversal.
     * Remember: the indentation needs to be proportional to the height of the node!
     */
    static void _postorder(Node *r, int margin){

      std::cout << "\n   WARNING:  POSTORDER UNIMPLEMENTED...\n";
    }

  public:
    void inorder() {
      std::cout << "\n======== BEGIN INORDER ============\n";
      _inorder(root);
      std::cout << "\n========  END INORDER  ============\n";
    }


    void preorder() {

      std::cout << "\n======== BEGIN PREORDER ============\n";
      _preorder(root, 0);
      std::cout << "\n========  END PREORDER  ============\n";

    }

    // indentation is proportional to depth of node being printed
    //   depth is #hops from root.
    void postorder() {

      std::cout << "\n======== BEGIN POSTORDER ============\n";
      _postorder(root, 0);
      std::cout << "\n========  END POSTORDER  ============\n";

    }
	
	private:
    /* 
     * Recursive  helper function _from_vec, used by
     * bst_from_sorted_arr(...). The function must return a sub-tree that is
     * perfectly balanced, given a sorted array of elements a.
     */
    static Node * _from_vec(const std::vector<T> &a, int low, int hi){
      int m;
      Node *root;

      if(hi < low) return nullptr;
      m = (low+hi)/2;
      root = new Node(a->at(m));
      root->left  = _from_vec(a, low, m-1);
      root->right = _from_vec(a, m+1, hi);
      return root;

    }

public:
    static bst * from_sorted_vec(std::vector<T> &a, int n){

      bst * t = new bst();
      t->root = _from_vec(a, 0, n-1);
      return t;
    }
	

    // TODO:  num_leaves
    //   Hint:  feel free to write a helper function!!
	void countL(Node *r, int &l) {
		if(r == nullptr)
			return;
		if(r->left == nullptr && r->right == nullptr)
			l++;
		
		countL(r->left, l);
		countL(r->right, l);
	}
	
    int num_leaves() {
		int numL = 0;
		countL(root, numL);
		return numL;
    }

    // TODO:  num_at_level
    // description:  returns the number of nodes at specified level
    //   in tree.
    // note:  the root is at level 0.
    // Hint:  recursive helper function?
    int num_at_level(int level) {

      std::cout << "\n     WARNING:  bst::num_leaves UNIMPLEMENTED...\n";
      return 0;
    }
	
private:
	Node * root;


}; // end class bst

#endif
