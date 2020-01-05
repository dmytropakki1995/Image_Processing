#pragma once

enum Weights {
	weight_1,
	weight_2,
	weight_3
};

struct Neuron
{
	float *_weight;
	float _bias;
	float _value;

	Neuron();
	void setWeight(float * weight);
	void setBias(float bias);
	void feedForward(float * input_data, int input_size);
	void feedForward(Neuron * input_data, int input_size);
	~Neuron();
};

