#ifndef ROC_POSITION_H
#define ROC_POSITION_H

struct ROC_position
{
	int layer;
	int module;
	int ladder;
	ROC_position(){};
	ROC_position(int layer_p, int module_p, int ladder_p): 
		layer(layer_p), module(module_p), ladder(ladder_p) {};
	~ROC_position() {};
	ROC_position(const ROC_position& other)
	{
		this -> layer = other.layer;
		this -> module = other.module;
		this -> ladder = other.ladder; 
	};
	const ROC_position& operator=(const ROC_position& other)
	{
		this -> layer = other.layer;
		this -> module = other.module;
		this -> ladder = other.ladder;
		return *this;
	};
	friend bool operator<(const ROC_position& lhs, const ROC_position& rhs);
};

bool operator<(const ROC_position& lhs, const ROC_position& rhs)
{
	if(lhs.layer < rhs.layer) { return true; }
	if(lhs.layer > rhs.layer) { return false; }
	if(lhs.module < rhs.module) { return true; }
	if(lhs.module > rhs.module) { return false; }
	if(lhs.ladder < rhs.ladder) { return true; }
	if(lhs.ladder > rhs.ladder) { return false; }
	return false;
};

#endif