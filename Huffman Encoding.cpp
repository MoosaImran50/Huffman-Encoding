#include <iostream>
#include <fstream>
#include <unordered_map>
#include <algorithm>
using namespace std;


struct HNode
{
	int freq;
	char character;
	HNode* left;
	HNode* right;

	HNode()
	{
		this->freq = 0;
		this->character = '\0';
		this->left = nullptr;
		this->right = nullptr;
	}

	HNode(int key, char value)
	{
		this->freq = key;
		this->character = value;
		this->left = nullptr;
		this->right = nullptr;
	}

	HNode(int key, char value, HNode* l, HNode* r)
	{
		this->freq = key;
		this->character = value;
		this->left = l;
		this->right = r;
	}
};


class Huffman
{
	HNode* root;
	HNode* arr;
	int totalElements;
	int capacity;

	void doubleCapacity()
	{
		HNode* newArr = new HNode[this->capacity * 2];
		this->capacity *= 2;
		for (int i = 0; i < this->totalElements; i++)
		{
			newArr[i] = this->arr[i];
		}
		delete[]this->arr;
		this->arr = newArr;
	}

public:
	Huffman()
	{
		this->arr = new HNode[1];
		this->totalElements = 0;
		this->capacity = 1;
		this->root = nullptr;
	}

	Huffman(int _capacity)
	{
		if (_capacity > 0) {
			this->arr = new HNode[_capacity];
			this->totalElements = 0;
			this->capacity = _capacity;
			this->root = nullptr;
		}
	}

	void insert(int key, char value, HNode* l, HNode* r)
	{
		if (this->totalElements == this->capacity) {
			this->doubleCapacity();
		}

		arr[this->totalElements].freq = key;
		arr[this->totalElements].character = value;
		if (l != nullptr && r != nullptr) {
			arr[this->totalElements].left = new HNode(l->freq, l->character, l->left, l->right);
			arr[this->totalElements].right = new HNode(r->freq, r->character, r->left, r->right);
		}

		shiftUp(this->totalElements);
		this->totalElements++;
		this->root = &this->arr[0];
	}

	void shiftUp(int index)
	{
		int parent = (index - 1) / 2;

		if (this->arr[index].freq < this->arr[parent].freq) {
			swap(this->arr[index], this->arr[parent]);
		}
		else {
			return;
		}

		if (parent > 0) {
			shiftUp(parent);
		}
	}

	void deleteMin()
	{
		if (this->totalElements == 0) {
			return;
		}

		swap(this->arr[totalElements - 1], this->arr[0]);

		
		this->totalElements--;
		shiftDown(0);
		this->root = &this->arr[0];
	}

	void shiftDown(int i)
	{
		int lChild = i * 2 + 1;
		int rChild = i * 2 + 2;
		int min = i;

		if (lChild < this->totalElements && this->arr[lChild].freq < this->arr[min].freq) {
			min = lChild;
		}
		if (rChild < this->totalElements && this->arr[rChild].freq < this->arr[min].freq) {
			min = rChild;
		}

		if (min == i) {
			return;
		}

		swap(this->arr[min], this->arr[i]);

		shiftDown(min);
	}

	int getElementCount() const
	{
		return this->totalElements;
	}

	int getCapacity() const
	{
		return this->capacity;
	}
	
	void print()
	{
		cout<<endl<<"Printing: "<<endl;
		for(int i=0; i< totalElements; i++){
			cout<<arr[i].freq << " " <<arr[i].character << endl;
		}
		cout<<"Total Elements: "<<totalElements<<endl;
		cout<<"New Capacity: "<<capacity<<endl;

	}
	
	void MakeHuffman() {
		HNode temp1;
		HNode temp2;
		int i = 1;
		while (totalElements != 1) {
			temp1 = arr[0];
			deleteMin();
			temp2 = arr[0];
			deleteMin();
			insert(temp1.freq + temp2.freq, i + 48, &temp1, &temp2);
			i++;
		}
		shrinkHeap();
	}
	
	void shrinkHeap()
	{
		HNode* newArr = new HNode[this->totalElements];
		this->capacity = totalElements;
		for (int i = 0; i < this->capacity; i++)
		{
			newArr[i] = this->arr[i];
		}
		delete[]this->arr;
		this->arr = newArr;
		this->root = &arr[0];
	}
	
	void printCodes(HNode* node, int arr[], int top) const
	{
		// Assign 0 to left edge and recur
		if (node->left != nullptr) {

			arr[top] = 0;
			printCodes(node->left, arr, top + 1);
		}

		// Assign 1 to right edge and recur
		if (node->right != nullptr) {

			arr[top] = 1;
			printCodes(node->right, arr, top + 1);
		}

		// If this is a leaf node, then
		// it contains one of the input
		// characters, print the character
		// and its code from arr[]
		if (node->left == nullptr && node->right == nullptr) {
			cout << "Huffman Code: ";
			for (int i = 0; i < top; i++) {
				cout << arr[i];
			}
			cout << endl;
			cout << "Character: " << "'" << node->character << "'" << endl;
			cout << "Frequency: " << node->freq << endl;
			cout << endl;
		}
		
	}
	
	void printHuffman() const{
		int code[2000];
		int top = 0;

		if (this->root != nullptr && totalElements != 0) {
			printCodes(this->root, code, top);
		}
	}

	void createHuffman(char* fileName) {
		unordered_map<char, int> hashmap;

		char character;

		ifstream in;
		in.open(fileName);
		if (in.is_open()) {
			while (in.get(character)) {

				if (hashmap.find(character) == hashmap.end()) {
					hashmap.insert(make_pair(character, 1));
				}
				else {
					hashmap.find(character)->second++;
				}

			}
			unordered_map<char, int>::iterator itr;
			for (itr = hashmap.begin(); itr != hashmap.end(); itr++) {
				insert(itr->second, itr->first, nullptr, nullptr);
			}
			MakeHuffman();
		}
		else {
			cout << endl << "File Does Not Exist !" << endl << endl;
		}
		in.close();
	}

	void DeleteHuffman(HNode* node) {
		if (node)
		{
			DeleteHuffman(node->left);
			DeleteHuffman(node->right);
			delete node;
		}
	}
	
	~Huffman()
	{
		DeleteHuffman(this->root);
	}
};

///////////////////////////////////////////////////////////////////////////////////////

int main() {

	Huffman m;

	char filename[50];

	cout << endl << "Enter File Name: ";
	cin.getline(filename, 50);
	cout << endl;

	m.createHuffman(filename);

	m.printHuffman();


}