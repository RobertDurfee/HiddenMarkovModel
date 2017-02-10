#ifndef HIDDEN_MARKOV_MODEL_HEADER
#define HIDDEN_MARKOV_MODEL_HEADER

#include "Matrices.h"
#include <vector>
#include <string>
#include <iomanip>
#include <random>
#include <chrono>

#define HIDDEN_MARKOV_MODEL_INITIAL_MATRIX 0x1
#define HIDDEN_MARKOV_MODEL_EMISSION_MATRIX 0x2
#define HIDDEN_MARKOV_MODEL_TRANSITION_MATRIX 0x4

using namespace std;

class HiddenMarkovModel
{
public:
	HiddenMarkovModel(vector<string> states, vector<string> observations)
	{
		this->States = states;
		this->Observations = observations;
		this->Initial.Assign(this->States.size(), this->States);
		this->Emission.Assign(this->States.size(), this->Observations.size(), this->States, this->Observations);
		this->Transition.Assign(this->States.size(), this->States.size(), this->States, this->States);
	}
	vector<string> Viterbi(vector<string> observationSequence)
	{
		vector<string> output;

		Matrix2D<double> Delta(observationSequence.size(), this->States.size(), observationSequence, this->States);

		for (int k = 0; k < (int)observationSequence.size(); k++)
			for (int j = 0; j < (int)this->States.size(); j++)
				//Initialization
				if (k == 0)
					Delta[k][j] = this->Initial[j] * this->Emission[j][observationSequence[k]];
				//Recursion
				else
				{
					double max = 0;
					for (int i = 0; i < (int)this->States.size(); i++)
					{
						double possibleMax = Delta[k - 1][i] * this->Transition[i][j];
						if (possibleMax > max)
							max = possibleMax;
					}
					Delta[k][j] = max * this->Emission[j][observationSequence[k]];
				}
		//Termination
		double max = 0; string * q = new string[observationSequence.size()];
		for (int i = 0; i < (int)this->States.size(); i++)
			if (Delta[observationSequence.size() - 1][i] > max)
			{
				max = Delta[observationSequence.size() - 1][i];
				q[observationSequence.size() - 1] = this->States[i];
			}
		//Back-Tracking
		for (int k = (int)observationSequence.size() - 2; k >= 0; k--)
		{
			double max = 0;
			for (int i = 0; i < (int)this->States.size(); i++)
			{
				double possibleMax = Delta[k][i] * this->Transition[i][q[k + 1]];
				if (possibleMax > max)
				{
					max = possibleMax;
					q[k] = this->States[i];
				}
			}
		}

		for (int i = 0; i < (int)observationSequence.size(); i++)
			output.push_back(q[i]);
		delete[] q;
		return output;
	}
	double Forward(vector<string> observationSequence)
	{
		Matrix2D<double> Alpha(observationSequence.size(), this->States.size(), observationSequence, this->States);

		for (int k = 0; k < (int)observationSequence.size(); k++)
			for (int j = 0; j < (int)this->States.size(); j++)
				//Initialization
				if (k == 0)
					Alpha[k][j] = this->Initial[j] * this->Emission[j][observationSequence[k]];
				//Recursion
				else
				{
					double sum = 0;
					for (int i = 0; i < (int)this->States.size(); i++)	sum += Alpha[k - 1][i] * this->Transition[i][j];
					Alpha[k][j] = sum * this->Emission[j][observationSequence[k]];
				}
		//Termination
		double sum = 0;
		for (int i = 0; i < (int)this->States.size(); i++) sum += Alpha[observationSequence.size() - 1][i];

		return sum;
	}
	double Forward(vector<string> observationSequence, Matrix2D<double> * Alpha)
	{
		Alpha->Assign(observationSequence.size(), this->States.size(), observationSequence, this->States);

		for (int k = 0; k < (int)observationSequence.size(); k++)
			for (int j = 0; j < (int)this->States.size(); j++)
				//Initialization
				if (k == 0)
					(*Alpha)[k][j] = this->Initial[j] * this->Emission[j][observationSequence[k]];
				//Recursion
				else
				{
					double sum = 0;
					for (int i = 0; i < (int)this->States.size(); i++)	sum += (*Alpha)[k - 1][i] * this->Transition[i][j];
					(*Alpha)[k][j] = sum * this->Emission[j][observationSequence[k]];
				}
		//Termination
		double sum = 0;
		for (int i = 0; i < (int)this->States.size(); i++) sum += (*Alpha)[observationSequence.size() - 1][i];

		return sum;
	}
	double Backward(vector<string> observationSequence)
	{
		Matrix2D<double> Beta(observationSequence.size(), this->States.size(), observationSequence, this->States);

		for (int k = observationSequence.size() - 1; k >= 0; k--)
			for (int i = 0; i < (int)this->States.size(); i++)
				//Initialization
				if (k == observationSequence.size() - 1)
					Beta[k][i] = 1;
				//Recursion
				else
				{
					double sum = 0;
					for (int j = 0; j < (int)this->States.size(); j++) sum += this->Transition[i][j] * this->Emission[j][observationSequence[k + 1]] * Beta[k + 1][j];
					Beta[k][i] = sum;
				}
		//Termination
		double sum = 0;
		for (int i = 0; i < (int)this->States.size(); i++) sum += this->Initial[i] * this->Emission[i][observationSequence[0]] * Beta[0][i];

		return sum;
	}
	double Backward(vector<string> observationSequence, Matrix2D<double> * Beta)
	{
		Beta->Assign(observationSequence.size(), this->States.size(), observationSequence, this->States);

		for (int k = observationSequence.size() - 1; k >= 0; k--)
			for (int i = 0; i < (int)this->States.size(); i++)
				//Initialization
				if (k == observationSequence.size() - 1)
					(*Beta)[k][i] = 1;
				//Recursion
				else
				{
					double sum = 0;
					for (int j = 0; j < (int)this->States.size(); j++) sum += this->Transition[i][j] * this->Emission[j][observationSequence[k + 1]] * (*Beta)[k + 1][j];
					(*Beta)[k][i] = sum;
				}
		//Termination
		double sum = 0;
		for (int i = 0; i < (int)this->States.size(); i++) sum += this->Initial[i] * this->Emission[i][observationSequence[0]] * (*Beta)[0][i];

		return sum;
	}
	vector<string> Posterior(vector<string> observationSequence)
	{
		vector<string> output; Matrix2D<double> Alpha, Beta;
		//Initialization
		double probabilityOfObservationSequence = this->Forward(observationSequence, &Alpha);
		this->Backward(observationSequence, &Beta);
		//Recursion
		for (int k = 0; k < (int)observationSequence.size(); k++)
		{
			double max = 0; string arg;
			for (int i = 0; i < (int)this->States.size(); i++)
			{
				double possibleMax = (Alpha[observationSequence[k]][i] * Beta[observationSequence[k]][i]) / probabilityOfObservationSequence;
				if (possibleMax > max)
				{
					max = possibleMax;
					arg = this->States[i];
				}
			}
			output.push_back(arg);
		}

		return output;
	}
	void BaumWelch(vector<string> observationSequence)
	{
		Matrix2D<double> Alpha, Beta, Gamma(observationSequence.size(), this->States.size(), observationSequence, this->States); Matrix3D<double> Xi(observationSequence.size(), this->States.size(), this->States.size(), observationSequence, this->States, this->States); double probabilityOfObservationSequence;

		//Initialization
		probabilityOfObservationSequence = this->Forward(observationSequence, &Alpha);

		this->Backward(observationSequence, &Beta);

		for (int i = 0; i < (int)this->States.size(); i++)
			for (int j = 0; j < (int)this->States.size(); j++)
				for (int k = 0; k < (int)observationSequence.size() - 1; k++)
					Xi[k][i][j] = (Alpha[k][i] * Beta[k + 1][j] * this->Transition[i][j] * this->Emission[j][observationSequence[k + 1]]) / probabilityOfObservationSequence;

		for (int k = 0; k < (int)observationSequence.size(); k++)
			for (int i = 0; i < (int)this->States.size(); i++)
				Gamma[k][i] = (Alpha[k][i] * Beta[k][i]) / probabilityOfObservationSequence;

		//Estimate Initial
		double sum = 0;
		for (int i = 0; i < (int)this->States.size(); i++)
			this->Initial[i] = Gamma[0][i];

		//Estimate Transition
		for (int i = 0; i < (int)this->States.size(); i++)
			for (int j = 0; j < (int)this->States.size(); j++)
			{
				double sum1 = 0, sum2 = 0;
				for (int k = 0; k < (int)observationSequence.size() - 1; k++) sum1 += Xi[k][i][j];
				for (int k = 0; k < (int)observationSequence.size() - 1; k++) sum2 += Gamma[k][i];
				this->Transition[i][j] = sum1 / sum2;
			}

		//Estimate Emission
		for (int j = 0; j < (int)this->States.size(); j++)
			for (int k = 0; k < (int)this->Observations.size(); k++)
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
	//Experimental
	void GenerateRandomSequence(int numberOfObservations, vector<string> * observationSequence, vector<string> * stateSequence)
	{
		int seed = std::chrono::system_clock::now().time_since_epoch().count();
		mt19937 generator(seed);
		vector<double> distributionValues;

		for (int k = 0; k < numberOfObservations; k++)
		{
			if (k == 0)
			{
				distributionValues.clear();
				for (int i = 0; i < (int)this->States.size(); i++)
					distributionValues.push_back(this->Initial[this->States[i]]);

				discrete_distribution<int> initialDistributions(distributionValues.begin(), distributionValues.end());

				stateSequence->push_back(this->States[initialDistributions(generator)]);

				distributionValues.clear();
				for (int i = 0; i < (int)this->Observations.size(); i++)
					distributionValues.push_back(this->Emission[(*stateSequence)[k]][Observations[i]]);

				discrete_distribution<int> emissionDistributions(distributionValues.begin(), distributionValues.end());

				observationSequence->push_back(this->Observations[emissionDistributions(generator)]);
			}
			else
			{
				distributionValues.clear();
				for (int i = 0; i < (int)this->States.size(); i++)
					distributionValues.push_back(this->Transition[(*stateSequence)[k - 1]][this->States[i]]);

				discrete_distribution<int> transitionDistributions(distributionValues.begin(), distributionValues.end());

				stateSequence->push_back(this->States[transitionDistributions(generator)]);

				distributionValues.clear();
				for (int i = 0; i < (int)this->Observations.size(); i++)
					distributionValues.push_back(this->Emission[(*stateSequence)[k]][Observations[i]]);

				discrete_distribution<int> emissionDistributions(distributionValues.begin(), distributionValues.end());

				observationSequence->push_back(this->Observations[emissionDistributions(generator)]);
			}
		}
	}
	void Normalize(int command)
	{
		if(command & HIDDEN_MARKOV_MODEL_INITIAL_MATRIX)
		{
			double sum = 0;
			for (int i = 0; i < (int)this->States.size(); i++)
				sum += this->Initial[this->States[i]];

			for (int i = 0; i < (int)this->States.size(); i++)
				this->Initial[this->States[i]] /= sum;
		}
		if(command & HIDDEN_MARKOV_MODEL_EMISSION_MATRIX)
			for (int i = 0; i < (int)this->States.size(); i++)
			{
				double sum = 0;
				for (int j = 0; j < (int)this->Observations.size(); j++)
					sum += this->Emission[this->States[i]][this->Observations[j]];
				for (int j = 0; j < (int)this->Observations.size(); j++)
					this->Emission[this->States[i]][this->Observations[j]] /= sum;
			}
		if(command & HIDDEN_MARKOV_MODEL_TRANSITION_MATRIX)
			for (int i = 0; i < (int)this->States.size(); i++)
			{
				double sum = 0;
				for (int j = 0; j < (int)this->States.size(); j++)
					sum += this->Transition[this->States[i]][this->States[j]];
				for (int j = 0; j < (int)this->States.size(); j++)
					this->Transition[this->States[i]][this->States[j]] /= sum;
			}
	}

	Matrix1D<double> Initial;
	Matrix2D<double> Emission;
	Matrix2D<double> Transition;
	vector<string> States;
	vector<string> Observations;
};

#endif