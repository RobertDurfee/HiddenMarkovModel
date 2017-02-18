# HiddenMarkovModel
C++ class for hidden Markov models.

### Disclaimer
This is not production-worthy code! View this simply as a proof-of-concept.

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
After the `BaumWelch` method is called, the matrices may no be 'normalized' in the sense that all columns add up to 100%. This method can be called to garuntee that the columns add up to 100%. The `command` can be any combination of the three constants `HIDDEN_MARKOV_MODEL_INITIAL_MATRIX`, `HIDDEN_MARKOV_MODEL_EMISSION_MATRIX`, and `HIDDEN_MARKOV_MODEL_TRANSITION_MATRIX`. Do normalize more than one matrix at a time, perform a bitwise OR on the constants.

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
```
