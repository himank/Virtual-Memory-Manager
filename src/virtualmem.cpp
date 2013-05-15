//============================================================================
// Name        : virtualmem.cpp
// Author      : Himank Chaudhary
// Version     :
// Copyright   : Your copyright notice
// Description : Main here...!!
//============================================================================

#include <iostream>
#include <string>
#include "../src/virtualmem.h"
#include "../src/algo.h"
#include <fstream>
#include <stdlib.h>
#include <algorithm>
#include <sstream>
using namespace std;
void parse(vector<string> &, string);
void printh();
int main( int argc, char *argv[] )
 {

        int opt = 0;
		string frame ="5";
		string replacementPolicy = "FIFO";
		string inputFile ;
        bool summary = false;
		opt = getopt( argc, argv,"hf:r:i:" );
        while( opt != -1 )
        {
            switch( opt )
            {
				case 'h':
					summary = true;
					break;
				case 'f':
                    frame.assign(optarg);
                	break;

                case 'r':
                     replacementPolicy.assign(optarg);
                    break;

                case 'i':
                    inputFile.assign(optarg);
                    break;

                default:
                    cout<<"Wrong parameters: Try again"<<endl;
                    exit(1);
                	break;
            }

            opt = getopt( argc, argv, "f:r:i:" );

        }
    if(summary)
    	printh();
	for(size_t i=0; i<frame.length();i++)
	{
		if(!isdigit(frame[i]))
		{
			cout<<"Frame Size Numeric Only: Try Again:"<<endl;
			exit(1);
		}
	}
	int frames;
	std::stringstream str(frame);
	str >> frames;
	vector<string> input;
	parse(input,inputFile);
	Algorithm algo;
	std::transform(replacementPolicy.begin(), replacementPolicy.end(),replacementPolicy.begin(),::toupper);

	if (replacementPolicy == "FIFO")
		algo.fifoReplacement(input, frames);
	else if(replacementPolicy == "LRU-STACK")
		algo.lruReplacement(input, frames);
	else if(replacementPolicy == "LFU")
		algo.lfuReplacement(input, frames);
	else if(replacementPolicy == "LRU-CLOCK")
		algo.lruClockReplacement(input, frames);
	else if(replacementPolicy == "LRU-REF8")
		algo.lruReplacement_additionalbit(input,frames);
	else
	{
		cout<<"Replacement policy name incorrect : Try again "<<endl;
		exit(1);
	}
	cin.get();
	return 0;
} // End of main
void parse(vector<string> &input,string inputFile)
{

	string file_input,s;
	ifstream file(inputFile.c_str());
	if(file.is_open())
	{

		while(file.good())
		{

			getline(file,s);
			file_input = file_input+s;
		}
		file.close();
	}
	else
	{
		cout<<"Enter the Input String: "<<endl;
		getline(cin,file_input);

	}


	char *input_tokenizer = new char[file_input.size() + 1];
	copy(file_input.begin(),file_input.end(),input_tokenizer);
	input_tokenizer[file_input.size()] = '\0';
	char *tokens = input_tokenizer;
	tokens = strtok(input_tokenizer," \t\n ");
	while(tokens != NULL)
	{
		string s(tokens);
		input.push_back(s);
		tokens = strtok(NULL," \t\n ");
	}

	for(vector<string>::iterator it=input.begin();it != input.end(); ++it)
	{
		//cout<<*it<<endl;
		string s = *it;
		for(size_t i=0;i<s.length();i++)
		{
			if(!isdigit(s[i]))
			{
				cout<<"Try Again: Invalid Input Entered"<<endl;
				exit(1);
			}
		}
	}
	delete [] input_tokenizer;
}
void printh()
{
cout<<endl<<"**************************************************************************"<<endl;
cout<<"virtualmem [−h] [-f available-frames] [−r replacement-policy] [−i input_file]"<<endl<<endl;
cout<<"Give -f then value to change by default frames value"<<endl;
cout<<"Give -r then relacement-policy to change by default value for example -r lru"<<endl;
cout<<"Give -i and then filename of input file for example: home/workspace/virtualmem/src/input.txt"<<endl;
cout<<"Press Ctrl+c anytime to exit the server";
cout<<endl<<"**************************************************************************"<<endl;
exit(1);
}
