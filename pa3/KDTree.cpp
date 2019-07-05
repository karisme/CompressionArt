
/**
 *
 * KDTree (pa3)
 * slight modification of a Kd tree of dimension 2.
 * KDTree.cpp
 * This file will be used for grading.
 *
 */

#include "KDTree.h"

KDTree::Node::Node(pair<int,int> ul, pair<int,int> lr, HSLAPixel a)
	:upLeft(ul),lowRight(lr),avg(a),left(NULL),right(NULL)
	{}

KDTree::~KDTree(){
	clear();
}

KDTree::KDTree(const KDTree & other) {
	copy(other);
}


KDTree & KDTree::operator=(const KDTree & rhs){
	if (this != &rhs) {
		clear();
		copy(rhs);
	}
	return *this;
}

KDTree::KDTree(PNG & imIn){ 
	stats s(imIn);
	
	height = imIn.height();
	width = imIn.width();
	
	

	pair<int,int> ul(0,0);
	pair<int,int> lr(width - 1, height - 1);  // because  

	
	root = buildTree(s,ul,lr);

}

KDTree::Node * KDTree::buildTree(stats & s, pair<int,int> ul, pair<int,int> lr) {
	buildRecursiveTree(s,ul,lr,root);
	// cout<< root->left->upLeft.first << " " << root->left->upLeft.second <<endl;
	// cout<< root->left->lowRight.first << " " << root->left->lowRight.second <<endl;
	// cout << endl;
	// cout<< root->right->upLeft.first << " " << root->right->upLeft.second <<endl;
	// cout<< root->right->lowRight.first << " " << root->right->lowRight.second <<endl;
	return root;
}

void KDTree::buildRecursiveTree(stats & s,pair<int,int> ul, pair<int,int> lr, Node * & curr){
	
	curr = new Node(ul,lr,s.getAvg(ul,lr));                  // THIS LINE IS WHAT IS ACTUALLY CREATING THE NODES OF TEH TREE WITH THE RIGHT DATA
	//cout << "fam WE MADE IT" << endl;
	
	if (s.rectArea(ul,lr) == 1){   // THE BASE CASE IF THERE IS ONLY ONE PIXEL /// UPPER LEFT = LOWER RIGHT
		curr->avg = s.getAvg(ul,lr);;
		curr->upLeft = ul;
		curr->lowRight = lr;
		return;
	}

	
	
	vector<pair<int,int>> splitResult = findSplit(ul,lr,s);     // this function will return the optimal place to make a split
	                                                            // assume it works properly and keep moving

	
	 
	                                                   
	pair<int,int> rectOneUpperLeft = ul; 
	pair<int,int> rectOneLowerRight = splitResult[1];

	pair<int,int> rectTwoUpperLeft = splitResult[0];
	pair<int,int> rectTwoLowerRight = lr;

	buildRecursiveTree(s,rectOneUpperLeft,rectOneLowerRight,curr->left);             // PASSING LEFT AND RIGHT NODE CREATE THE LINK BETWEEN
	buildRecursiveTree(s,rectTwoUpperLeft,rectTwoLowerRight,curr->right);            // THE NODES AS WE RECURSIVELY DIVIDE THE IMG INTO RECTANGLES
	
}





vector<pair<int,int>> KDTree::findSplit(pair<int,int> ul, pair<int,int> lr, stats & s){
	vector<pair<int,int>> splitLine; // vector of size three that contains both points of the split we want to make and a 
	splitLine.resize(2);             // a 0 in splitLine[2] means we have a horizontal line and a 1 means we have a vertical line

	double minEntropy = INFINITY;
	double picSize = s.rectArea(ul,lr);

	pair<int,int> minEntropyPosFirst(ul.first,ul.second);
	pair<int,int> minEntropyPosSecond(lr.first,lr.second);

	pair<int,int> verticalLine(0,0);
	pair<int,int> horizontalLine(1,1);
	


	// VERTICAL SPLIT CHECK
	for(int i = ul.first; i < lr.first; i++){
		
		pair<int,int> newLowerRight(i,lr.second);
		pair<int,int> newUpperLeft(i + 1,ul.second);

		double currEntropyRectLeft = s.entropy(ul,newLowerRight) * (s.rectArea(ul,newLowerRight) / picSize);     // takes entropy of rectangle to the left of the line
		double currEntropyRectRight = s.entropy(newUpperLeft,lr) * (s.rectArea(newUpperLeft,lr) / picSize);      // takes entropy of rectangle to the right of the line
		double currEntropy = currEntropyRectLeft + currEntropyRectRight;   // adds them (is this correct? LMAO)
		
		bool makeSwitch = currEntropy < minEntropy;
		
		if (makeSwitch){
			minEntropy = currEntropy;
			minEntropyPosFirst.first = i + 1;
			minEntropyPosFirst.second = ul.second;

			minEntropyPosSecond.first = i;
			minEntropyPosSecond.second = lr.second;
			//splitLine[2] = verticalLine;
		}
	}

	// HORIZONTAL SPLIT CHECK

	for (int j = ul.second; j < lr.second; j++){
		
		pair<int,int> newLowerRight(lr.first,j);
		pair<int,int> newUpperLeft(ul.first,j + 1);
		double currEntropyRectTop = s.entropy(ul,newLowerRight) * (s.rectArea(ul,newLowerRight) / picSize);
		double currEntropyRectBottom = s.entropy(newUpperLeft,lr) * (s.rectArea(newUpperLeft,lr) / picSize);
		double currEntropy = currEntropyRectTop + currEntropyRectBottom;
		
		bool makeSwitch = currEntropy < minEntropy;
		
		if (makeSwitch){
			minEntropy = currEntropy;
			minEntropyPosFirst.first = ul.first;
			minEntropyPosFirst.second = j + 1;
			

			minEntropyPosSecond.first = lr.first;
			minEntropyPosSecond.second = j;
			//splitLine[2] = horizontalLine;
		}
	}

	

	   // ASSUMING FOR LOOPS HAVE DONE THEIR JOB, minEntropyPos have two xy coordinates that hold the line of the split. 

	   splitLine[0] = minEntropyPosFirst;
	   splitLine[1] = minEntropyPosSecond;
	   return splitLine;

	
}



PNG KDTree::render(){
	PNG treePic(width,height);
	renderTraversal(root,treePic);

	return treePic;

}

bool KDTree::isLeaf(Node * & curr){ 
	return curr->left == nullptr & curr->right == NULL;
}

void KDTree::renderTraversal(Node * & curr, PNG & pic){


	if (curr == NULL) return;
	
	if (curr->left == NULL && curr->right == NULL){
		pair<int,int> start = curr->upLeft;
	    pair<int,int> end = curr->lowRight;

		for (int i = start.first; i <= end.first; i++){
			for (int j = start.second; j <= end.second; j++){
				//cout << "HEEEE" <<endl;
				HSLAPixel * temp = pic.getPixel(i,j);
				*temp = curr->avg;
			}
		}
		return;
	}

	
	renderTraversal(curr->left, pic);
	renderTraversal(curr->right,pic);
	

}

void KDTree::prune(double pct, double tol){
	pruneTraversal(root,pct,tol);
	
}

// THIS FUNCTION WILL TRAVERSE THE TREE AND PRUNNING ACCORDINGLY
// USING THE RESULT from leafCounterAll TO DECIDE
// IF IT PRUNES, IT WILL STOP ITERATING THROUGH THE TREE,
// OR IT WILL KEEP GOING TO THE nExT LEVEL

void KDTree::pruneTraversal(Node * & curr, double pct, double tol){
	if (curr == NULL){
		return;
	}


	
	int counterLeaf = 0;
	int counterLeafCond = 0;

	leafCounterAll(curr, tol, counterLeaf, counterLeafCond, curr->avg);


	  if ((double)counterLeafCond  / (double)counterLeaf >= pct){        // CANT USE INT DIVISION, HAVE TO CAST AS A DOUBLE
		deleteTraversal(curr->left);
		deleteTraversal(curr->right);
	} else {

      pruneTraversal(curr->left,pct,tol);
	  pruneTraversal(curr->right,pct,tol);
	}

}


void KDTree::leafCounterAll(Node * & curr, double tol, int & rsfLeaves, int & rsfLeavesCond, HSLAPixel rootAvg){
	if (curr == NULL){
		return;
	}

	if (curr->left == NULL && curr->right == NULL){
		rsfLeaves++;
		if (rootAvg.dist(curr->avg) <= tol) rsfLeavesCond++;
	}

	leafCounterAll(curr->left, tol, rsfLeaves,rsfLeavesCond, rootAvg);
	leafCounterAll(curr->right, tol, rsfLeaves,rsfLeavesCond, rootAvg);
}



void KDTree::clear() {
	
	deleteTraversal(root);
	height = 0;
	width = 0;
}

void KDTree::deleteTraversal(Node * & curr){
	if (curr == NULL) {
		return;
	} else {

	deleteTraversal(curr->left);
	deleteTraversal(curr->right);

	delete curr;
	curr = NULL;
	}
}

void KDTree::copyTraversal(Node * & curr, Node * treeLoc){      // HAD BUNCH OF ERRORS, BUT PASSING BY REFERENCE SEEMS TO SOLVE MOST OF THEM
	if (treeLoc == NULL) return;

	Node * temp = new Node(treeLoc->upLeft, treeLoc->lowRight, treeLoc->avg);
	curr = temp;
	//cout << "DOING SOMETHING" << endl;

	copyTraversal(curr->left, treeLoc->left);
	copyTraversal(curr->right, treeLoc->right);
}


void KDTree::copy(const KDTree & orig){
	width = orig.width;
	height = orig.height;
	
	copyTraversal(root,orig.root);
}

