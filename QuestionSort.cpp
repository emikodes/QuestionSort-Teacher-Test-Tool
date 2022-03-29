#include "main.h"
#include "utilities.h"

using namespace std;

int main(void) {
	srand(time(NULL));
	setlocale(LC_ALL, "it_IT.utf8");

	const string emptyLine = "__________________________________________";
	const char token = '|';

	string fileNameTest = "Test";
	string firstLine, secondLine,header="",dummystr;

	size_t pos = 0;
	int numberOfTestsToWrite = 0;

	vector<pair<string, vector<string>>> singleQuestions; //first element is the question, second element is an array containing the answers.

	fstream MasterKey("MasterKey.qsrt", ios::in); //file with form
	fstream Answers("ForTeachers.qsrt", ios::out); //file with answers,for teacher to correct.
	fstream Tests; //file with answers,for teacher to correct.

	Tests.open("TEMPLATE.html",ios::in);
		while(!Tests.fail()&&getline(Tests,dummystr)){
			header += dummystr;
		}

		if(header!=""){
			Tests.close();
		}



	if (!MasterKey.fail() && !Answers.fail() && header!=""){

		//input test name
		cout << "Name of the test? ";
		getline(cin,dummystr);

		//change test name in header
		while((pos=header.find('*',++pos))!=string::npos){
			header.insert(++pos,dummystr+"*");
			pos+=dummystr.length();
		}

		while (getline(MasterKey, firstLine) && getline(MasterKey, secondLine)) { //read two lines from file
			singleQuestions.push_back(make_pair(firstLine, parser(secondLine, token))); //filling singleQuestions with couple of question - answer(s)
		}

		do{
			cout << "How many tests do you want to prepare? ";
			cin >> numberOfTestsToWrite;
		}while (cin.fail());

		for (int i = 0; i < numberOfTestsToWrite ; i++) {
			//open file
			Tests.open((fileNameTest + to_string(i) + ".html"), ios::out);

			Tests << header; //inserting header in each test

			////////SHUFFLER/////////
			random_shuffle(singleQuestions.begin(), singleQuestions.end(), [](int i) -> int { return rand() % i; }); //shuffling questions

			for (int i = 0; i < singleQuestions.size(); i++) {
				for (int j = 0; j < singleQuestions[i].second.size(); j++) {
					random_shuffle(singleQuestions[i].second.begin(), singleQuestions[i].second.end(), [](int i) -> int { return rand() % i; }); //shuffling answers
				}
			}
			/////////////////////////

			for (int INDEX = 0; INDEX < singleQuestions.size(); INDEX++) {
				Tests << "<br>" << singleQuestions[INDEX].first << "<br>"; //printing question to screen
				Answers << "Risposta corretta per la domanda: \"" << singleQuestions[INDEX].first << "\" -> "; //print answer to file for teacher.

				if (singleQuestions[INDEX].second.size() > 1) { //if vector with answers contains more than a question,this means it's a multiple choice question, so i have to print all of them
					for (auto answer : singleQuestions[INDEX].second) {
						if ((pos = answer.find('#')) != string::npos) { //correct answer
							answer.replace(pos, 1, ""); //i remove '#' from string, to show it on the file (otherwise, students would have known which is the correct answer.
							Answers << answer << endl;
						}
						Tests << "[] " << answer << "<br>"; //printing answer(s).
					}
				}
				else {
					//otherwise, vector with answer contains only one correct answer, that has to be written by the student.
					Answers << singleQuestions[INDEX].second[0] << endl;
					Tests << emptyLine << endl;//open questions.
				}
			}

		Tests.close();
		}
		MasterKey.close();
		Answers.close();
	}
	else {
		cout << "An Error occurred while opening the files...";
	}

	return 0;
}
