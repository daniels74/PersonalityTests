// TODO(dflore): driver.h for Personality Quiz
#include <math.h>
#include <limits>
#include <string>
#include <map>
#include <set>
#include "myrandom.h"
#include <exception>
#include <stdexcept>

using namespace std;

constexpr double lowest_double = std::numeric_limits<double>::lowest();

/* Type: Question
 *
 * Type representing a personality quiz question.
 */
struct Question {
    string questionText;  // Text of the question
    map<char, int> factors;   // Map from factors to +1 or -1
    friend bool operator< (const Question& lhs, const Question& rhs) {
        return lhs.questionText < rhs.questionText;
    }
    friend bool operator == (const Question& lhs, const Question& rhs) {
        return lhs.questionText == rhs.questionText;
    }
    friend bool operator != (const Question& lhs, const Question& rhs) {
        return lhs.questionText != rhs.questionText;
    }
};

/* Type: Person
 *
 * Type representing a person, used to represent people when determining
 * who's the closest match to the user.
 */
struct Person {
    string name;      // Name of the person
    map<char, int> scores;  // Map from factors to +1 or -1
    friend bool operator< (const Person& lhs,   const Person& rhs) {
        return lhs.name < rhs.name;
    }
    friend bool operator== (const Person& lhs, const Person& rhs) {
        return lhs.name == rhs.name;
    }
    friend bool operator!= (const Person& lhs, const Person& rhs) {
        return lhs.name != rhs.name;
    }
};

/* randomElement
 *
 * This function selects, at random, a Question from the inputted questions set
 * and returns the question.  Note, this function does not remove the randomly
 * selected question from the set.
*/
Question randomElement(set<Question>& questions) {
    int ind = randomInteger(0, (int)questions.size()-1);
    int i = 0;
    for (auto e : questions) {
        if (i == ind) {
            return e;
        }
        i++;
    }
    return {};
}

/*randomQuestionFrom takes a set of type Question and returns
a random element from it. The element returned is deleted 
from the set*/
Question randomQuestionFrom(set<Question>& questions) {
      if (questions.empty()) {
      throw runtime_error("Question Set: empty");
      }

      Question theRandom = randomElement(questions);  // select random
      Question theOutQuestion = theRandom;
      questions.erase(theRandom);
        return theOutQuestion;
}

// will check if letter is in the map
// if true will access its content and update it by deleting
// the current map and making a new one with the new score
// I will add current score with the one just recieved for
// the current letter(key)
void factorScoreUpdate(char letter, int score, map<char, int>& newMap) {
  newMap[letter] = newMap[letter] + score;  // we just update th esecond value
}
/*scoresFrom takes a map with a Question type as keys and 
int values as data. This corresponds to the users question and 
answer. This uses the info from each Question struct to come up 
with a score based on the users answer to that question. The letter and score of that letter will be saved in a seperate map 
which is to be returned*/
map<char, int> scoresFrom(map<Question, int>& answers) {
  map<char, int> newMap;
  for (auto& m : answers) {
    Question theQuestionAndFactors = m.first;  // Passes a new question struct
    int theAnswer = m.second;  // gives the answer
    for (auto& f : theQuestionAndFactors.factors) {
      char letter = f.first;
      int scoreRatingStyle = f.second;
      int score = 0;

      if (theAnswer == 1) {   // -2
        score = scoreRatingStyle * (-2);
        factorScoreUpdate(letter, score, newMap);
        }
      if (theAnswer == 2) {   // -1
        score = scoreRatingStyle * (-1);
        factorScoreUpdate(letter, score, newMap);
        }
      if (theAnswer == 3) {   // 0
        score = scoreRatingStyle * (0);
        factorScoreUpdate(letter, score, newMap);
        }
      if (theAnswer == 4) {  // +1
        score = scoreRatingStyle * (1);
        factorScoreUpdate(letter, score, newMap);
        }
      if (theAnswer == 5) {  // +2
        score = scoreRatingStyle * (2);
        factorScoreUpdate(letter, score, newMap);
        }
      }
    }
    return newMap;
}

map<char, double> normalize(map<char, int>& scores) {
    vector<double> nums;
    double normalized;
    double sqrtoftotal;
    map<char, double> normalizedMap;

    double  mult = 0;
    for (auto& m : scores) {  // stores numbers to be added
       mult += m.second * m.second;
    }
    sqrtoftotal = sqrt(mult);
    if (sqrtoftotal == 0) {
        throw runtime_error("Score Map: empty");
    }
    for (auto& ms : scores) {               // Score dividing and storing
      normalized = ms.second / sqrtoftotal;
      normalizedMap.emplace(ms.first, normalized);
    }
    return normalizedMap;
}

double cosineSimilarityOf(const map<char, double>& lhs,
                          const map<char, double>& rhs) {
    char letter;
    double number;
    double sim;

    for (auto& a : lhs) {  // for each letter in 1st map
      letter = a.first;
      number = a.second;
      auto itter = rhs.find(letter);  // search for it in map 2
      if (itter == rhs.end()) {           // if not found
        sim += 0;
      }
      else {
        double d = itter->second;
       sim += number * d;
      }
    }
  return sim;
}
Person findBest(map<Person, double>& similarities) {
  double num = -1;  // similarities.begin()->second;
  Person newBest;
  for (auto& a : similarities) {
    if (a.second > num) {
    newBest = a.first;
    num = a.second;
    }
  }
  return newBest;
}

void deleteMembersFromPerson(map<char, int>& lhs,
 map<char, int>& rhs) {
  char letter;
  for (auto& a : lhs) {  // for each letter in 1st map
    letter = a.first;
    auto itter = rhs.find(letter);  // search for it in map 2
    if (itter == rhs.end()) {           // if not found
      rhs.erase(a.first);
    }
  }
  return;
}
/*mostSimilarTo will first normalize the user's scores and then
run through the map of poeple whos type is of "Person"
and it will be taking each persons scores and normalizing them. For each
person in the map it will compare their normalized scores with 
users normalized scores. If the new result is higher than the last, it
will save it as the best match. WHen done going through people
it will return the Person struct that best matched*/
Person mostSimilarTo(map<char, int>& scores, set<Person>& people) {
    Person bestMatch;
    map<char, int> unNormalized;
    map<char, double> normalizedScores;
    Person secondBest;
    map<Person, double> resultsMap;

    if (people.empty()) {
        throw runtime_error("People Set: empty");
      }
    map<char, double> indiv = normalize(scores);
    for (auto& g : people) {
      unNormalized = g.scores;
      normalizedScores = normalize(unNormalized);
      double newResults = cosineSimilarityOf(indiv, normalizedScores);
      resultsMap.emplace(g, newResults);
    }
    bestMatch = findBest(resultsMap);
    return bestMatch;
}