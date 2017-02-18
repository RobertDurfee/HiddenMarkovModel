# HiddenMarkovModel
C++ class for hidden Markov models.

### Disclaimer
This is not production-worthy code! View this simply as a proof-of-concept.

### Initialization
```C++
HiddenMarkovModel(vector<string> states, vector<string> observations);
HiddenMarkovModel(char ** states, int nStates, char ** observations, int nObservations);
```

### Viterbi
```C++
vector<string> Viterbi(vector<string> observationSequence);
char ** Viterbi(char ** observationSequence, int nObservationSequence);
```

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
