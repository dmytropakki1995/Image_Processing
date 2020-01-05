#include "pch.h"
#include "NetWork.h"
#include <iostream>


using namespace std;

NeuralNetwork::NeuralNetwork()
{
	_data_layer_1 = nullptr;
	_data_layer_2 = nullptr;
	_data_layer_3 = nullptr;
}

NeuralNetwork::NeuralNetwork(Neuron *layer_1, Neuron * layer_2, Neuron * layer_3)
{
	_data_layer_1 = layer_1;
	_data_layer_2 = layer_2;
	_data_layer_3 = layer_3;
}

void NeuralNetwork::defineLayer(float * input_data, float ** weight, int layer_size, int input_size, int layer_Number)
{
	if (layer_Number == 1)
	{
		for (int i = 0; i < layer_size; i++)
		{
			_data_layer_1[i].setWeight(weight[i]);
			_data_layer_1[i].feedForward(input_data, input_size);
		}
	}
	else
	{
		cout << "ERROR: Invalid input_value!" << endl;
	}
}

void NeuralNetwork::defineLayer(Neuron * input_data, float ** weight, int layer_size, int input_size, int layer_Number)
{
	switch (layer_Number)
	{
	case 1:
		throw "ERROR: Invalid input_value!";

	case 2:
		for (int i = 0; i < layer_size; i++)
		{
			_data_layer_2[i].setWeight(weight[i]);
			_data_layer_2[i].feedForward(input_data, input_size);
		}
		break;

	case 3:
		for (int i = 0; i < layer_size; i++)
		{
			_data_layer_3[i].setWeight(weight[i]);
			_data_layer_3[i].feedForward(input_data, input_size);
		}
		break;

	default:
		break;
	}
}

float ** NeuralNetwork::Weight(Weights type)
{
	int in_size = 9;
	int layer_size_1 = 8;
	int layer_size_2 = 12;
	int layer_size_3 = 3;

	switch (type)
	{
	case weight_1:
	{
		float ** w_1 = new float *[layer_size_1];
		for (int i = 0; i < layer_size_1; i++)
			w_1[i] = new float[in_size];

		w_1[0][0] = 0.5; w_1[0][1] = -0.5; w_1[0][2] = 0; w_1[0][3] = 0.5; w_1[0][4] = -0.5; w_1[0][5] = 0; w_1[0][6] = 0; w_1[0][7] = 0; w_1[0][8] = 0;
		w_1[1][0] = 0.5; w_1[1][1] = 0.5; w_1[1][2] = 0; w_1[1][3] = -0.5; w_1[1][4] = -0.5; w_1[1][5] = 0; w_1[1][6] = 0; w_1[1][7] = 0; w_1[1][8] = 0;

		w_1[2][0] = 0; w_1[2][1] = 0.5; w_1[2][2] = -0.5; w_1[2][3] = 0; w_1[2][4] = 0.5; w_1[2][5] = -0.5; w_1[2][6] = 0; w_1[2][7] = 0; w_1[2][8] = 0;
		w_1[3][0] = 0; w_1[3][1] = 0.5; w_1[3][2] = 0.5; w_1[3][3] = 0; w_1[3][4] = -0.5; w_1[3][5] = -0.5; w_1[3][6] = 0; w_1[3][7] = 0; w_1[3][8] = 0;

		w_1[4][0] = 0; w_1[4][1] = 0; w_1[4][2] = 0; w_1[4][3] = 0.5; w_1[4][4] = -0.5; w_1[4][5] = 0; w_1[4][6] = 0.5; w_1[4][7] = -0.5; w_1[4][8] = 0;
		w_1[5][0] = 0; w_1[5][1] = 0; w_1[5][2] = 0; w_1[5][3] = 0.5; w_1[5][4] = 0.5; w_1[5][5] = 0; w_1[5][6] = -0.5; w_1[5][7] = -0.5; w_1[5][8] = 0;

		w_1[6][0] = 0; w_1[6][1] = 0; w_1[6][2] = 0; w_1[6][3] = 0; w_1[6][4] = 0.5; w_1[6][5] = -0.5; w_1[6][6] = 0; w_1[6][7] = 0.5; w_1[6][8] = -0.5;
		w_1[7][0] = 0; w_1[7][1] = 0; w_1[7][2] = 0; w_1[7][3] = 0; w_1[7][4] = 0.5; w_1[7][5] = 0.5; w_1[7][6] = 0; w_1[7][7] = -0.5; w_1[7][8] = -0.5;

		return w_1;
		break;
	}

	case weight_2:
	{
		float lx1 = _data_layer_1[0]._value;
		float ly1 = _data_layer_1[1]._value;
		float lx2 = _data_layer_1[2]._value;
		float ly2 = _data_layer_1[3]._value;
		float lx3 = _data_layer_1[4]._value;
		float ly3 = _data_layer_1[5]._value;
		float lx4 = _data_layer_1[6]._value;
		float ly4 = _data_layer_1[7]._value;

		float ** w_2 = new float *[layer_size_2];
		for (int i = 0; i < layer_size_2; i++)
			w_2[i] = new float[layer_size_1];

		w_2[0][0] = lx1;  w_2[0][1] = 0;  w_2[0][2] = 0;  w_2[0][3] = 0;  w_2[0][4] = 0;  w_2[0][5] = 0;  w_2[0][6] = 0;  w_2[0][7] = 0;
		w_2[1][0] = ly1;  w_2[1][1] = 0;  w_2[1][2] = 0;  w_2[1][3] = 0;  w_2[1][4] = 0;  w_2[1][5] = 0;  w_2[1][6] = 0;  w_2[1][7] = 0;
		w_2[2][0] = 0;  w_2[2][1] = ly1;  w_2[2][2] = 0;  w_2[2][3] = 0;  w_2[2][4] = 0;  w_2[2][5] = 0;  w_2[2][6] = 0;  w_2[2][7] = 0;
		w_2[3][0] = 0;  w_2[3][1] = 0;  w_2[3][2] = lx2;  w_2[3][3] = 0;  w_2[3][4] = 0;  w_2[3][5] = 0;  w_2[3][6] = 0;  w_2[3][7] = 0;
		w_2[4][0] = 0;  w_2[4][1] = 0;  w_2[4][2] = ly2;  w_2[4][3] = 0;  w_2[4][4] = 0;  w_2[4][5] = 0;  w_2[4][6] = 0;  w_2[4][7] = 0;
		w_2[5][0] = 0;  w_2[5][1] = 0;  w_2[5][2] = 0;  w_2[5][3] = ly2;  w_2[5][4] = 0;  w_2[5][5] = 0;  w_2[5][6] = 0;  w_2[5][7] = 0;
		w_2[6][0] = 0;  w_2[6][1] = 0;  w_2[6][2] = 0;  w_2[6][3] = 0;  w_2[6][4] = lx3;  w_2[6][5] = 0;  w_2[6][6] = 0;  w_2[6][7] = 0;
		w_2[7][0] = 0;  w_2[7][1] = 0;  w_2[7][2] = 0;  w_2[7][3] = 0;  w_2[7][4] = ly3;  w_2[7][5] = 0;  w_2[7][6] = 0;  w_2[7][7] = 0;
		w_2[8][0] = 0;  w_2[8][1] = 0;  w_2[8][2] = 0;  w_2[8][3] = 0;  w_2[8][4] = 0;  w_2[8][5] = ly3;  w_2[8][6] = 0;  w_2[8][7] = 0;
		w_2[9][0] = 0;  w_2[9][1] = 0;  w_2[9][2] = 0;  w_2[9][3] = 0;  w_2[9][4] = 0;  w_2[9][5] = 0;  w_2[9][6] = lx4;  w_2[9][7] = 0;
		w_2[10][0] = 0; w_2[10][1] = 0; w_2[10][2] = 0; w_2[10][3] = 0; w_2[10][4] = 0; w_2[10][5] = 0; w_2[10][6] = ly4; w_2[10][7] = 0;
		w_2[11][0] = 0; w_2[11][1] = 0; w_2[11][2] = 0; w_2[11][3] = 0; w_2[11][4] = 0; w_2[11][5] = 0; w_2[11][6] = 0; w_2[11][7] = ly4;

		return w_2;
		break;
	}

	case weight_3:
	{
		float ** w_3 = new float *[layer_size_3];
		for (int i = 0; i < layer_size_3; i++)
		{
			w_3[i] = new float[layer_size_2];
			for (int j = 0; j < layer_size_2; j++)
			{
				if ((j - i) % 3 == 0)
					w_3[i][j] = 0.5;
				else
					w_3[i][j] = 0;
			}
		}
		return w_3;
		break;
	}

	default:
		break;
	}
}

NeuralNetwork::~NeuralNetwork()
{
	_data_layer_1->~Neuron();
	_data_layer_2->~Neuron();
	_data_layer_3->~Neuron();
}