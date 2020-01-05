#include "pch.h"
#include "Neuron.h"
#include <iostream>

using namespace std;

Neuron::Neuron()
{
	_weight = nullptr;
	_bias = 0;
	_value = 0;
}

void Neuron::setWeight(float * weight)
{
	_weight = weight;
}

void Neuron::setBias(float bias)
{
	_bias = bias;
}

void Neuron::feedForward(float * input_data, int input_size)
{
	float summ = 0;
	for (int i = 0; i < input_size; i++)
	{
		summ += input_data[i] * _weight[i];
	}
	_value = summ + _bias;
}
void Neuron::feedForward(Neuron * input_data, int input_size)
{
	float summ = 0;
	for (int i = 0; i < input_size; i++)
	{
		summ += input_data[i]._value * _weight[i];
	}
	_value = summ + _bias;
}

Neuron::~Neuron()
{
	if (_weight) delete[] _weight;
	_weight = nullptr;

}