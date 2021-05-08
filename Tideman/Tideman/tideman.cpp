#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Max number of candidates, don't use ; here, causes errors
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pairing has a winner, loser
// pairing is user defined data type with two fields of winner or loser, changed from pair to avoid namespace ambiguity
// the integer represents the candidate index in the candidates array

typedef struct {
	int winner;
	int loser;
} pairing;

// Array of candidates
string candidates[MAX];
// https://math.stackexchange.com/questions/2214839/exactly-how-does-the-equation-nn-1-2-determine-the-number-of-pairs-of-a-given
// we have 9 candidates, each candidate is paired with the other 8 candidates, this counts each pair twice so divide by 2
pairing pairs[MAX * (MAX - 1) / 2];

int pairCount;
int numCandidates;

// Function prototypes
bool vote(int rank, string name, vector<int> ranks);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

void merge_sort(int i, int j, pairing a[], pairing aux[]);
bool recursive_lock(int a, int b);

int main() {
	// Check for invalid usage

	cout << "Number of candidates : " << flush;
	cin >> numCandidates;
	cout << endl;

	while(numCandidates < 2) {
		cout << "Must enter more than 1 candidate." << endl;
		cout << "Please enter total number of candidates : " << flush;
		cin >> numCandidates;
		cout << endl;
	}

	while (numCandidates > MAX) {
		cout << "Maximum number of candidates is " << MAX << endl;
		cout << "Please enter number of candidates or 0 to exit : " << flush;
		cin >> numCandidates;
		if (numCandidates == 0) {
			cout << "exiting" << endl;
			return 1;
		}
		cout <<  endl;
	}

	cout << "You entered there are " << numCandidates << " candidates" << endl;

	string name;
	
	for (int i = 0; i < numCandidates; i++) {
		cout << "Name for candidate " << i + 1 << ": " << flush;
		cin >> name;
		cout << "Candidate " << i + 1 << " is " << name << endl;
		candidates[i] = name;
	}
	
	// Clear graph of locked in pairs
	for (int i = 0; i < numCandidates; i++) {
		for (int j = 0; j < numCandidates; j++) {
			locked[i][j] = false;
		}
	}

	// initialize number of pairs and get number of voters from user
	pairCount = 0;
	int voterCount;
	cout << endl;
	cout << "Number of voters : " << flush;
	cin >> voterCount;

	// Query for votes
	for (int i = 0; i < voterCount; i++) {

		// ranks[i] is voter's ith preference
		// size of vector set to numCandidates, initialized to 0
		vector<int> ranks(numCandidates, 0);

		cout << "Enter ranks for voter " << i + 1 << endl;

		// Query for each rank
		for (int j = 0; j < numCandidates; j++) {
			string name;
			cout << "Rank " << j + 1 << ": " << flush;
			cin >> name;

	/*		if (!vote(j, name, ranks)) {
				cout << "Invalid vote" << endl;
				return 2;
			}*/
		}
		cout << endl;
	}


	return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, vector<int> ranks) {
	//for (int i = 0; i < numCandidates; i++) {
	//	if ()
	//}
	return true;
}



