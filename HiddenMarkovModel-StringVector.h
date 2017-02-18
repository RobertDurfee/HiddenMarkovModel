#ifndef HIDDEN_MARKOV_MODEL_STRING_VECTOR_HEADER
#define HIDDEN_MARKOV_MODEL_STRING_VECTOR_HEADER

#include "Matrices-StringVector.h" //Matrix1D, Matrix2D, MATRIX1D(), MATRIX2D()
#include <string>     //string
#include <vector>     //vector

#define HIDDEN_MARKOV_MODEL_INITIAL_MATRIX 0x1
#define HIDDEN_MARKOV_MODEL_EMISSION_MATRIX 0x2
#define HIDDEN_MARKOV_MODEL_TRANSITION_MATRIX 0x4

using namespace std;

class HiddenMarkovModel
{
public:
	HiddenMarkovModel(vector<string> states, vector<string> observations);

	vector<string> Viterbi(vector<string> observationSequence);

	double Forward(vector<string> observationSequence);
	double Forward(vector<string> observationSequence, Matrix2D * Alpha);

	double Backward(vector<string> observationSequence);
	double Backward(vector<string> observationSequence, Matrix2D * Beta);

	vector<string> Posterior(vector<string> observationSequence);

	void BaumWelch(vector<string> observationSequence);

	void Normalize(int command);

	Matrix1D Initial;
	Matrix2D Emission;
	Matrix2D Transition;

private:
	vector<string> States;
	vector<string> Observations;
};

HiddenMarkovModel::HiddenMarkovModel(vector<string> states, vector<string> observations)
{
	this->States = states;
	this->Observations = observations;
	this->Initial = MATRIX1D(States);
	this->Emission = MATRIX2D(States, Observations);
	this->Transition = MATRIX2D(States, States);
}

vector<string> HiddenMarkovModel::Viterbi(vector<string> observationSequence)
{
	Matrix2D Delta = MATRIX2D(observationSequence, States);

	for (int k = 0; k < (int)observationSequence.size(); k++)
		for (int j = 0; j < (int)States.size(); j++)
			//Initialization
			if (k == 0)
				Delta[k][j] = this->Initial[j] * this->Emission[j][observationSequence[k]];
			//Recursion
			else
			{
				double max = 0;
				for (int i = 0; i < (int)States.size(); i++)
				{
					double possibleMax = Delta[k - 1][i] * this->Transition[i][j];
					if (possibleMax > max)
						max = possibleMax;
				}
				Delta[k][j] = max * this->Emission[j][observationSequence[k]];
			}
	//Termination
	double max = 0; vector<string> q((int)observationSequence.size());
	for (int i = 0; i < (int)States.size(); i++)
		if (Delta[(int)observationSequence.size() - 1][i] > max)
		{
			max = Delta[(int)observationSequence.size() - 1][i];
			q[(int)observationSequence.size() - 1] = States[i];
		}
	//Back-Tracking
	for (int k = (int)observationSequence.size() - 2; k >= 0; k--)
	{
		double max = 0;
		for (int i = 0; i < (int)States.size(); i++)
		{
			double possibleMax = Delta[k][i] * this->Transition[i][q[k + 1]];
			if (possibleMax > max)
			{
				max = possibleMax;
				q[k] = States[i]; 
			}
		}
	}

	return q;
}

double HiddenMarkovModel::Forward(vector<string> observationSequence)
{
	Matrix2D Alpha;

	double result = Forward(observationSequence, &Alpha);
	
	return result;
}
double HiddenMarkovModel::Forward(vector<string> observationSequence, Matrix2D * Alpha)
{
	*Alpha = MATRIX2D(observationSequence, States);

	for (int k = 0; k < (int)observationSequence.size(); k++)
		for (int j = 0; j < (int)States.size(); j++)
			//Initialization
			if (k == 0)
				(*Alpha)[k][j] = this->Initial[j] * this->Emission[j][observationSequence[k]];
			//Recursion
			else
			{
				double sum = 0;
				for (int i = 0; i < (int)States.size(); i++)	sum += (*Alpha)[k - 1][i] * this->Transition[i][j];
				(*Alpha)[k][j] = sum * this->Emission[j][observationSequence[k]];
			}
	//Termination
	double sum = 0;
	for (int i = 0; i < (int)States.size(); i++) sum += (*Alpha)[(int)observationSequence.size() - 1][i];

	return sum;
}

double HiddenMarkovModel::Backward(vector<string> observationSequence)
{
	Matrix2D Beta;

	double result = Backward(observationSequence, &Beta);
	
	return result;
}
double HiddenMarkovModel::Backward(vector<string> observationSequence, Matrix2D * Beta)
{
	*Beta = MATRIX2D(observationSequence, States);

	for (int k = (int)observationSequence.size() - 1; k >= 0; k--)
		for (int i = 0; i < (int)States.size(); i++)
			//Initialization
			if (k == (int)observationSequence.size() - 1)
				(*Beta)[k][i] = 1;
			//Recursion
			else
			{
				double sum = 0;
				for (int j = 0; j < (int)States.size(); j++) sum += this->Transition[i][j] * this->Emission[j][observationSequence[k + 1]] * (*Beta)[k + 1][j];
				(*Beta)[k][i] = sum;
			}
	//Termination
	double sum = 0;
	for (int i = 0; i < (int)States.size(); i++) sum += this->Initial[i] * this->Emission[i][observationSequence[0]] * (*Beta)[0][i];

	return sum;
}

vector<string> HiddenMarkovModel::Posterior(vector<string> observationSequence)
{
	vector<string> output((int)observationSequence.size());
	Matrix2D Alpha, Beta;

	//Initialization
	double probabilityOfObservationSequence = Forward(observationSequence, &Alpha);
	Backward(observationSequence, &Beta);
	//Recursion
	for (int k = 0; k < (int)observationSequence.size(); k++)
	{
		double max = 0; string arg;
		for (int i = 0; i < (int)States.size(); i++)
		{
			double possibleMax = (Alpha[observationSequence[k]][i] * Beta[observationSequence[k]][i]) / probabilityOfObservationSequence;
			if (possibleMax > max)
			{
				max = possibleMax;
				arg = States[i];
			}
		}
		output[k] = arg;
	}
	
	return output;
}

void HiddenMarkovModel::BaumWelch(vector<string> observationSequence)
{
	Matrix2D Alpha, Beta, Gamma = MATRIX2D(observationSequence, States); Matrix3D Xi = MATRIX3D(observationSequence, States, States); double probabilityOfObservationSequence;

	//Initialization
	probabilityOfObservationSequence = Forward(observationSequence, &Alpha);

	Backward(observationSequence, &Beta);

	for (int i = 0; i < (int)States.size(); i++)
		for (int j = 0; j < (int)States.size(); j++)
			for (int k = 0; k < (int)observationSequence.size() - 1; k++)
				Xi[k][i][j] = (Alpha[k][i] * Beta[k + 1][j] * this->Transition[i][j] * this->Emission[j][observationSequence[k + 1]]) / probabilityOfObservationSequence;

	for (int k = 0; k < (int)observationSequence.size(); k++)
		for (int i = 0; i < (int)States.size(); i++)
			Gamma[k][i] = (Alpha[k][i] * Beta[k][i]) / probabilityOfObservationSequence;
	
	//Estimate Initial
	double sum = 0;
	for (int i = 0; i < (int)States.size(); i++)
		this->Initial[i] = Gamma[0][i];

	//Estimate Transition
	for (int i = 0; i < (int)States.size(); i++)
		for (int j = 0; j < (int)States.size(); j++)
		{
			double sum1 = 0, sum2 = 0;
			for (int k = 0; k < (int)observationSequence.size() - 1; k++) sum1 += Xi[k][i][j];
			for (int k = 0; k < (int)observationSequence.size() - 1; k++) sum2 += Gamma[k][i];
			this->Transition[i][j] = sum1 / sum2;
		}

	//Estimate Emission
	for (int j = 0; j < (int)States.size(); j++)
		for (int k = 0; k < (int)Observations.size(); k++)
		{
			double sum1 = 0, sum2 = 0;
			for (int l = 0; l < (int)observationSequence.size(); l++)
				if (observationSequence[l] == this->Observations[k])
					sum1 += Gamma[l][j];
			for (int l = 0; l < (int)observationSequence.size(); l++)
				sum2 += Gamma[l][j];
			this->Emission[j][k] = sum1 / sum2;
		}
}

void HiddenMarkovModel::Normalize(int command)
{
	if (command & HIDDEN_MARKOV_MODEL_INITIAL_MATRIX)
	{
		double sum = 0;
		for (int i = 0; i < (int)States.size(); i++)
			sum += this->Initial[this->States[i]];

		for (int i = 0; i < (int)States.size(); i++)
			this->Initial[this->States[i]] /= sum;
	}
	if (command & HIDDEN_MARKOV_MODEL_EMISSION_MATRIX)
		for (int i = 0; i < (int)States.size(); i++)
		{
			double sum = 0;
			for (int j = 0; j < (int)Observations.size(); j++)
				sum += this->Emission[this->States[i]][this->Observations[j]];
			for (int j = 0; j < (int)Observations.size(); j++)
				this->Emission[this->States[i]][this->Observations[j]] /= sum;
		}
	if (command & HIDDEN_MARKOV_MODEL_TRANSITION_MATRIX)
		for (int i = 0; i < (int)States.size(); i++)
		{
			double sum = 0;
			for (int j = 0; j < (int)States.size(); j++)
				sum += this->Transition[this->States[i]][this->States[j]];
			for (int j = 0; j < (int)States.size(); j++)
				this->Transition[this->States[i]][this->States[j]] /= sum;
		}
}

#endif
