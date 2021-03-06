# HiddenMarkovModel
C++ class for hidden Markov models.

### Disclaimer
This is not production-worthy code! View this simply as a proof-of-concept. Preconditions are implicit. No error checking exists.

### Initialization
```C++
HiddenMarkovModel(vector<string> states, vector<string> observations);
HiddenMarkovModel(char ** states, int nStates, char ** observations, int nObservations);
```
These constructors initialize all the possible states and observations of a `HiddenMarkovModel`. Two headers are provided utilizing a vector of strings or a pointer to a pointer of charaters as labels to facilitate different applications and avoid converting types.

### Viterbi
```C++
vector<string> Viterbi(vector<string> observationSequence);
char ** Viterbi(char ** observationSequence, int nObservationSequence);
```
This method takes a sequence of observations and outputs the most likely sequence of hidden states. If the pointer to a pointer of characters implementation is used, the output must be deallocated by iterating through each outputted state and calling `delete[]` and then calling `delete[]` for the output to avoid memory leaks. For more information on the Viterbi algorithm, read the [Wikipedia page](https://en.wikipedia.org/wiki/Viterbi_algorithm).

### Forward
```C++
double Forward(vector<string> observationSequence);
double Forward(char ** observationSequence, int nObservationSequence);

double Forward(vector<string> observationSequence, Matrix2D * Alpha);
double Forward(char ** observationSequence, int nObservationSequence, Matrix2D * Alpha);
```
This method generates the `Alpha` matrix required by the `Posterior` and `BaumWelch` methods. For the methods with `Alpha` as a parameter, the `Matrix` must be allocated already. This is the first half of of the Forward-Backward algorithm. For more information on the Forward-Backward algorithm, read the [Wikipedia page](https://en.wikipedia.org/wiki/Forward%E2%80%93backward_algorithm).

### Backward
```C++
double Backward(vector<string> observationSequence);
double Backward(char ** observationSequence, int nObservationSequence);
  
double Backward(vector<string> observationSequence, Matrix2D * Beta);
double Backward(char ** observationSequence, int nObservationSequence, Matrix2D * Beta);
```
This method generates the `Beta` matrix required by the `Posterior` and `BaumWelch` methods. For the methods with `Beta` as a parameter, the `Matrix` must be allocated already. This is the second half of of the Forward-Backward algorithm. For more information on the Forward-Backward algorithm, read the [Wikipedia page](https://en.wikipedia.org/wiki/Forward%E2%80%93backward_algorithm).

### Posterior
```C++
vector<string> Posterior(vector<string> observationSequence);
char ** Posterior(char ** observationSequence, int nObservationSequence);
```
This method takes a sequence of observations and outpus the most likely hidden state for each observation. If the pointer to a pointer of characters implementation is used, the output must be deallocated by iterating through each outputted state and calling `delete[]` and then calling `delete[]` for the output to avoid memory leaks. This combines both halves of the Forward-Backward algorithm. For more information on the Forward-Backward algorithm, read the [Wikipedia page](https://en.wikipedia.org/wiki/Forward%E2%80%93backward_algorithm).

### Baum-Welch
```C++
void BaumWelch(vector<string> observationSequence);
void BaumWelch(char ** observationSequence, int nObservationSequence);
```
This method takes a sequence of observations and adjusts the initial, emission, and transition matrices to better reflect the observation sequence. For more information about the Baum-Welck algorithm, read the [Wikipedia page](https://en.wikipedia.org/wiki/Baum%E2%80%93Welch_algorithm).

### Normalize
```C++
void Normalize(int command);
```
After the `BaumWelch` method is called, the matrices may no be 'normalized' in the sense that all columns add up to 100%. This method can be called to garuntee that the columns add up to 100%. The `command` can be any combination of the three constants `HIDDEN_MARKOV_MODEL_INITIAL_MATRIX`, `HIDDEN_MARKOV_MODEL_EMISSION_MATRIX`, and `HIDDEN_MARKOV_MODEL_TRANSITION_MATRIX`. To normalize more than one matrix at a time, perform a bitwise `OR` on the constants.

### Matrices
```C++
Matrix1D Initial;
Matrix2D Emission;
Matrix2D Transition;
```
These matrices hold the probabilities of the initial states, connections of states to observations, and connections between two different states. They can be accessed by the particular element's state and/or observation label for ease.

### Deinitialization
```C++
~HiddenMarkovModel();
```
A deconstructor is only needed for the pointer to a pointer of characters implementation as the vector of strings implementation uses RAII. In this destructor, all allocated memory used by the `HiddenMarkovModel` is freed.

### Example
```C++
#include "HiddenMarkovModel-StringVector.h"

int main()
{
	vector<string> States = { "Healthy", "Fever" };
	vector<string> Observations = { "Dizzy", "Cold", "Normal" };

	HiddenMarkovModel illnessDiagnosis(States, Observations);

	illnessDiagnosis.Initial["Healthy"] = 0.6;
	illnessDiagnosis.Initial["Fever"] = 0.4;

	illnessDiagnosis.Transition["Healthy"]["Healthy"] = 0.7;
	illnessDiagnosis.Transition["Healthy"]["Fever"] = 0.3;
	illnessDiagnosis.Transition["Fever"]["Healthy"] = 0.4;
	illnessDiagnosis.Transition["Fever"]["Fever"] = 0.6;

	illnessDiagnosis.Emission["Healthy"]["Normal"] = 0.5;
	illnessDiagnosis.Emission["Healthy"]["Cold"] = 0.4;
	illnessDiagnosis.Emission["Healthy"]["Dizzy"] = 0.1;
	illnessDiagnosis.Emission["Fever"]["Normal"] = 0.1;
	illnessDiagnosis.Emission["Fever"]["Cold"] = 0.3;
	illnessDiagnosis.Emission["Fever"]["Dizzy"] = 0.6;

	vector<string> ObservationSequence = { "Normal", "Cold", "Dizzy" };

	vector<string> StateSequence = illnessDiagnosis.Viterbi(ObservationSequence);

	return 0;
}
```
This example is copied from Wikipedia's page on the [Viterbi algorithm](https://en.wikipedia.org/wiki/Viterbi_algorithm). The states represent a person's diagnosis: healthy or fever. Their symptoms can be normal, dizzy, or cold. Using the probabilities given by Wikipedia, the initial, emission, and transision matrices are initialized. The observation sequence represents how the person felt on three different days. Using the Viterbi algorithm, the most probable sequence of states, or the diagnosis for the day, is generated.
