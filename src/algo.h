/*
 * algo.h
 *
 *  Created on: 16-Nov-2012
 *      Author: himank
 */

#ifndef ALGO_H_
#define ALGO_H_
#include<iostream>
#include<string.h>
#include<vector>
#include<sys/types.h>

using namespace std;

class Algorithm {
public:
	struct Node
	{
		string value;
		Node *next;
	};

	struct Doublylist
	{
		string value;
		Doublylist *prev;
		Doublylist *next;
	};

	struct Circularlist
	{
		string value;
		int ref_bit;
		Circularlist *next;
	};

	struct ResultSet
	{
		int page_replacements;
		uint time;
	};

	ResultSet optimalReplacement(vector<string> &, int);
	void fifoReplacement(vector<string> &, int);
	bool isDuplicateFifo(Node *, string);
	void lruReplacement(vector<string> &, int);
	bool isDuplicate(Doublylist *, string);
	void lfuReplacement(vector<string> &, int);
	void lruClockReplacement(vector<string> &, int);
	bool isDuplicateLruClock(Circularlist **, string);
	void lruReplacement_additionalbit(vector<string> &input, int frames);
	void printResult(ResultSet r_optimal, ResultSet r_other, string s);
};
#endif /* ALGO_H_ */
