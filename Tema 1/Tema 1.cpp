#include "pch.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <regex>

using namespace std;

struct letter
{
	char* text;
	letter* next;
};

struct link 
{
	int dictionaryId;
	int node;
	link *nextLink;
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

	 link *links;

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

int getDictionaryId(letter* dictionary, char text[])
{
	int i = 0;
	while(dictionary!=NULL)
	{
		if (!strcmp(dictionary->text, text))
			return i;
		i++;
		dictionary = dictionary->next;
	}
	return -1;
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
		if(!strcmp(p->text, text))
			return; 
		p = p->next;
	}
	p->next = v;
}

int addLink(link* &links, int node, int dictionryId, int node_count)
{
	auto n_link = new link;
	n_link->node = node;
	n_link->dictionaryId = dictionryId;
	n_link->nextLink = NULL;
	if (n_link->dictionaryId < 0)
		return 2;
	if (n_link->node >= node_count)
		return 3;
	if(links == NULL)
	{
		links = n_link;
		return 0;
	}

	link* p = links;
	while (p->nextLink != NULL)
	{
		if (p->node == n_link->node && p->dictionaryId == n_link->dictionaryId)
			return 0;
		p = p->nextLink;
	}
	p->nextLink = n_link;
	return 0;
}

int createLinks(char line[],int node_count, Node* start, letter* dictionary, char const splitter[])
{
	char *next_val = NULL;
	auto val = strtok_s(line, splitter, &next_val);
	int i = 0;
	char* data[3];

	while (val != NULL)
	{
		data[i] = new char[sizeof(val)];
		strcpy_s(data[i++], sizeof(val), val);
		val = strtok_s(NULL, splitter, &next_val);
	}
	auto const node = atoi(data[0]);
	if (node > node_count)
		return 1;

	return addLink(start[node - 1].links, atoi(data[1]) - 1, getDictionaryId(dictionary, data[2]), node_count);
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

		while(f.good())
		{
			f.getline(line, sizeof(line));
			const auto info = createLinks(line, node_count, start, dictionary, splitter);
			if (info != 0)
				throw info;
		}
	}
	catch (int e)
	{
		cout << "eroare la citire: " << e;
	}
	return 0;
}

bool check(char word[], Node* start, int current_node, letter* dictionary)
{
	if (strlen(word) == 0 && start[current_node].stare == Node::final_st)
		return true;
	bool result = false;
	link* p = start[current_node].links;
	char *st_letter = new char[3];
	st_letter[0] = word[0];
	st_letter[1] = '\0';
	auto dic_id = getDictionaryId(dictionary, st_letter);
	if (dic_id >= 0)
		while (p != NULL)
		{
			if(p->dictionaryId == dic_id)
			{
				char* n_word = new char[sizeof(char) * strlen(word)];
				strcpy_s(n_word, sizeof(char) * strlen(word), word + 1);
				result = result || check(n_word, start, p->node, dictionary);
			}
			p = p->nextLink;
		}
	return result;
}

bool checkWord(char word[], Node* start, letter* dictionary, int node_count)
{
	bool result = false;
	for (int i = 0; i < node_count; i++)
	{
		if (start[i].stare == Node::initial_st)
			result = result || check(word, start, i, dictionary);
	}
	return result;
}

int main()
{
	char const file_name[] = "date.in";
	char const splitter[] = " ,.'-*&^%$#@!~`|<>?/\"[]{}+_()!=";
	char const input_message[] = "Insert word to be tested:";

	int node_count;
	Node* start;				//list of nodes

	letter *dictionary = NULL;			//list of letters
	read(file_name, node_count, start, dictionary, splitter);
	char word[500];
	cout << input_message << endl;
	cin >> word;
	if (checkWord(word, start, dictionary, node_count))
		cout << "DA!";
	else 
		cout << "NU!";

	return 0;
}
