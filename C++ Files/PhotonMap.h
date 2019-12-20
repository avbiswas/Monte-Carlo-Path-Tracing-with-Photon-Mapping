#include <iostream>
#include <vector>
#include "Vec3f.h"
using namespace std;

/*
Photon Map Data Structure
*/

struct Photon {
	
	Vec3f position;
	Vec3f direction;
	Color power;
	Photon() {}
	Photon( Vec3f& vec,  Vec3f& dir, Color p) :position(vec), direction(dir), power(p) {}

};

enum cut_axis {
	X,
	Y,
	Z
};
class KDNode {
	// The KD Tree
	Photon p;
	cut_axis cutAxis;
	KDNode* left;
	KDNode* right;
public:
	KDNode() {}
	KDNode(Photon P, cut_axis ax) {
		p = P;
		cutAxis = ax;
		left = nullptr;
		right = nullptr;
	}

	KDNode(Photon P) {
		p = P;
		cutAxis = X;

		left = nullptr;
		right = nullptr;
	}
	void insert(Photon P) {
		// Recursively checks the location to add the photon after successively checking the current axis of division at each depth starting from the root
		// Basic (unbalanced) KD Tree implementation
		if (cutAxis == Y) {
			if (P.position.x < p.position.x) {
				if (left == nullptr) {
					left =  new KDNode(P, X);
					return;
				}
				else {
					left->insert(P);
				}
			}
			else {
				if (right == nullptr) {
					right =  new KDNode(P, X);
					return;
				}
				else {
					right->insert(P);
				}
			}
		}
		if (cutAxis == X) {
			if (P.position.z < p.position.z) {
				if (left == nullptr) {
					left =  new KDNode(P, Z);
					return;
				}
				else {
					left->insert(P);
				}
			}
			else {
				if (right == nullptr) {
					right =  new KDNode(P, Z);
					return;
				}
				else {
					right->insert(P);
				}
			}
		}
		if (cutAxis == Z) {
			if (P.position.y < p.position.y) {
				if (left == nullptr) {
					left =  new KDNode(P, Y);
					return;
				}
				else {
					left->insert(P);
				}
			}
			else {
				if (right == nullptr) {
					right =  new KDNode(P, Y);
					return;
				}
				else {
					right->insert(P);
				}
			}
		}

	}

	int size() {

		if (left == nullptr && right == nullptr) {
			return 1;
		}
		if (left == nullptr) {
			return right->size() + 1;
		}
		if (right == nullptr) {
			return left->size() + 1;
		}
		return 1 + left->size() + right->size();
	}

	void rangeSearch( Vec3f& c1,  Vec3f& c2, vector<Photon>& maps, int size) {
		// c1 = Query Position 1
		// c2 = Query Position 2
		// maps = output vector to store all photons in the photon map that lies inside the cuboid formed by (c1, c2)
		// size = maximum number of photons required

		if (maps.size() >= size)
			return;
		if (cutAxis == X) {
			if (p.position.z >= c1.z) {
				if (p.position.z <= c2.z) {
					if (p.position.x >= c1.x && p.position.x <= c2.x &&
						p.position.y >= c1.y && p.position.y <= c2.y) {
						maps.push_back(p);
					}
					if (left != nullptr) left->rangeSearch(c1, c2, maps, size);
					if (right != nullptr) right->rangeSearch(c1, c2, maps, size);
					return;
				}
				else {
					if (left != nullptr) left->rangeSearch(c1, c2, maps, size);
					return;
				}
			}
			else {
				if (right != nullptr) right->rangeSearch(c1, c2, maps, size);
				return;
			}
		}
		else if (cutAxis == Y) {
			if (p.position.x >= c1.x) {
				if (p.position.x <= c2.x) {
					if (p.position.y >= c1.y && p.position.y <= c2.y &&
						p.position.z >= c1.z && p.position.z <= c2.z
						) {
						maps.push_back(p);
					}
					if (left != nullptr) left->rangeSearch(c1, c2, maps, size);
					if (right != nullptr) right->rangeSearch(c1, c2, maps, size);
					return;
				}
				else {
					if (left != nullptr) left->rangeSearch(c1, c2, maps, size);
					return;
				}
			}
			else {
				if (right != nullptr) right->rangeSearch(c1, c2, maps, size);
				return;
			}
		}
		else {
			if (p.position.y >= c1.y) {
				if (p.position.y <= c2.y) {
					if (p.position.z >= c1.z && p.position.z <= c2.z &&
						p.position.x >= c1.x && p.position.x <= c2.x) {
						maps.push_back(p);
					}
					if (left != nullptr) left->rangeSearch(c1, c2, maps, size);
					if (right != nullptr) right->rangeSearch(c1, c2, maps, size);
					return;
				}
				else {
					if (left != nullptr) left->rangeSearch(c1, c2, maps, size);
					return;
				}
			}
			else {
				if (right != nullptr) right->rangeSearch(c1, c2, maps, size);
				return;
			}
		}
	}

	void showDef() {

		// Utility function to show how the photon map "looks". 
		// There for debugging purposes.

		string axis = (cutAxis == X) ? "X" : (cutAxis == Y) ? "Y" : "Z";
		cout << p.position << "Axis : " << axis << endl;
		if (left != nullptr) {
			cout << "LEFT : " << endl;
			left->showDef();
		}
		else {
			cout << "NO LEFT CHILD" << endl;
		}
		if (right != nullptr) {
			cout << "RIGHT : " << endl;
			right->showDef();
		}
		else {
			cout << "NO RIGHT CHILD" << endl;
		}
		cout << endl;
	}
};
