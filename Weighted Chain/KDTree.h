#include <iostream>  
#include <algorithm>  
#include <stack>  
#include <math.h>  
#include "Node.h"

using namespace std;

struct Tnode
{
	struct Node cooordinate;
	int split;
	struct Tnode* left;
	struct Tnode* right;
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
		return a.pos.x == b.pos.x && a.pos.y == b.pos.y;
	}

	// ѡ��x, yά���У�����ϴ��ά�Ƚ��зָ�
	void chooseSplitDim(Node dataSet[], int size, int &split, Node &splitChoice) 
	{
		// ����xά�ȷ���
		double tmp1 = 0, tmp2 = 0;
		for (int i = 0; i < size; i++)
		{
			tmp1 += 1.0 / (double)size * dataSet[i].pos.x * dataSet[i].pos.x;
			tmp2 += 1.0 / (double)size * dataSet[i].pos.x;
		}
		double varX = tmp1 - tmp2 * tmp2;

		// ����yά�ȷ���
		tmp1 = tmp2 = 0;
		for (int i = 0; i < size; i++)
		{
			tmp1 += 1.0 / (double)size * dataSet[i].pos.y * dataSet[i].pos.y;
			tmp2 += 1.0 / (double)size * dataSet[i].pos.y;
		}
		double varY = tmp1 - tmp2 * tmp2;

		// ѡ��ϴ�Ľ��зָ�
		split = varX > varY ? 0 : 1;
		if (split == 0)
			sort(dataSet, dataSet + size, cmp1);
		else
			sort(dataSet, dataSet + size, cmp2);

		// ѡ��ָ��е� 
		splitChoice = dataSet[size / 2];
	}

	Tnode* buildTree(Node dataSet[], int size, Tnode* tree) 
	{
		if (size == 0) 
			return NULL;

		int split;
		Node cooordinate;
		chooseSplitDim(dataSet, size, split, cooordinate);

		Node* dataSetRight = new Node[size];
		Node* dataSetLeft = new Node[size];
		int sizeLeft = 0, sizeRight = 0;

		if (split == 0)
		{
			for (int i = 0; i < size; i++)
			{
				// ����������������������
				if (!equal(dataSet[i], cooordinate) && dataSet[i].pos.x <= cooordinate.pos.x)
				{
					dataSetLeft[sizeLeft] = dataSet[i];
					sizeLeft++;
				}
				else if (!equal(dataSet[i], cooordinate) && dataSet[i].pos.x > cooordinate.pos.x)
				{
					dataSetRight[sizeRight] = dataSet[i];
					sizeRight++;
				}
			}
		}
		else 
		{
			for (int i = 0; i < size; i++)
			{
				// ����������������������
				if (!equal(dataSet[i], cooordinate) && dataSet[i].pos.y <= cooordinate.pos.y)
				{
					dataSetLeft[sizeLeft] = dataSet[i];
					sizeLeft++;
				}
				else if (!equal(dataSet[i], cooordinate) && dataSet[i].pos.y > cooordinate.pos.y)
				{
					dataSetRight[sizeRight] = dataSet[i];
					sizeRight++;
				}
			}
		}

		tree = new Tnode;
		tree->cooordinate = cooordinate;
		tree->split = split;
		tree->left = buildTree(dataSetLeft, sizeLeft, tree->left);
		tree->right = buildTree(dataSetRight, sizeRight, tree->right);

		delete[] dataSetLeft;
		delete[] dataSetRight;

		return tree;
	}


	double getDistance(Node a, Node b) 
	{
		return sqrt((a.pos.x - b.pos.x) * (a.pos.x - b.pos.x) + (a.pos.y - b.pos.y) * (a.pos.y - b.pos.y));
	}


	string searchNearest(Tnode * Kd, Node target) 
	{
		// 1. ���Kd�ǿյģ�����distΪ����󷵻�  
		// 2. ��������ֱ��Ҷ�ӽ��  
		stack<Tnode*> searchPath;
		Tnode* pSearch = Kd;
		Node nearest;
		double dist;

		while (pSearch != NULL)
		{
			// �������·�� 
			searchPath.push(pSearch);

			if (pSearch->split == 0)
			{
				if (target.pos.x <= pSearch->cooordinate.pos.x)
					pSearch = pSearch->left;
				else
					pSearch = pSearch->right;
			}
			else 
			{
				if (target.pos.y <= pSearch->cooordinate.pos.y)
					pSearch = pSearch->left;
				else
					pSearch = pSearch->right;
			}
		}
		// �״β�ѯʱ��ʹ��·������Ҷ�ڵ��е�����ڵ�
		nearest = searchPath.top()->cooordinate;
		searchPath.pop();
		dist = getDistance(nearest, target);

		// 3. ��������·��  
		Tnode* pBack;

		while (searchPath.size() != 0)
		{
			// ȡ��searchPath���һ����㸳��pBack  
			pBack = searchPath.top();
			searchPath.pop();

			// ���pBackΪҶ�ڵ㣬�����һ������
			if (pBack->left == NULL && pBack->right == NULL)
			{
				if (getDistance(nearest, target) > getDistance(pBack->cooordinate, target))
				{
					nearest = pBack->cooordinate;
					dist = getDistance(pBack->cooordinate, target);
				}
			}
			else
			{
				if (pBack->split == 0)
				{
					// �����targetΪ���ģ��뾶Ϊdist��Բ��ָ�ƽ���ཻ�� ��ȥ��һ�ߵ�����ȥ�������ܽ���
					if (fabs(pBack->cooordinate.pos.x - target.pos.x) < dist) 
					{
						if (getDistance(nearest, target) > getDistance(pBack->cooordinate, target))
						{
							nearest = pBack->cooordinate;
							dist = getDistance(pBack->cooordinate, target);
						}
						//  ���targetλ��pBack������������ȥ�������������ܽ���
						if (target.pos.x <= pBack->cooordinate.pos.x) /* */
							pSearch = pBack->right;
						else
							pSearch = pBack->left;
						if (pSearch != NULL)
							// �������·�� 
							searchPath.push(pSearch);
					}
				}
				else 
				{
					// �����targetΪ���ģ��뾶Ϊdist��Բ��ָ�ƽ���ཻ�� ��ȥ��һ�ߵ�����ȥ�������ܽ���
					if (fabs(pBack->cooordinate.pos.y - target.pos.y) < dist)
					{
						if (getDistance(nearest, target) > getDistance(pBack->cooordinate, target))
						{
							nearest = pBack->cooordinate;
							dist = getDistance(pBack->cooordinate, target);
						}
						//  ���targetλ��pBack������������ȥ�������������ܽ���
						if (target.pos.y <= pBack->cooordinate.pos.y)
							pSearch = pBack->right;
						else
							pSearch = pBack->left;
						if (pSearch != NULL)
							// �������·�� 
							searchPath.push(pSearch);
					}
				}
			}
		}

		return nearest.id;
	}
};
