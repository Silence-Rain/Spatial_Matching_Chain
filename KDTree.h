#include <iostream>  
#include <algorithm>  
#include <stack>  
#include <math.h>  
#include "Node.h"

using namespace std;

struct Tnode
{
	struct Node dom_elt;
	int split;
	struct Tnode * left;
	struct Tnode * right;
};


class KDTree
{
public:
	static bool cmp1(Node a, Node b) 
	{
		return a.pos.x < b.pos.x;
	}

	static bool cmp2(Node a, Node b) 
	{
		return a.pos.y < b.pos.y;
	}

	static bool equal(Node a, Node b) 
	{
		if (a.pos.x == b.pos.x && a.pos.y == b.pos.y)
		{
			return true;
		}
		else {
			return false;
		}
	}

	void ChooseSplit(Node exm_set[], int size, int &split, Node &SplitChoice) 
	{
		// 计算x，y维度方差，选择较大的进行分割
		double tmp1, tmp2;
		tmp1 = tmp2 = 0;
		for (int i = 0; i < size; ++i)
		{
			tmp1 += 1.0 / (double)size * exm_set[i].pos.x * exm_set[i].pos.x;
			tmp2 += 1.0 / (double)size * exm_set[i].pos.x;
		}
		double varX = tmp1 - tmp2 * tmp2;

		tmp1 = tmp2 = 0;
		for (int i = 0; i < size; ++i)
		{
			tmp1 += 1.0 / (double)size * exm_set[i].pos.y * exm_set[i].pos.y;
			tmp2 += 1.0 / (double)size * exm_set[i].pos.y;
		}
		double varY = tmp1 - tmp2 * tmp2;

		split = varX > varY ? 0 : 1;

		if (split == 0)
		{
			sort(exm_set, exm_set + size, cmp1);
		}
		else {
			sort(exm_set, exm_set + size, cmp2);
		}

		// 选择分割中点 
		SplitChoice = exm_set[size / 2];
	}

	Tnode* build_kdtree(Node exm_set[], int size, Tnode* T) 
	{
		if (size == 0) 
		{
			return NULL;
		}
		else 
		{
			int split;
			Node dom_elt;
			ChooseSplit(exm_set, size, split, dom_elt);
			Node* exm_set_right = new Node[size];
			Node* exm_set_left = new Node[size];
			int sizeleft, sizeright;
			sizeleft = sizeright = 0;

			if (split == 0)
			{
				for (int i = 0; i < size; ++i)
				{
					if (!equal(exm_set[i], dom_elt) && exm_set[i].pos.x <= dom_elt.pos.x)
					{
						exm_set_left[sizeleft] = exm_set[i];
						sizeleft++;
					}
					else if (!equal(exm_set[i], dom_elt) && exm_set[i].pos.x > dom_elt.pos.x)
					{
						exm_set_right[sizeright] = exm_set[i];
						sizeright++;
					}
				}
			}
			else {
				for (int i = 0; i < size; ++i)
				{
					if (!equal(exm_set[i], dom_elt) && exm_set[i].pos.y <= dom_elt.pos.y)
					{
						exm_set_left[sizeleft] = exm_set[i];
						sizeleft++;
					}
					else if (!equal(exm_set[i], dom_elt) && exm_set[i].pos.y > dom_elt.pos.y)
					{
						exm_set_right[sizeright] = exm_set[i];
						sizeright++;
					}
				}
			}

			T = new Tnode;
			T->dom_elt = dom_elt;
			T->split = split;
			T->left = build_kdtree(exm_set_left, sizeleft, T->left);
			T->right = build_kdtree(exm_set_right, sizeright, T->right);

			delete[] exm_set_left;
			delete[] exm_set_right;

			return T;
		}
	}


	double Distance(Node a, Node b) 
	{
		return sqrt((a.pos.x - b.pos.x) * (a.pos.x - b.pos.x) + (a.pos.y - b.pos.y) * (a.pos.y - b.pos.y));
	}


	string searchNearest(Tnode * Kd, Node target, Node &nearestpoint, double & distance) 
	{
		//1. 如果Kd是空的，则设dist为无穷大返回  
		//2. 向下搜索直到叶子结点  
		stack<Tnode*> search_path;
		Tnode* pSearch = Kd;
		Node nearest;
		double dist;

		while (pSearch != NULL)
		{
			// 保存查找路径 
			search_path.push(pSearch);

			if (pSearch->split == 0)
			{
				if (target.pos.x <= pSearch->dom_elt.pos.x)
				{
					pSearch = pSearch->left;
				}
				else
				{
					pSearch = pSearch->right;
				}
			}
			else 
			{
				if (target.pos.y <= pSearch->dom_elt.pos.y)
				{
					pSearch = pSearch->left;
				}
				else
				{
					pSearch = pSearch->right;
				}
			}
		}
		//取出search_path最后一个赋给nearest  
		nearest = search_path.top()->dom_elt;
		search_path.pop();
		dist = Distance(nearest, target);

		//3. 回溯搜索路径  
		Tnode* pBack;

		while (search_path.size() != 0)
		{
			//取出search_path最后一个结点赋给pBack  
			pBack = search_path.top();
			search_path.pop();

			if (pBack->left == NULL && pBack->right == NULL) /* 如果pBack为叶子结点 */
			{
				if (Distance(nearest, target) > Distance(pBack->dom_elt, target))
				{
					nearest = pBack->dom_elt;
					dist = Distance(pBack->dom_elt, target);
				}
			}
			else
			{
				int s = pBack->split;
				if (s == 0)
				{
					if (fabs(pBack->dom_elt.pos.x - target.pos.x) < dist) /* 如果以target为中心的圆（球或超球），半径为dist的圆与分割超平面相交， 那么就要跳到另一边的子空间去搜索 */
					{
						if (Distance(nearest, target) > Distance(pBack->dom_elt, target))
						{
							nearest = pBack->dom_elt;
							dist = Distance(pBack->dom_elt, target);
						}
						if (target.pos.x <= pBack->dom_elt.pos.x) /* 如果target位于pBack的左子空间，那么就要跳到右子空间去搜索 */
							pSearch = pBack->right;
						else
							pSearch = pBack->left; /* 如果target位于pBack的右子空间，那么就要跳到左子空间去搜索 */
						if (pSearch != NULL)
							//pSearch加入到search_path中  
							search_path.push(pSearch);
					}
				}
				else 
				{
					if (fabs(pBack->dom_elt.pos.y - target.pos.y) < dist) /* 如果以target为中心的圆（球或超球），半径为dist的圆与分割超平面相交， 那么就要跳到另一边的子空间去搜索 */
					{
						if (Distance(nearest, target) > Distance(pBack->dom_elt, target))
						{
							nearest = pBack->dom_elt;
							dist = Distance(pBack->dom_elt, target);
						}
						if (target.pos.y <= pBack->dom_elt.pos.y) /* 如果target位于pBack的左子空间，那么就要跳到右子空间去搜索 */
							pSearch = pBack->right;
						else
							pSearch = pBack->left; /* 如果target位于pBack的右子空间，那么就要跳到左子空间去搜索 */
						if (pSearch != NULL)
							// pSearch加入到search_path中  
							search_path.push(pSearch);
					}
				}
			}
		}

		nearestpoint = nearest;
		distance = dist;

		return nearestpoint.id;
	}
};
