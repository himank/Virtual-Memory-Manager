//============================================================================
// Name        : virtualmem.cpp
// Author      : Himank Chaudhary
// Version     :
// Copyright   : Your copyright notice
// Description : Algorithm Implementation Here..!!
//============================================================================

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "../src/algo.h"
#include<sys/types.h>
#include<sys/time.h>
#include <stdlib.h>
using namespace std;

bool Algorithm::isDuplicateFifo(Node *head, string s)
{
	if(head == NULL)
		return false;
	else {
		Node *node;
		node = head;
		while(node != NULL) {
			if(node->value == s)
				return true;
			node = node->next;
		}
	}

return false;
}

bool Algorithm::isDuplicate(Doublylist *head, string s)
{
	if(head == NULL)
		return false;
	else {
		Doublylist *node;
		node = head;
		while(node != NULL) {
			if(node->value == s)
				return true;
			node = node->next;
		}
	}

return false;
}

bool Algorithm::isDuplicateLruClock(Circularlist **head, string s)
{
	if(*head == NULL)
		return false;
	else {
		Circularlist *node;
		node = *head;
		do {
			if(node->value == s) {
				node->ref_bit = 1;
				return true;
			}
			node = node->next;
		} while(node != *head);
	}

	return false;
}

//********************************************************************** Starting of FIFO replacement
void Algorithm::fifoReplacement(vector<string> &input, int frames)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	uint begin = tv.tv_usec;
	Node *head = new Node;
	head = NULL;
	Node *node;
	int counter = 0, no_replacement = 0;
	size_t i=0;
	while(input.size() > i) {
		bool flag = isDuplicateFifo(head,input[i]);
		if(counter < frames && !flag) {
			if(head == NULL) {
				node = new Node;
				node->value = input[i];
				node->next = NULL;
				//head = new Node;
				head = node;
				i++;
				counter++;
			} else {
				node->next = new Node;
				node = node->next;
				node ->value = input[i];
				node->next = NULL;
				i++;
				counter++;
			}
		} else if(flag) {
				i++;
		} else {
			Node *temp;
			temp = head;
			head = head->next;
			delete temp;
			counter--;
			no_replacement++;
		}
	}

	gettimeofday(&tv, NULL);
	uint end = tv.tv_usec;
	ResultSet r_optimal = optimalReplacement(input,frames);
	ResultSet r_fifo;
	r_fifo.page_replacements = no_replacement;
	r_fifo.time = (end-begin);
	printResult(r_optimal,r_fifo,"FIFO");
	
	delete node;
	delete head;
}

//**************************************************************** Staring of LRU replacement
void Algorithm::lruReplacement(vector<string> &input, int frames)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	uint begin = tv.tv_usec;
	Doublylist *head = new Doublylist;
	head = NULL;
	Doublylist *node;
	int counter = 0, no_replacement = 0;
	size_t i=0;
	while(input.size() > i) {
		bool flag = isDuplicate(head,input[i]);
		if(counter < frames && !flag) {
			if(head == NULL) {
				node = new Doublylist;
				node->value = input[i];
				node->next = NULL;
				node->prev = NULL;
				head = node;
				i++;
				counter++;
			} else {
				node->next = new Doublylist;
				Doublylist *temp = NULL;
				temp = node->next;
				temp ->value = input[i];
				temp->next = NULL;
				temp->prev = node;
				node = node->next;
				i++;
				counter++;
			}
		} else if(flag) {
			Doublylist *temp = NULL;
			temp = head;
			while(temp != NULL && (temp->value != input[i])) {
				temp = temp->next;
			}

			if(temp->next != NULL && temp->prev != NULL) {
				temp->next->prev = temp->prev;
				temp->prev->next = temp->next;
				node->next = temp;
				temp->next = NULL;
				temp->prev = node;
				node = node->next;
			} else if(temp->prev == NULL) {
				head = head->next;
				temp->next->prev = temp->prev;
				node->next = temp;
				temp->next = NULL;
				temp->prev = node;
				node = node->next;
			}
			i++;
		} else {
			Doublylist *temp = NULL;
			temp = head;
			head = head->next;
			head->prev = NULL;
			delete temp;
			counter--;
			no_replacement++;
		}
	}

	gettimeofday(&tv, NULL);
	uint end = tv.tv_usec;
	ResultSet r_optimal = optimalReplacement(input,frames);
	ResultSet r_lru;
	r_lru.page_replacements = no_replacement;
	r_lru.time = (end-begin);
	printResult(r_optimal,r_lru,"LRU-Stack");
	
	delete node;
	delete head;
}

//*************************************************************** Starting of LFU Replacement
void Algorithm::lfuReplacement(vector<string> &input, int frames)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	uint begin = tv.tv_usec;
	Doublylist *head = new Doublylist;
	head = NULL;
	Doublylist *node;
	int counter = 0, no_replacement = 0;
	size_t i=0;
	while(input.size() > i)	{
		// loop work till we have elements in vector

		// if the coming string is already present in list
		bool flag = isDuplicate(head,input[i]);	
		if(counter < frames && !flag) {
			// Add elememts till counter is less than frame
			if(head == NULL) {
				node = new Doublylist;
				node->value = input[i];
				node->next = NULL;
				node->prev = NULL;
				//head = new Node;
				head = node;
				i++;
				counter++;
			} else {
				node->next = new Doublylist;
				Doublylist *temp = NULL;
				temp = node->next;
				temp ->value = input[i];
				temp->next = NULL;
				temp->prev = node;
				node = node->next;
				i++;
				counter++;
			}
		} else if(flag) {
			i++;
		}else {
			// if all frames are done

			// To store the occurence of the string
			vector<int> occurence(frames);
			int count =0;
			size_t j = 0, k=0;
			Doublylist *temp = NULL;
			temp = head;
			while(temp != NULL) {
				while(j < i) {
					if(temp->value == input[j]) {
						count = count + 1;
					}
					j++;
				}

				occurence.at(k) = count;
				k++;
				j = 0;
				count = 0;
				temp = temp->next;
			}
			k=0;
			int min = occurence[k];
			size_t min_location =0;
			for(k = 0;k < occurence.size() - 1; k++) {
				// finding the one which is refernce very late
				if(min > occurence[k+1]) {
					min = occurence[k+1];
					min_location =k+1;
				}
			}
			size_t c = 0;
			temp = head;
			while(c < min_location) {
				temp = temp->next;
				c++;
			}

			if(min_location > 0) {
				if(min_location < occurence.size() - 1) {
					// if the node to be pop out is in between
					temp->next->prev = temp->prev;
					temp->prev->next = temp->next;
					temp->next = head;
					temp->prev = NULL;
					head = temp;
				} else {
					// if it is at the tail
					temp->prev->next = temp->next;
					node = temp->prev;
					temp->next = head;
					temp->prev = NULL;
					head = temp;
				}
			}

			temp = head;
			head = head->next;
			head->prev = NULL;
			delete temp;
			counter--;
			no_replacement++;
		}
	}
	
	gettimeofday(&tv, NULL);
	uint end = tv.tv_usec;
	ResultSet r_optimal = optimalReplacement(input,frames);
	ResultSet r_lfu;
	r_lfu.page_replacements = no_replacement;
	r_lfu.time = (end-begin);
	printResult(r_optimal,r_lfu,"LFU");
	
	delete node;
	delete head;
}

//*******************************************************************************Starting LRU Clock Replacement
void Algorithm::lruClockReplacement(vector<string> &input, int frames)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	uint begin = tv.tv_usec;
	Circularlist *head;
	head = NULL;
	Circularlist *node;

	int counter = 0, no_replacement = 0;
	size_t i=0;
	while(input.size() > i) {
		bool flag = isDuplicateLruClock(&head,input[i]);
		if(counter < frames && !flag) {
			if(head == NULL) {
				node = new Circularlist;
				head = node;
				node->value = input[i];
				node->ref_bit = 0;
				node->next = head;
				//head = new Node;

				i++;
				counter++;
			} else {
				node->next = new Circularlist;
				node = node->next;
				node ->value = input[i];
				node->ref_bit = 0;
				node->next = head;
				i++;
				counter++;
			}
		} else if(flag) {
				i++;
		} else {
			Circularlist *temp;
			temp = head;
			int check_refbit =0;
			while(check_refbit == 0) {
				if(temp->ref_bit == 0 && temp == head) {
					head = head->next;
					delete temp;
					node->next=head;
					counter--;
					no_replacement++;
					check_refbit = 1;
				} else if(temp->ref_bit == 0) {
					head->next = temp->next;
					node = head;
					head = temp->next;
					delete temp;
					counter--;
					no_replacement++;
					check_refbit = 1;
				} else {
					temp->ref_bit = 0;
					head = temp;
					temp = temp->next;

				}
			}
		}
	}

	gettimeofday(&tv, NULL);
	uint end = tv.tv_usec;
	ResultSet r_optimal = optimalReplacement(input,frames);
	ResultSet r_clock;
	r_clock.page_replacements = no_replacement;
	r_clock.time = (end-begin);
	printResult(r_optimal,r_clock,"LRU-Clock");
	
	delete node;
}

//**************************************************************** Staring of LRU using 8 Refernce bit
void Algorithm::lruReplacement_additionalbit(vector<string> &input, int frames)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	uint begin = tv.tv_usec;
	Doublylist *head = new Doublylist;
	head = NULL;
	Doublylist *node;
	int counter = 0, no_replacement = 0;
	size_t i=0;
	while(input.size() > i) {
		bool flag = isDuplicate(head,input[i]);
		if(counter < frames && !flag) {
			if(head == NULL) {
				node = new Doublylist;
				node->value = input[i];
				node->next = NULL;
				node->prev = NULL;
				head = node;
				i++;
				counter++;
			} else {
				node->next = new Doublylist;
				Doublylist *temp = NULL;
				temp = node->next;
				temp ->value = input[i];
				temp->next = NULL;
				temp->prev = node;
				node = node->next;
				i++;
				counter++;
			}
		} else if(flag) {
			i++;
		} else {
			vector<string> occurence(frames);	// To store the future positions of the string
			int check_till = 0;
			int k=0,j = i-1;
			Doublylist *temp = NULL;
			temp = head;
			while(temp != NULL) {
				string s;
				while(check_till < 8 && j>=0) {
					if(temp->value == input[j]) {
						s.push_back('1');
					} else {
						s.push_back('0');
					}
					j--;
					check_till++;
				}
				occurence.at(k) = s;
				k++;
				j = i-1;
				temp = temp->next;
			}
			k=0;
			vector<int> decimal_value(frames);
			for(k=0;k<frames-1;k++) {
				// finding the one which is refernce very late
				int value = 0, twos = 1;
				string s_temp = occurence[k];
				for(int i=s_temp.size()-1; i >= 0; i--) {
					int single_bit = s_temp[i] - '0';
					if(single_bit % 10 == 1) {
						value = single_bit*twos;
					}
					twos*=2;
				}
				decimal_value.at(k) = value;
			}
			
			int min = decimal_value[0];
			size_t min_location =0;
			for(k=1;k<occurence.size()-1;k++) {
				if( min > decimal_value[k]) {
					min = decimal_value[k];
					min_location = k;
				}
			}

			size_t c = 0;
			temp = head;
			while(c < min_location) {
				temp = temp->next;
				c++;
			}

			if(min_location > 0) {
				if(min_location < occurence.size() - 1) {
					// if the node to be pop out is in between
					temp->next->prev = temp->prev;
					temp->prev->next = temp->next;
					temp->next = head;
					temp->prev = NULL;
					head = temp;
				} else {
					// if it is at the tail
					temp->prev->next = temp->next;
					node = temp->prev;
					temp->next = head;
					temp->prev = NULL;
					head = temp;
				}
			}
			temp = head;
			head = head->next;
			head->prev = NULL;
			delete temp;
			counter--;
			no_replacement++;
		}
	}

	gettimeofday(&tv, NULL);
	uint end = tv.tv_usec;
	ResultSet r_optimal = optimalReplacement(input,frames);
	ResultSet r_clock;
	r_clock.page_replacements = no_replacement;
	r_clock.time = (end-begin);
	printResult(r_optimal,r_clock,"LRU-REF8");

	delete node;
	delete head;
}

//**************************************************************************** Starting of Optimal Replacement
Algorithm::ResultSet Algorithm::optimalReplacement(vector<string> &input, int frames)
{
	struct timeval tv;
	ResultSet rs;
	gettimeofday(&tv, NULL);
	uint begin = tv.tv_usec;
	Doublylist *head = new Doublylist;
	head = NULL;
	Doublylist *node;
	int counter = 0, no_replacement = 0;
	size_t i=0;
	while(input.size() > i)	{
		// loop work till we have elements in vector

		// if the coming string is already present in list
		bool flag = isDuplicate(head,input[i]);

		if(counter < frames && !flag) {
			// Add elememts till counter is less than frame
			if(head == NULL) {
				node = new Doublylist;
				node->value = input[i];
				node->next = NULL;
				node->prev = NULL;
				//head = new Node;
				head = node;
				i++;
				counter++;
			} else {
				node->next = new Doublylist;
				Doublylist *temp = NULL;
				temp = node->next;
				temp ->value = input[i];
				temp->next = NULL;
				temp->prev = node;
				node = node->next;
				i++;
				counter++;
			}
		} else if(flag) {
			i++;
		} else {
			// if all frames are done
			vector<int> position(frames);	// To store the future positions of the string
			size_t j = i, k=0;
			Doublylist *temp = NULL;
			temp = head;
			while(temp != NULL) {
				while(input.size() > j) {
					if(temp->value == input[j]) {
						position.at(k) = j;
						break;
					} else {
						position.at(k) = 1000;
					}
					j++;
				}
				k++;
				j = i;
				temp = temp->next;
			}
			k=0;
			int max = position[k];
			size_t max_location =0;
			for(k=0;k<position.size()-1;k++) {
				// finding the one which is refernce very late
				if(max < position[k+1]) {
					max = position[k+1];
					max_location =k+1;
				}
			}
			size_t c = 0;
			temp = head;
			while(c < max_location) {
				temp = temp->next;
				c++;
			}
			if(max_location > 0) {
				if(max_location < position.size() - 1) {
					// if the node to be pop out is in between
					temp->next->prev = temp->prev;
					temp->prev->next = temp->next;
					temp->next = head;
					temp->prev = NULL;
					head = temp;
				} else {
					// if it is at the tail
					temp->prev->next = temp->next;
					node = temp->prev;
					temp->next = head;
					temp->prev = NULL;
					head = temp;
				}
			}
			temp = head;
			head = head->next;
			head->prev = NULL;
			delete temp;
			counter--;
			no_replacement++;
		}
	}

	gettimeofday(&tv, NULL);
	uint end = tv.tv_usec;
	rs.page_replacements = no_replacement;
	rs.time = (end-begin);
	delete node;
	delete head;
	return rs;
}

void Algorithm::printResult(ResultSet r_optimal, ResultSet r_other, string s)
{
	cout<<"# of page replacement with "<<s<<" : "<<r_other.page_replacements<<endl;
	cout<<"# of page Replacement with Optimal"<<" : "<<r_optimal.page_replacements<<endl;
	if(r_optimal.page_replacements <= r_other.page_replacements) {
		int diff = r_other.page_replacements - r_optimal.page_replacements;
		float per = ((float)diff/(float)r_optimal.page_replacements)*100;
		cout<<"% page replacement penalty using "<<s<<" : "<<per<<endl<<endl;
	} else {
		cout<<"% page replacement penalty using "<<s<<" : "<<0<<endl<<endl;
	}

	cout<<"Total time to run "<<s<<" algorithm"<<" : "<<r_other.time<<endl;
	cout<<"Total time to run Optimal algorithm"<<" : "<<r_optimal.time<<endl;
	if(r_optimal.time >= r_other.time) {
		int diff = r_optimal.time - r_other.time;
		float per = ((float)diff/(float)r_optimal.time)*100;
		cout<<s<<" is "<<per<<"% faster than Optimal Algorithm"<<endl<<endl;
	} else {
		int diff = r_other.time - r_optimal.time ;
		float per = ((float)diff/(float)r_optimal.time)*100;
		cout<<s<<" is "<<per<<"% slower than Optimal Algorithm"<<endl<<endl;
	}
	exit(1);
}

//**********************************************************************************************END