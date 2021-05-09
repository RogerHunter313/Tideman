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
bool vote(int rank, string name, int ranks[]);
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

	while (numCandidates < 2) {
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
		cout << endl;
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

	// allocate an array, ranks, set to the number of candidates. this is dynamically created
	int *ranks = new int[numCandidates];

	// Query for votes
	// i is the current voter being recorded
	for (int i = 0; i < voterCount; i++) {

		cout << "Enter ranks for voter " << i + 1 << endl;

		// Query for each rank, j is current rank being recorded
		for (int j = 0; j < numCandidates; j++) {
			string name;
			cout << "Rank " << j + 1 << ": " << flush;
			cin >> name;

			if (!vote(j, name, ranks)) {
				cout << "Invalid vote" << endl;
				return 2;
			}	
		}
		
		record_preferences(ranks);
		//cout << ranks[0] << " " << ranks[1] << " " << ranks[2] << endl;
		cout << endl;
	}
		
	add_pairs();
	sort_pairs();
	lock_pairs();
	print_winner();
	

	////print locked
	//for (int i = 0; i < numCandidates; i++) {
	//	for (int j = 0; j < numCandidates; j++) {
	//		cout << boolalpha << locked[i][j] << endl;
	//	}
	//}

	delete[] ranks;
	return 0;
}

// Update ranks given a new vote
// rank is the current rank being recorded, name in the inputted name, ranks are being stored for one voter
bool vote(int rank, string name, int ranks[]) {
	for (int i = 0; i < numCandidates; i++) {
		// check if entered name matches any names in the array of candidates
		// the value of the candidate in the candidate list is stored in ranks at the current rank that was inputted
		if (name.compare(candidates[i]) == 0) {
			ranks[rank] = i;
			return true;
		}
	}
	return false;
}

// update preferences given one voter's ranks
void record_preferences(int ranks[]) {
	for (int i = 0; i < numCandidates; i++) {
		for (int j = i + 1; j < numCandidates; j++)
		// the ranks for one voter was stored in vote, now the two dimensional array is populated tally the number of times
		// one candidate out ranked another candidate, i is the first candidate, j is the second
		// because ranks for each voter is populated by order, the first entry is preferred over all the other candidates
		// one time, so you keeping adding ++ according to how many other candidates are to the right of current candidate value
		// in ranks.
		// preferences is just a tally of how many total other candidates the current candidate had a higher ranking
		preferences[ranks[i]][ranks[j]] ++;
	}
}

// Record pairs of candidates where on is preferred over the other
// pairs is a struct with two fields, winner and loser, that are populated with the corresponding candidate index
// a pair is only populated in the case where one pair of candiates gets more votes than the reverse pairing
// add pairs is not adding arithmetaclly, but just placing an additional pair in the pairs array
void add_pairs(void) {
	for (int i = 0; i < numCandidates; i++) {
		for (int j = 0; j < numCandidates; j++) {
			if (preferences[i][j] > preferences[j][i]) {
				pairs[pairCount].winner = i;
				pairs[pairCount].loser = j;
				pairCount++;
			}
			else if (preferences[i][j] < preferences[j][i]) {
				pairs[pairCount].winner = j;
				pairs[pairCount].loser = i;
				pairCount++;
			}
		}
	}
	return;
}

// Sort pairs in decreasing order by strength of victory
// i just used a bubble sort to put the strongest pair first, strength of pair is determined by preferences 2d array
// bubble sort isn't fast, but we only have a maximum of 9 candidates with a maximum of 36 pairings
// keep in mind the pairs struct isn't populated with equal pairings
void sort_pairs(void) {
	for (int i = 0; i < pairCount; i++) {
		for (int j = i + 1; j < pairCount; j++) {
			if (preferences[pairs[i].winner][pairs[i].loser] < preferences[pairs[j].winner][pairs[j].loser]) {
				pairing temp = pairs[i];
				pairs[i] = pairs[j];
				pairs[j] = temp;
			}
		}
	}
	return;
}

// Test for cycle by checking arrow coming into each candidate
bool is_cycle(int loser, int winner) {
	// Return true if there is a cycle created (Recursion base case)
	// candidate eventually compared to itself in recursive case
	if (loser == winner) {
		return true;
	}

	// Loop through candidates (Recursive case)
	for (int i = 0; i < numCandidates; i++) {
		if (locked[loser][i]) {  //check if loser is locked with a candidate
			if (is_cycle(i, winner)) { //check if that candidate is locked with winner // checking if the loser eventually becomes a winner when iterated through candidates
				return true;
			}
		}
	}
	return false;
}

// update the locked array for all the pairs that should get locked.
// pairs are locked in when they don't create a cycle.  only pairs that don't create a cycle are locked in
// we are adding edges until an edge creates a cycle, we ignore edges that create cycles and move on to the next edge
void lock_pairs(void) {
	for (int i = 0; i < pairCount; i++) {
		if (!is_cycle(pairs[i].loser, pairs[i].winner)) { //prevents placing a true where it cycles back to a loser
			locked[pairs[i].winner][pairs[i].loser] = true;
			cout << "true recorded" << endl;
		}
	}
	return;
}

// Print the winner of the election
void print_winner(void) {
	// Winner is the candidate with no arrows pointing to them
	for (int i = 0; i < numCandidates; i++) {
		int false_count = 0;

		for (int j = 0; j < numCandidates; j++) {
			if (locked[j][i] == false) {
				false_count++;
				if (false_count == numCandidates) {
					cout << candidates[i] << endl;
				}
			}
		}
	}
	return;
}