#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;
vector<string> split(const string& str, const string& delim)
{
    vector<string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();
        string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    return tokens;
}
void examineLine(string line ){

}
void readFiles(string filename, string filename2 ,string outputFile){
    vector<string> setOfStates;
    vector<string> inputAlphabet;
    vector<string> stackAlphabet;
    vector<string> transitions;
    cout << filename << filename2 << outputFile << endl;
    ifstream file(filename);
    if(!file) {
        cout << "Cannot open input file.\n";
        exit(1);
    }
    string str;
    while (getline(file, str)) {
        cout << str << endl;
        vector<string> main = split(str, ":");
        if(main[0] == "Q"){
            vector<string> input = split(main[1], "=>");
            vector<string> alphabet = split(input[0], ",");
            for (auto i : alphabet){ // pushing to alphabet vector
                setOfStates.push_back(i);
            }
        }else if(main[0] == "A"){
            vector<string> alphabet = split(main[1], ",");
            for (auto i : alphabet){ // pushing to alphabet vector
                inputAlphabet.push_back(i);
            }
        }else if(main[0] == "Z"){
            vector<string> alphabet = split(main[1], ",");
            for (auto i : alphabet){ // pushing to alphabet vector
                stackAlphabet.push_back(i);
            }
        }else if(main[0] == "T"){
            cout << "in T " << endl;
            transitions.push_back(main[1]);
        }
    }
    string word;
    cout <<" transitions.size()" << endl;
    cout << transitions.size() << endl;
    for (decltype(inputAlphabet.size()) i = 0; i < inputAlphabet.size(); i++)
    {
        cout << inputAlphabet[i] << endl;
    }
    file.close();
}
int main(int argc, char **argv) {
    //std::cout << "Hello, World!" << std::endl;
    readFiles(argv[1],argv[2],argv[3]);
    return 0;
}
