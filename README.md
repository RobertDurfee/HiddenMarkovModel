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
This method takes a sequence of observations and outputs the most likely sequence of hidden states. If the pointer to a pointer of characters implementation is used, the output must be freed by iterating through each outputted state and calling `delete[]` and then calling `delete[]` for the output. For more information on the Viterbi algorithm, read the [Wikipedia page](https://en.wikipedia.org/wiki/Viterbi_algorithm).

### Forward
```C++
double Forward(vector<string> observationSequence);
double Forward(char ** observationSequence, int nObservationSequence);

double Forward(vector<string> observationSequence, Matrix2D * Alpha);
double Forward(char ** observationSequence, int nObservationSequence, Matrix2D * Alpha);
```

### Backward
```C++
double Backward(vector<string> observationSequence);
double Backward(char ** observationSequence, int nObservationSequence);
  
double Backward(vector<string> observationSequence, Matrix2D * Beta);
double Backward(char ** observationSequence, int nObservationSequence, Matrix2D * Beta);
```

### Posterior
```C++
vector<string> Posterior(vector<string> observationSequence);
char ** Posterior(char ** observationSequence, int nObservationSequence);
```

### Baum-Welch
```C++
void BaumWelch(vector<string> observationSequence);
void BaumWelch(char ** observationSequence, int nObservationSequence);
```

### Normalize
```C++
void Normalize(int command);
```
A deconstructor is only needed for the pointer to a pointer of characters implementation as the vector of strings implementation uses RAII. In this destructor, all allocated memory used by the `HiddenMarkovModel` is freed.

### Matrices
```C++
Matrix1D Initial;
Matrix2D Emission;
Matrix2D Transition;
```

### Deinitialization
```C++
~HiddenMarkovModel();
```

### Example
```C++
```
