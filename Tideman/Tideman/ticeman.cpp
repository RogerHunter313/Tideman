#include <iostream>
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

int pair_count;
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

	return 0;
}




