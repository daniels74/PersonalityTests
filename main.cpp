// TODO(dflore36): Personality Quiz 
#include <set>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "driver.h"
#include <cstdlib>

using namespace std;

int concat(int a, int b) {
    string s1 = to_string(a);
    string s2 = to_string(b);
    string s = s1 + s2;
    int c = stoi(s);
    return c;
}
void saveFactors(string& singlePair, map<char, int>& factorMap) {
  char letter;
  vector<int> scoreVec;
  int sign = 0;
  int newScore;

  for (char& c : singlePair) {
    if ((c >='a' && c <='z') || (c >='A' && c <='Z')) {
      letter = c;
    }
    if (c == '-') {   // if value is negative
      sign = -1;
    }                      // If digit
    if (isdigit(c)) {
      stringstream stream;
      stream << c;
      int b;
      stream >> b;
      scoreVec.push_back(b);
    }
  }

  if (scoreVec.size() > 1) {
  int begin = scoreVec.at(0);
  for (unsigned j = 1 ; j < scoreVec.size(); j++) {
    begin = concat(begin, scoreVec[j]);
  }
  newScore = begin;
  }
  else newScore = scoreVec[0]; 
  
  if (sign == -1) {
    newScore = newScore * -1;
  }
  factorMap.emplace(letter, newScore);
}
map<char, int> getFactors(string factors) {
  map<char, int> factorMap;
  stringstream stream(factors);
  string singlePair;
  while (stream >> singlePair) {   // Pass one factor pair at a time
    saveFactors(singlePair, factorMap);  // Save the pair in the map
  }
return factorMap;
}

void readFile(string fileName, set<Question>& theQuestions,
 map<char, int>& factorScoreMap, set<Person>& thePeople, int fileType) {
  ifstream inFS;
  inFS.open(fileName); 
  if (!inFS.is_open()) {
    throw runtime_error("File cant be accessed");
  }
  string line;
  getline(inFS, line);
  while (!inFS.eof()) {
    stringstream parser(line);
    string questionName;
    string factorsScore;
    getline(parser, questionName, '.');  // obtains qustion or Name
    getline(parser, factorsScore);        // Obatains factors or scores
    if (fileType == 1) {
      Question newQuestionStruct;
      newQuestionStruct.questionText = questionName;
      factorScoreMap = getFactors(factorsScore);
      newQuestionStruct.factors = factorScoreMap;
      theQuestions.emplace(newQuestionStruct);
    }
    if (fileType == 2) {
      Person newPersonStruct;
      newPersonStruct.name = questionName;
      factorScoreMap = getFactors(factorsScore);
      newPersonStruct.scores = factorScoreMap;
      thePeople.emplace(newPersonStruct);
    }
    getline(inFS, line);
  }
}

int main() {
    set<Question> theQuestions;
    set<Person> blank;
    map<char, int> usersFactorScores;
    Person theChosenOne;
    map<char, int> FactorsScoresMap;
    map<Question, int> indivualsScores;
    bool over = false;
    int numberQuestions;
    int userAnswer;
    int userTestNumber;
    Question questionMember;
    set<Person> thePeople;
    Person theBEST;
    readFile("questions.txt", theQuestions, FactorsScoresMap, blank, 1);
    cout << "Welcome to the Personality Quiz!" << endl << endl;
    cout << "Choose number of questions: ";
    cin >> numberQuestions;
    for (int i = 0; i < numberQuestions; i++) {
      cout << endl << "How much do you agree with this statement?" << endl;
      questionMember = randomQuestionFrom(theQuestions);
      cout << "\"" << questionMember.questionText << ".\"" << endl << endl;
      cout << "1. Strongly disagree" << endl;
      cout << "2. Disagree" << endl;
      cout << "3. Neutral" << endl;
      cout << "4. Agree" << endl;
      cout << "5. Strongly agree" << endl << endl;
      cout << "Enter your answer here (1-5): ";
      cin >> userAnswer;
      indivualsScores.emplace(questionMember, userAnswer);  
    }
    usersFactorScores = scoresFrom(indivualsScores);
    while (!over) {
      cout << "1. BabyAnimals" << endl;
      cout << "2. Brooklyn99" << endl;
      cout << "3. Disney" << endl;
      cout << "4. Hogwarts" << endl;
      cout << "5. MyersBriggs" << endl;
      cout << "6. SesameStreet" << endl;
      cout << "7. StarWars" << endl;
      cout << "8. Vegetables" << endl;
      cout << "9. mine" << endl;
      cout << "0. To end program." << endl << endl;
      cout << "Choose test number (1-9, or 0 to end): ";
      cin >> userTestNumber;
      if (userTestNumber == 1) {
        readFile("BabyAnimals.people", theQuestions,
         FactorsScoresMap, thePeople, 2);
        theChosenOne = mostSimilarTo(usersFactorScores, thePeople);
        Person theBEST = theChosenOne;
        cout << "You got " << theBEST.name << "!" << endl << endl;
        thePeople.clear();
      }
      if (userTestNumber == 2) {
        readFile("Brooklyn99.people", theQuestions,
         FactorsScoresMap, thePeople, 2);
        theChosenOne = mostSimilarTo(usersFactorScores, thePeople);
        cout << "You got " << theChosenOne.name << "!" << endl << endl;
        thePeople.clear();
      }
      if (userTestNumber == 3) {
        readFile("Disney.people", theQuestions,
         FactorsScoresMap, thePeople, 2);
        theChosenOne = mostSimilarTo(usersFactorScores, thePeople);
        Person theBEST = theChosenOne;
        cout << "You got " << theBEST.name << "!" << endl << endl;
        thePeople.clear();
      }
      if (userTestNumber == 4) {
        readFile("Hogwarts.people", theQuestions,
         FactorsScoresMap, thePeople, 2);
        theChosenOne = mostSimilarTo(usersFactorScores, thePeople);
        Person theBEST = theChosenOne;
        cout << "You got " << theBEST.name << "!" << endl << endl;
        thePeople.clear();
      }
      if (userTestNumber == 5) {
        readFile("MyersBriggs.people", theQuestions,
         FactorsScoresMap, thePeople, 2);
        theChosenOne = mostSimilarTo(usersFactorScores, thePeople);
        Person theBEST = theChosenOne;
        cout << "You got " << theBEST.name << "!" << endl << endl;
        thePeople.clear();
      }
      if (userTestNumber == 6) {
        readFile("SesameStreet.people", theQuestions,
         FactorsScoresMap, thePeople, 2);
        theChosenOne = mostSimilarTo(usersFactorScores, thePeople);
        Person theBEST = theChosenOne;
        cout << "You got " << theBEST.name << "!" << endl << endl;
        thePeople.clear();
      }
      if (userTestNumber == 7) {
        readFile("StarWars.people", theQuestions,
         FactorsScoresMap, thePeople, 2);
        theChosenOne = mostSimilarTo(usersFactorScores, thePeople);
        Person theBEST = theChosenOne;
        cout << "You got " << theBEST.name << "!" << endl << endl;
        thePeople.clear();
      }
      if (userTestNumber == 8) {
        readFile("Vegetables.people", theQuestions,
         FactorsScoresMap, thePeople, 2);
        theChosenOne = mostSimilarTo(usersFactorScores, thePeople);
        Person theBEST = theChosenOne;
        cout << "You got " << theBEST.name << "!" << endl << endl;
        thePeople.clear();
      }
      if (userTestNumber == 9) {
        readFile("mine.people", theQuestions,
         FactorsScoresMap, thePeople, 2);
        theChosenOne = mostSimilarTo(usersFactorScores, thePeople);
        Person theBEST = theChosenOne;
        cout << "You got " << theBEST.name << "!" << endl << endl;
        thePeople.clear();
      }
      if (userTestNumber == 0) {
        cout << "Goodbye!" << endl;
        over = true;
      }
    }
return 0;
cout << endl;
}
