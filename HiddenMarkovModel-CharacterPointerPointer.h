#ifndef HIDDEN_MARKOV_MODEL_CHARACTER_POINTER_POINTER_HEADER
#define HIDDEN_MARKOV_MODEL_CHARACTER_POINTER_POINTER_HEADER

#define _CRT_SECURE_NO_WARNINGS               //Necessary for use of strcpy()

#include "Matrices-CharacterPointerPointer.h" //Matrix1D, Matrix2D, MATRIX1D(), MATRIX2D()
#include <string.h>                           //strlen(), strcpy()

#define HIDDEN_MARKOV_MODEL_INITIAL_MATRIX 0x1
#define HIDDEN_MARKOV_MODEL_EMISSION_MATRIX 0x2
#define HIDDEN_MARKOV_MODEL_TRANSITION_MATRIX 0x4

using namespace std;

class HiddenMarkovModel
{
public:
	HiddenMarkovModel(char ** states, int nStates, char ** observations, int nObservations);

	~HiddenMarkovModel();

	char ** Viterbi(char ** observationSequence, int nObservationSequence);

	double Forward(char ** observationSequence, int nObservationSequence);
	double Forward(char ** observationSequence, int nObservationSequence, Matrix2D * Alpha);

	double Backward(char ** observationSequence, int nObservationSequence);
	double Backward(char ** observationSequence, int nObservationSequence, Matrix2D * Beta);

	char ** Posterior(char ** observationSequence, int nObservationSequence);

	void BaumWelch(char ** observationSequence, int nObservationSequence);

	void Normalize(int command);

	Matrix1D Initial;
	Matrix2D Emission;
	Matrix2D Transition;

private:
	char ** States;
	int nStates;
	char ** Observations;
	int nObservations;
};

HiddenMarkovModel::HiddenMarkovModel(char ** states, int nStates, char ** observations, int nObservations)
{
	States = new char*[nStates];
	for (int i = 0; i < nStates; i++)
	{
		States[i] = new char[strlen(states[i]) + 1];
		strcpy(States[i], states[i]);
	}
	this->nStates = nStates;

	Observations = new char*[nObservations];
	for (int i = 0; i < nObservations; i++)
	{
		Observations[i] = new char[strlen(observations[i]) + 1];
		strcpy(Observations[i], observations[i]);
	}
	this->nObservations = nObservations;

	Initial = MATRIX1D(nStates, States);
	Emission = MATRIX2D(nStates, nObservations, States, Observations);
	Transition = MATRIX2D(nStates, nStates, States, States);
}

HiddenMarkovModel::~HiddenMarkovModel()
{
	for (int i = 0; i < nStates; i++)
		delete[] States[i];
	delete[] States;

	for (int i = 0; i < nObservations; i++)
		delete[] Observations[i];
	delete[] Observations;
}

char ** HiddenMarkovModel::Viterbi(char ** observationSequence, int nObservationSequence)
{
	Matrix2D Delta = MATRIX2D(nObservationSequence, nStates, observationSequence, States);

	for (int k = 0; k < nObservationSequence; k++)
		for (int j = 0; j < nStates; j++)
			//Initialization
			if (k == 0)
				Delta[k][j] = Initial[j] * Emission[j][observationSequence[k]];
			//Recursion
			else
			{
				double max = 0;
				for (int i = 0; i < nStates; i++)
				{
					double possibleMax = Delta[k - 1][i] * Transition[i][j];
					if (possibleMax > max)
						max = possibleMax;
				}
				Delta[k][j] = max * Emission[j][observationSequence[k]];
			}
	//Termination
	double max = 0; char ** q = new char*[nObservationSequence]; memset(q, NULL, nObservationSequence * sizeof(char *));
	for (int i = 0; i < nStates; i++)
		if (Delta[nObservationSequence - 1][i] > max)
		{
			max = Delta[nObservationSequence - 1][i];
			delete[] q[nObservationSequence - 1];
			q[nObservationSequence - 1] = new char[strlen(States[i]) + 1];
			strcpy(q[nObservationSequence - 1], States[i]);
		}
	//Back-Tracking
	for (int k = nObservationSequence - 2; k >= 0; k--)
	{
		double max = 0;
		for (int i = 0; i < nStates; i++)
		{
			double possibleMax = Delta[k][i] * Transition[i][q[k + 1]];
			if (possibleMax > max)
			{
				max = possibleMax;
				delete[] q[k];
				q[k] = new char[strlen(States[i]) + 1];
				strcpy(q[k], States[i]);
			}
		}
	}

	return q;
}

double HiddenMarkovModel::Forward(char ** observationSequence, int nObservationSequence)
{
	Matrix2D Alpha;

	double result = Forward(observationSequence, nObservationSequence, &Alpha);
	
	return result;
}
double HiddenMarkovModel::Forward(char ** observationSequence, int nObservationSequence, Matrix2D * Alpha)
{
	*Alpha = MATRIX2D(nObservationSequence, nStates, observationSequence, States);

	for (int k = 0; k < nObservationSequence; k++)
		for (int j = 0; j < nStates; j++)
			//Initialization
			if (k == 0)
				(*Alpha)[k][j] = Initial[j] * Emission[j][observationSequence[k]];
			//Recursion
			else
			{
				double sum = 0;
				for (int i = 0; i < nStates; i++) sum += (*Alpha)[k - 1][i] * Transition[i][j];
				(*Alpha)[k][j] = sum * Emission[j][observationSequence[k]];
			}
	//Termination
	double sum = 0;
	for (int i = 0; i < nStates; i++) sum += (*Alpha)[nObservationSequence - 1][i];

	return sum;
}

double HiddenMarkovModel::Backward(char ** observationSequence, int nObservationSequence)
{
	Matrix2D Beta;

	double result = Backward(observationSequence, nObservationSequence, &Beta);
	
	return result;
}
double HiddenMarkovModel::Backward(char ** observationSequence, int nObservationSequence, Matrix2D * Beta)
{
	*Beta = MATRIX2D(nObservationSequence, nStates, observationSequence, States);

	for (int k = nObservationSequence - 1; k >= 0; k--)
		for (int i = 0; i < nStates; i++)
			//Initialization
			if (k == nObservationSequence - 1)
				(*Beta)[k][i] = 1;
			//Recursion
			else
			{
				double sum = 0;
				for (int j = 0; j < nStates; j++) sum += Transition[i][j] * Emission[j][observationSequence[k + 1]] * (*Beta)[k + 1][j];
				(*Beta)[k][i] = sum;
			}
	//Termination
	double sum = 0;
	for (int i = 0; i < nStates; i++) sum += Initial[i] * Emission[i][observationSequence[0]] * (*Beta)[0][i];

	return sum;
}

char ** HiddenMarkovModel::Posterior(char ** observationSequence, int nObservationSequence)
{
	char ** output = new char*[nObservationSequence];
	Matrix2D Alpha, Beta;

	//Initialization
	double probabilityOfObservationSequence = Forward(observationSequence, nObservationSequence, &Alpha);
	Backward(observationSequence, nObservationSequence, &Beta);
	//Recursion
	for (int k = 0; k < nObservationSequence; k++)
	{
		double max = 0; char * arg = NULL;
		for (int i = 0; i < nStates; i++)
		{
			double possibleMax = (Alpha[observationSequence[k]][i] * Beta[observationSequence[k]][i]) / probabilityOfObservationSequence;
			if (possibleMax > max)
			{
				max = possibleMax;
				delete[] arg;
				arg = new char[strlen(States[i]) + 1];
				strcpy(arg, States[i]);
			}
		}
		output[k] = new char[strlen(arg) + 1];
		strcpy(output[k], arg);
		delete[] arg;
	}
	
	return output;
}

void HiddenMarkovModel::BaumWelch(char ** observationSequence, int nObservationSequence)
{
	Matrix2D Alpha, Beta, Gamma = MATRIX2D(nObservationSequence, nStates, observationSequence, States); Matrix3D Xi = MATRIX3D(nObservationSequence, nStates, nStates, observationSequence, States, States); double probabilityOfObservationSequence;

	//Initialization
	probabilityOfObservationSequence = Forward(observationSequence, nObservationSequence, &Alpha);

	Backward(observationSequence, nObservationSequence, &Beta);

	for (int i = 0; i < nStates; i++)
		for (int j = 0; j < nStates; j++)
			for (int k = 0; k < nObservationSequence - 1; k++)
				Xi[k][i][j] = (Alpha[k][i] * Beta[k + 1][j] * Transition[i][j] * Emission[j][observationSequence[k + 1]]) / probabilityOfObservationSequence;

	for (int k = 0; k < nObservationSequence; k++)
		for (int i = 0; i < nStates; i++)
			Gamma[k][i] = (Alpha[k][i] * Beta[k][i]) / probabilityOfObservationSequence;
	
	//Estimate Initial
	double sum = 0;
	for (int i = 0; i < nStates; i++)
		Initial[i] = Gamma[0][i];

	//Estimate Transition
	for (int i = 0; i < nStates; i++)
		for (int j = 0; j < nStates; j++)
		{
			double sum1 = 0, sum2 = 0;
			for (int k = 0; k < nObservationSequence - 1; k++) sum1 += Xi[k][i][j];
			for (int k = 0; k < nObservationSequence - 1; k++) sum2 += Gamma[k][i];
			Transition[i][j] = sum1 / sum2;
		}

	//Estimate Emission
	for (int j = 0; j < nStates; j++)
		for (int k = 0; k < nObservations; k++)
		{
			double sum1 = 0, sum2 = 0;
			for (int l = 0; l < nObservationSequence; l++)
				if (observationSequence[l] == Observations[k])
					sum1 += Gamma[l][j];
			for (int l = 0; l < nObservationSequence; l++)
				sum2 += Gamma[l][j];
			Emission[j][k] = sum1 / sum2;
		}
}

void HiddenMarkovModel::Normalize(int command)
{
	if (command & HIDDEN_MARKOV_MODEL_INITIAL_MATRIX)
	{
		double sum = 0;
		for (int i = 0; i < nStates; i++)
			sum += Initial[States[i]];

		for (int i = 0; i < nStates; i++)
			Initial[States[i]] /= sum;
	}
	if (command & HIDDEN_MARKOV_MODEL_EMISSION_MATRIX)
		for (int i = 0; i < nStates; i++)
		{
			double sum = 0;
			for (int j = 0; j < nObservations; j++)
				sum += Emission[States[i]][Observations[j]];
			for (int j = 0; j < nObservations; j++)
				Emission[States[i]][Observations[j]] /= sum;
		}
	if (command & HIDDEN_MARKOV_MODEL_TRANSITION_MATRIX)
		for (int i = 0; i < nStates; i++)
		{
			double sum = 0;
			for (int j = 0; j < nStates; j++)
				sum += Transition[States[i]][States[j]];
			for (int j = 0; j < nStates; j++)
				Transition[States[i]][States[j]] /= sum;
		}
}

#endif
