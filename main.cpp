#include "tree.h"

int main() {


	//-------------------------------- CREATING A TREE t1
	Dictionary<int, string> t1;

	//-------------------------------- Printing empty t1
	cout << t1;
	t1.graph();

	//-------------------------------- Inserting one element and printing
	t1.insert(1, "one");
	cout << t1;
	t1.graph();

	//-------------------------------- Inserting few more elements and printing
	t1.insert(2, "two");
	t1.insert(10, "ten");
	t1.insert(-10, "-ten");
	t1.insert(13, "thirteen");
	t1.insert(4, "four");
	t1.insert(6, "six");
	t1.insert(-5, "-five");
	cout << t1;
	t1.graph();

	//-------------------------------- Inserting two already existing elements and printing
	t1.insert(-10, "-ten v.2");
	t1.insert(4, "four v.2");
	cout << t1;
	t1.graph();

	//-------------------------------- Removing several elements, including root, and printing
	t1.remove(2);
	t1.remove(4);
	t1.remove(13);
	cout << t1;
	t1.graph();

	//-------------------------------- Trying to remove non-existing element (exception will be thrown)
	//t1.remove(1000);
	cout << t1;
	t1.graph();

	//-------------------------------- Using subscript operator for existing elements
	cout << "t1[10] = " << t1[10] << endl;
	cout << "t1[-5] = " << t1[-5] << endl;

	//-------------------------------- Using subscript operator for non-existing element (exception will be thrown)
	cout << "t1[1000] = " << t1[1000];


	//-------------------------------- CREATING A DYNAMIC TREE t2 BY COPYING t1
	Dictionary<int, string>* t2 = new Dictionary<int, string>(t1);

	//-------------------------------- Printing t2
	cout << *t2;
	t2->graph();


	//-------------------------------- CREATING A DYNAMIC TREE t3 BY COPYING t2
	Dictionary<int, string>* t3 = new Dictionary<int, string>(*t2);

	//-------------------------------- Printing t3 after deleting (!) t2
	delete t2;
	cout << *t3;
	t3->graph();
	delete t3;

	//-------------------------------- CREATING A TREE t4 BY COPYING EMPTY TREE t1
	t1.clear();
	Dictionary<int, string> t4(t1);

	//-------------------------------- Printing t4 (empty)
	cout << t4;
	t4.graph();
}
