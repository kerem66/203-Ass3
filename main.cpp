#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>
#include <algorithm>
using namespace std;
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}
class Stack
{
    string *arr;
    int top;
    int capacity;

public:
    Stack(int size = 200);         // constructor
    ~Stack();                       // destructor

    void push(string);
    string pop();
    string peek();

    int size();
    bool isEmpty();
    bool isFull();
    string print();
};

Stack::Stack(int size)
{
    arr = new string [size];
    capacity = size;
    top = -1;
}

// Destructor to free memory allocated to the stack
Stack::~Stack() {
    delete[] arr;
}
void Stack::push(string x)
{
    if (isFull()){cout << "Overflow\nProgram Terminated\n";
        exit(EXIT_FAILURE);
    }

    arr[++top] = x;
}
string Stack::print() {
    string stackStr = "";
    stackStr += " [STACK]:";
    if (!isEmpty()) {
        for(int i = 0 ; i<= top ; i++){
            stackStr += arr[i] ;
            if(i != top){
                stackStr +=",";
            }
        }
    }
    stackStr +="\n";
    return stackStr;
}
string Stack::pop()
{
    if (isEmpty()){
        cout << "Underflow\nProgram Terminated\n";
        exit(EXIT_FAILURE);
    }

    return arr[top--];
}

string Stack::peek()
{
    if (!isEmpty()) {return arr[top];}
    else {exit(EXIT_FAILURE);}
}

int Stack::size() {
    return top + 1;
}

bool Stack::isEmpty() {
    return top == -1;
}

bool Stack::isFull() {
    return top == capacity - 1;
}

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
void processInput(string input , vector<string> &setOfStates , vector<string> &inputAlphabet, vector<string> &stackAlphabet , vector<string> &transitions , vector<string> &endStates, string startState, string outputFile){
    fstream outputfile;
    outputfile.open(outputFile, std::ios_base::app | std::ios_base::in | std::ios_base::out);
    Stack stack(200);
    string currentState = startState;
    vector<string> inputs = split(input, ",");
    //checking if dpda is valid
    bool valid = true;
    if(inputs.size() > 0){
        for(int v = 0 ; v<transitions.size() ; v++){
            vector<string> transition = split(transitions[v],",");
            if (find(setOfStates.begin(), setOfStates.end(), transition[0]) != setOfStates.end()){ //finding in related vector
            }else{
                if(setOfStates[setOfStates.size()-1] != transition[0]){
                    valid = false;
                    break;
                }
            }
            if ((transition[1] == "e" || transition[1] == "$") || find(inputAlphabet.begin(), inputAlphabet.end(), transition[1]) != inputAlphabet.end()){//finding in related vector
            }else{
                if(inputAlphabet[inputAlphabet.size()-1] != transition[1]){
                    valid = false;
                    break;
                }
            }
            if ((transition[2] == "e" || transition[2] == "$") || find(stackAlphabet.begin(), stackAlphabet.end(), transition[2]) != stackAlphabet.end()){//finding in related vector
            }else{
                if(stackAlphabet[stackAlphabet.size()-1] != transition[2]){
                    valid = false;
                    break;
                }
            }
            if (find(setOfStates.begin(), setOfStates.end(), transition[3]) != setOfStates.end()){//finding in related vector
            }else{
                if(setOfStates[setOfStates.size()-1] != transition[3]){
                    valid = false;
                    break;
                }
            }
            if ((transition[4] == "e" || transition[4] == "$") || find(stackAlphabet.begin(), stackAlphabet.end(), transition[4]) != stackAlphabet.end()){//finding in related vector

            }else{
                if(stackAlphabet[stackAlphabet.size()-1] != transition[4]){
                    valid = false;
                    break;
                }
            }
        }
    }


    if(!valid){ // input is not valid
        outputfile << "Error [1]:DPDA description is invalid!\n";
        outputfile.close();
        exit(1);
    }else{
        if(inputs.size() == 0){ // there is empty line input checking if starting state is also an end state
            for(int e = 0 ; e<endStates.size() ; e++){
                if(currentState == endStates[e]){
                    if(stack.isEmpty() || (stack.size() == 1 && stack.peek() == "$")){
                        outputfile << "ACCEPT\n\n";
                    }
                }
            }
        }else{
            for (int i=0; i<inputs.size(); i++) //reading input lines
            {
                string inputCharacter = inputs[i];
                for(int j = 0 ; j<transitions.size() ; j++){ // going through all transition to find most suitable
                    vector<string> transition = split(transitions[j],",");

                    if(currentState == transition[0] && transition[1] == "e"){
                        if(stack.isEmpty()){
                            currentState = transition[3];
                            if(transition[4] != "e"){
                                stack.push(transition[4]);
                            }
                            outputfile << transition[0] << "," << transition[1] << "," << transition[2] << " => " << transition[3] << "," << transition[4];
                            string stackString = stack.print();
                            outputfile << stackString;
                        }else if (stack.peek() == transition[2] || (stack.peek() == "e" || transition[2] == "e")){
                            if(stack.peek() == transition[2]){
                                stack.pop();
                            }
                            currentState = transition[3];
                            if(transition[4] != "e"){
                                stack.push(transition[4]);
                            }
                            outputfile << transition[0] << "," << transition[1] << "," << transition[2] << " => " << transition[3] << "," << transition[4];
                            string stackString = stack.print();
                            outputfile << stackString;
                            if(transition[1] != "e"){
                                break;
                            }

                        }
                    }else if(currentState == transition[0] && transition[1] == inputCharacter){
                        if(stack.isEmpty()){
                            currentState = transition[3];
                            if(transition[4] != "e"){
                                stack.push(transition[4]);
                            }
                            outputfile << transition[0] << "," << transition[1] << "," << transition[2] << " => " << transition[3] << "," << transition[4];
                            string stackString = stack.print();
                            outputfile << stackString;
                            break;
                        }else if (stack.peek() == transition[2] || (stack.peek() == "$" || transition[2] == "e")){
                            if(stack.peek() == transition[2]){
                                stack.pop(); // problem with pop
                            }
                            currentState = transition[3];
                            if(transition[4] != "e"){
                                stack.push(transition[4]);
                            }
                            outputfile << transition[0] << "," << transition[1] << "," << transition[2] << " => " << transition[3] << "," << transition[4];
                            string stackString = stack.print();
                            outputfile << stackString;
                            break;
                        }
                    }
                }
            }
            // looking for if in acceptance state, if not then looking if there is any rule that moves us to final state
            bool acceptance = false;
            for(int e = 0 ; e<endStates.size() ; e++){
                if(currentState == endStates[e]){
                    if(stack.isEmpty() || (stack.size() == 1 && stack.peek() == "$")){
                        outputfile << "ACCEPT\n\n";
                        acceptance = true;
                    }
                }
            }
            while(!acceptance){ // looking for a rule where we can move to final state
                for(int j = 0 ; j<transitions.size() ; j++){
                    vector<string> transition = split(transitions[j],",");
                    if(currentState == transition[0] && transition[1] == "e"){
                        if(stack.isEmpty()){
                            currentState = transition[3];
                            if(transition[4] != "e"){
                                stack.push(transition[4]);
                            }
                            outputfile << transition[0] << "," << transition[1] << "," << transition[2] << " => " << transition[3] << "," << transition[4];
                            string stackString = stack.print();
                            outputfile << stackString;
                        }else if (stack.peek() == transition[2] || (stack.peek() == "e" || transition[2] == "e")){
                            if(stack.peek() == transition[2]){
                                stack.pop();
                            }
                            currentState = transition[3];
                            if(transition[4] != "e"){
                                stack.push(transition[4]);
                            }
                            outputfile << transition[0] << "," << transition[1] << "," << transition[2] << " => " << transition[3] << "," << transition[4];
                            string stackString = stack.print();
                            outputfile << stackString;
                            if(transition[1] != "e"){
                                break;
                            }
                        }
                    }
                    if(stack.isEmpty() || (stack.size() == 1 && stack.peek() == "$")){
                        for(int h = 0 ; h<endStates.size() ; h++){
                            if(currentState == endStates[h]){
                                if(stack.isEmpty() || (stack.size() == 1 && stack.peek() == "$")){
                                    outputfile << "ACCEPT\n\n";
                                    acceptance = true;
                                    break;
                                }
                            }
                        }
                    }
                }
                if(!acceptance){
                    outputfile << "REJECT\n\n";
                }
                acceptance = true;
            }
        }
    }
    outputfile.close();
}
void readFiles(string filename, string filename2 ,string outputFile){
    vector<string> setOfStates;
    vector<string> inputAlphabet;
    vector<string> stackAlphabet;
    vector<string> transitions;
    vector<string> endStates;
    string startState;
    ifstream file(filename);
    if(!file) {
        cout << "Cannot open input file.\n";
        exit(1);
    }
    string str;
    while (getline(file, str)) { //reading first input file
        vector<string> main = split(str, ":");
        if(main[0] == "Q"){
            vector<string> input = split(main[1], "=>");
            vector<string> alphabet = split(input[0], ",");
            for (auto i : alphabet){ // pushing to alphabet vector
                rtrim(i);
                setOfStates.push_back(i);
            }
            string states = input[1];
            unsigned first = states.find('(');
            unsigned last = states.find(')');
            startState = states.substr (first+1,last-first-1);
            unsigned firstEnd = states.find('[');
            unsigned lastEnd = states.find_last_of(']');
            string endState = states.substr (firstEnd,lastEnd);
            vector<string> tempEndStates = split(endState, ",");
            for (auto i : tempEndStates){ // pushing to alphabet vector
                unsigned firstPart = i.find('[');
                unsigned lastPart = i.find(']');
                string end = i.substr (firstPart+1,lastPart-1);
                endStates.push_back(end);
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
            transitions.push_back(main[1]);
        }
    }
    ifstream inputFile(filename2);
    if(!inputFile) {
        cout << "Cannot open input file.\n";
        exit(1);
    }
    string inputLine;
    while (getline(inputFile, inputLine)) {
        processInput(inputLine, setOfStates , inputAlphabet , stackAlphabet , transitions , endStates,startState,outputFile);

    }
    inputFile.close();

    file.close();
}

int main(int argc, char **argv) {
    remove(argv[3]); // since i am appending to file, i am removing the output file here to get rid of writing every output to same file.
    readFiles(argv[1],argv[2],argv[3]);

    return 0;
}