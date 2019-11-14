#pragma once
#include "Neuron.h"

struct NeuralNetwork
{
	Neuron * _data_layer_1;
	Neuron * _data_layer_2;
	Neuron * _data_layer_3;

	NeuralNetwork();
	NeuralNetwork(Neuron *layer_1, Neuron * layer_2, Neuron * layer_3);
	void defineLayer(float * input_data, float ** weight, int layer_size, int input_size, int layer_Number);
	void defineLayer(Neuron * input_data, float ** weight, int layer_size, int input_size, int layer_Number);
	float ** Weight(Weights type);

	~NeuralNetwork();
};