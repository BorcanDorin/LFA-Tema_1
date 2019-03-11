#include "pch.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <string.h>

using namespace std;

struct letter
{
	char* text;
	letter* next;
};

class Node 
{
public:
	enum state 
	{
		no_st = 0,
		initial_st = 1,
		final_st = 2
	} stare;

	 struct link {
		 int letter;
		 int node;
	 } *links;

	Node(state st);
	void set_state(state st);
};

Node::Node(state st = no_st) : stare(st) 
{

	links = NULL;
}

void Node::set_state(state st)
{
	stare = st;
};

int splitAndInitialize(char line[], char const splitter[], int nodeCount, Node *start, Node::state st) 
{
	char *next_val = NULL;
	auto val = strtok_s(line, splitter, &next_val);

	while (val != NULL)
	{
		const auto nodeNr = atoi(val);
		if (nodeNr > nodeCount)
			return 1;
		start[nodeNr - 1].set_state(st);
		val = strtok_s(NULL, splitter, &next_val);
	}
	return 0;
}

void dictionaryAdd(letter* &dictionary, char* text)
{
	auto v = new letter();
	v->next = NULL;
	v->text = new char[sizeof(text)];
	strcpy_s(v->text,sizeof(text), text);

	if(dictionary == NULL)
	{
		dictionary = v;
		return;
	}

	letter *p = dictionary;
	while (p->next != NULL)
	{
		p = p->next;
	}
	p->next = v;
}

int read(char const file_name[], int &node_count, Node* &start, letter* &dictionary, char const splitter[])
{
	try
	{
		ifstream f(file_name);
			//read line 1
		f >> node_count;
		start = new Node[node_count]();
		auto c = f.get();
			//read line 2
		char line[500];
		f.getline(line, sizeof(line));

		if (splitAndInitialize(line, splitter, node_count, start, Node::initial_st) != 0)
			throw 1;
			//read line 3
		f.getline(line, sizeof(line));

		if (splitAndInitialize(line, splitter, node_count, start, Node::final_st) != 0)
			throw 2;
			//read line 4  -- dictionary
		f.getline(line, sizeof(line));

		char *next_val = NULL;
		auto val = strtok_s(line, splitter, &next_val);

		while (val != NULL)
		{
			dictionaryAdd(dictionary, val);
			val = strtok_s(NULL, splitter, &next_val);
		}
		if (dictionary == NULL)
			throw 3;
	}
	catch (int e)
	{
		cout << "eroare la citire: " << e;
	}
	return 0;
}

int main()
{
	char const file_name[] = "date.in";
	char const splitter[] = " ,.'-*&^%$#@!~`|";

	int node_count;
	Node* start;				//list of nodes

	letter *dictionary = NULL;			//list of letters
	read(file_name, node_count, start, dictionary, splitter);
	cout << "hello";

	return 0;
}
