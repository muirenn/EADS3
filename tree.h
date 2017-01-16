#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED


#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <exception>
#include <stdexcept>
#include <sstream>


using namespace std;
/*
template<typename Key>
class element_not_found : public exception {
	Key key;
	static ostringstream cnvt;

public:
	element_not_found(const Key& _key) : exception("Not found the given key"), key(_key) {}

	virtual const char* what() const throw() {
		cnvt.str("");
		cnvt << exception::what() << ": " << getKey();
		return cnvt.str().c_str();

	}

	const Key& getKey() const {
		return key;
	}

};

template<typename Key>
ostringstream element_not_found<typename Key>::cnvt;

*/

template<typename Key>
class element_not_found : public exception {
	Key key;
	static ostringstream cnvt;

public:
	element_not_found(const Key& _key) : exception("Not found the given key"), key(_key) {}

	virtual const char* what() const throw() {
		cnvt.str("");
		cnvt << "Not found the given key: " << getKey();
		return cnvt.str().c_str();
	}

	const Key& getKey() const {
		return key;
	}

};

template<typename Key>
ostringstream element_not_found<typename Key>::cnvt;




class zero_pointer : public exception {
	static ostringstream cnvt;

public:
	zero_pointer() : exception("Zero pointer"){}

	virtual const char* what() const throw() {
		cnvt.str("");
		cnvt << exception::what();
		return cnvt.str().c_str();

	}

};
ostringstream zero_pointer::cnvt;


template<typename Key, typename Info>
class Dictionary {
	struct Node {
		Key key;
		Info info;
		short int balanceFactor;

		Node* left;
		Node* right;

		/* ----------------------------- Constructors for Node ----------------------------- */
		Node() : key(0), info(0), balanceFactor(0), left(nullptr), right(nullptr) {}
		Node(const Key& _key, const Info& _info) : key(_key), info(_info), balanceFactor(0), left(nullptr), right(nullptr) {}
	};
	Node* root;


	/* -------------------------------------------- Auxiliary private methods -------------------------------------------- */
	int height(Node* subtree) const {
		if (!subtree)
			return 0; //If height is defined as number of nodes, then return 0;
		else
			return 1 + max(height(subtree->left), height(subtree->right));
	}

	int balanceFactorAux(Node* subtree) const {
		if (!subtree)
			throw zero_pointer();
		return height(subtree->right) - height(subtree->left);
	}

	void findBalanceFactor(Node* subtree) {
		subtree->balanceFactor = balanceFactorAux(subtree);
	}

	void deleteRecur(Node* subtree) {
		if (subtree) {
			deleteRecur(subtree->left);
			deleteRecur(subtree->right);
			delete subtree;
		}
	}

	void inorderRecur(ostream& os, Node* subtree) {
		if (subtree) {
			inorderRecur(os, subtree->left);
			os << subtree->key << ") " << subtree->info << endl;
			inorderRecur(os, subtree->right);
		}
	}

	void graphRecur(ostream& os, int indent, Node* subtree) const {
		if (subtree) {
			graphRecur(os, indent + 8, subtree->right);
			//	os << endl << setw(indent) << " " << subtree->key << ") h: " << height(subtree) << "; bf: " << subtree->balanceFactor << endl; //setw(indent) puts space in the string to make tree look understandable
			os << setw(indent) << " " << subtree->key << ") " << subtree->info << endl; //setw(indent) puts space in the string to make tree look understandable
			graphRecur(os, indent + 8, subtree->left);
		}
	}

	Node* findRecur(Node* subtree, const Key& _key) const {
		if (!subtree) //the subtree is empty
			return NULL;
		if (_key < subtree->key) //search left subtree
			return findRecur(subtree->left, _key);
		else
			if (subtree->key < _key) //search right subtree
				return findRecur(subtree->right, _key);
			else //item is found
				return subtree;
	}


	/* ----------------------------- Methods connected to balance ----------------------------- */
	Node* rotateRight(Node* subtree) {
		Node* temp = subtree->left;
		subtree->left = temp->right;
		temp->right = subtree;
		findBalanceFactor(subtree);
		findBalanceFactor(temp);
		return temp;
	}
	Node* rotateLeft(Node* subtree) {
		Node* temp = subtree->right;
		subtree->right = temp->left;
		temp->left = subtree;
		findBalanceFactor(subtree);
		findBalanceFactor(temp);
		return temp;
	}
	Node* balance(Node* subtree) {
		findBalanceFactor(subtree);
		if (subtree->balanceFactor == 2) {
			if (subtree->right->balanceFactor < 0)
				subtree->right = rotateRight(subtree->right);
			return rotateLeft(subtree);
		}
		if (subtree->balanceFactor == -2) {
			if (subtree->left->balanceFactor > 0)
				subtree->left = rotateLeft(subtree->left);
			return rotateRight(subtree);
		}
		return subtree;
	}


	/* -------------------------- Methods connected to removing nodes ------------------------- */
	Node* findMinRecur(Node* subtree) {
		if (subtree->left)
			return findMinRecur(subtree->left);
		else
			return subtree;
	}
	Node* removeMinRecur(Node* subtree) {
		if (!subtree->left)
			return subtree->right;
		subtree->left = removeMinRecur(subtree->left);
		return balance(subtree);
	}
	Node* removeRecur(Node* subtree, const Key& _key) {
		if (!subtree)
			return nullptr;
		if (_key < subtree->key)
			subtree->left = removeRecur(subtree->left, _key);
		else
			if (_key > subtree->key)
				subtree->right = removeRecur(subtree->right, _key);
			else { //  k == p->key
				Node* left = subtree->left;
				Node* right = subtree->right;
				delete subtree;
				if (!right)
					return left;
				Node* min = findMinRecur(right);
				min->right = removeMinRecur(right);
				min->left = left;
				return balance(min);
			}
			return balance(subtree);
	}


	/* ------------------------- Methods connected to inserting nodes ------------------------- */
	Node* insertRecur(Node* subtree, const Key& _key, const Info& _info) {
		if (!subtree)
			return new Node(_key, _info);
		if (_key < subtree->key)
			subtree->left = insertRecur(subtree->left, _key, _info);
		else
			subtree->right = insertRecur(subtree->right, _key, _info);
		return balance(subtree);
	}
	Node* copyRecur(Node* from) {
		if (!from)
			return nullptr;
		Node* temp = new Node(from->key, from->info);
		temp->left = copyRecur(from->left);
		temp->right = copyRecur(from->right);
		return temp;
	}

public:
	/* -------------------------------------------- Constructors, destructor --------------------------------------------- */
	Dictionary() {
		root = nullptr;
	}

	Dictionary(const Dictionary& copied) {
		root = copyRecur(copied.root);
	}

	~Dictionary() {
		clear();
	}


	/* ------------------------------------------------ Printing methods ------------------------------------------------- */
	void inorder(ostream& os) {
		os << endl << "In-order printing: " << endl;
		inorderRecur(os, root);
		os << endl;
	}
	friend ostream& operator<<(ostream& os, Dictionary& printed) {
		printed.inorder(os);
		return os;
	}

	void graph() const {
		cout << endl << "Graph printing: " << endl;
		graphRecur(cout, 0, root);
		cout << endl;
	}


	/* ----------------------------------------------------- Insert ------------------------------------------------------ */
	void insert(const Key& _key, const Info& _info) {
		Node* found = findRecur(root, _key);
		if (!found)
			root = insertRecur(root, _key, _info);
		else
			found->info = _info;
	}


	/* ----------------------------------------------------- Remove ------------------------------------------------------ */
	void remove(const Key& _key) {
		try {
			if (!exists(_key))
				throw exception();
			root = removeRecur(root, _key);
		}
		catch (exception& e) {
			std::cerr << "exception caught: " << e.what() << '\n';
		}
	}

	void clear() {
		deleteRecur(root);
		root = nullptr;
	}

	/* ------------------------------------------------------ Other ------------------------------------------------------ */
	bool exists(const Key& _key) const {
		return findRecur(root, _key);
	}

	const Info& operator[](const Key& _key) const {
		try {
			Node* found = findRecur(root, _key);
			if (!found)
				throw element_not_found<Key>(_key);
			return found->info;
		}
		catch (const element_not_found<Key>& e) {
			std::cerr << e.what() << endl;
			throw exception();
		}
	}

};


#endif // TREE_H_INCLUDED
