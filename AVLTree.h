#pragma once

template <class K,class V>
struct AVLTreeNode
{
	//key value
	K _key;
	V _value;
	// ���� �Һ��� ���ڵ�
	AVLTreeNode<K, V>* _left;
	AVLTreeNode<K, V>* _right;
	AVLTreeNode<K, V>* _parent;
	int _bf;														//ƽ������

	AVLTreeNode(const K& key, const V& value)
		:_key(key)
		,_value(value)
		,_left(NULL)
		,_right(NULL)
		,_parent(NULL)
		,_bf(0)
	{}
};
template <class K,class V>
class AVLTree
{
	typedef AVLTreeNode<K, V> Node;
public:
	AVLTree()
		:_root(NULL)
	{}
	~AVLTree()
	{
		_Destory(_root);
		_root == NULL;
	}
public: 
	bool Insert(const K& key, const V& value)
	{
		//1.����
		if (_root == NULL)
		{
			_root = new Node(key, value);
			return true;
		}
		//����λ��

		Node* parent = NULL;
		Node* cur = _root;
		
		while (cur)
		{
			if (cur->_key < key)
			{
				parent = cur;
				cur = cur->_right;
			}
			else if (cur->_key > key)
			{
				parent = cur;
				cur = cur->_left;
			}
			else
			{
				//�Ѿ����ڣ��򷵻ء�
				return false;
			}
		}
		//����ڵ�                                                             
		Node* tmp = new Node(key, value);

		if (parent->_key < key)
		{
			parent->_right = tmp;
			tmp->_parent = parent;
		}
		else 
		{
			parent->_left = tmp;
			tmp->_parent = parent;
		}
		
			//parent->_left = new Node(key, value);
			
		//����ƽ������,������
		cur = tmp;
		while (parent)
		{
			if (cur == parent->_right)
				++parent->_bf;
			else
				--parent->_bf;
			//1_bf == 0   ֱ���˳�
			//2 _bf == 1/-1 ���ϲ���Ҫ�������������ϵ���
			//2_bf == 2/-2  ��Ҫ��ת������    
			if (parent->_bf == 0)
			{
				break;
			}
			else if ((parent->_bf == 1) || (parent->_bf == -1))
			{
				cur = parent;
				parent = cur->_parent;
			}
			else                                                       //ƽ�����ӵ���2����-2����Ҫ��ת
			{
				if (parent->_bf == -2 )						
				{
					if (cur->_bf == -1)							//�������   ��ת
						_RotateR(parent);
					else
						_RotateLR(parent);
				}
				else
				{
					if (cur->_bf == 1)							//���Һ���  ��ת
						_RotateL(parent);
					else
						_RotateRL(parent);
				}
				break;
			}
		}
	}
	Node* Find(const K& key)
	{
		Node* cur = _root;
		while (cur)
		{
			if (cur->_key < key)
				cur = cur->_right;
			else if (cur->_key > key)
				cur = cur->_left;
			else
				return cur;
		}
		return NULL;
	}
	void Inorder()
	{
		_Inorder(_root);
		cout << endl;
	}
	bool IsBlanceTree()
	{
		return _IsBlanceTree(_root) < 2;
	}
protected:
	void _Inorder(Node* root)
	{
		if (root == NULL)
			return;

		_Inorder(root->_left);
		cout << root->_key << " ";
		_Inorder(root->_right);

	}
	int _Height(Node* root)
	{
		if (root == NULL)
			return 0;
		int left = _Height(root->_left) + 1;
		int right = _Height(root->_right) + 1;

		return left > right ? left : right;
	}
	int _IsBlanceTree(Node* root)
	{
		if (root == NULL)
			return 0;
		int left = _Height(root->_left);
		int right = _Height(root->_right);

		if (right - left != root->_bf && abs(right - left) > 1)
		{
			cout << "ƽ������������:";
			cout<< root->_key <<"->"<<root->_bf<< endl;
		}
			
		return _IsBlanceTree(root->_left) && _IsBlanceTree(root->_right);
	}
	void _RotateL(Node* parent)
	{
		//1�����Һ���
		//2�Һ��ӵ�  �� �����������ڵ�� ��
		//3���ڵ��Ϊ�Һ��ӵ���
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		
		parent->_right = subRL;
		if (subRL)
			subRL->_parent = parent;
		subR->_left = parent;

		Node*  ppNode = parent->_parent;
		parent->_parent = subR;

		if (ppNode == NULL)
		{
			_root = subR;
			subR->_parent = NULL;
		}
		else
		{
			if (parent== ppNode->_left)
				ppNode->_left = subR;
			else
				ppNode->_right = subR;
		}
		subR->_parent = ppNode;
		subR->_bf = parent->_bf = 0;
	}
	void _RotateR(Node* parent)
	
	{
		//1 ��������
		//2���ӵ���������Ϊ���ڵ����
		//3���ڵ��Ϊ���ӵ���
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
		parent->_left = subLR;
		
		if (subLR)
			subLR->_parent = parent;
		subL->_right = parent;

		Node* ppNode = parent->_parent;
		parent->_parent = subL;

		if (ppNode == NULL)
		{
			_root = subL;
			subL->_parent = NULL;
		}
		else
		{
			if (parent == ppNode->_right)
				ppNode->_right = subL;
			else
				ppNode->_left = subL;
		}
		subL->_parent = ppNode;
		subL->_bf = parent->_bf = 0;
	}
	void _RotateLR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
		int bf = subLR->_bf;

		_RotateL(parent->_left);
		_RotateR(parent);

		//����subLR��ƽ�����ӵ��������ڵ��ƽ������
		if (bf == -1)
		{
			subL->_bf = 0;
			parent->_bf = 1;
		}
		else if (bf == 1)
		{
			subL->_bf = -1;
			parent->_bf = 0;
		}
	}
	void _RotateRL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		int bf = subRL->_bf;

		_RotateR(parent->_right);
		_RotateL(parent);

		//����subRL��ƽ�����ӵ��������ڵ��ƽ������
		if (bf == 1)
		{
			subR->_bf = 0;
			parent->_bf = -1;
		}
		else if (bf == -1)
		{
			subR->_bf = 1;
			parent->_bf = 0;
		}
	}
	void _Destory(Node* root)
	{
		if (root == NULL)
			return;
		_Destory(root->_left);
		_Destory(root->_right);

		delete root;
	}
protected:
	Node* _root;
};

void Test()
{
	int a[] = { 5,3,4,1,7,8,2,6,0,9 };
	AVLTree <int, int>  t;
	for (size_t i = 0; i <( sizeof(a) / sizeof(a[0])); ++i)
	{
		t.Insert(a[i], i);
	}

	t.Inorder();
	t.IsBlanceTree();
}
void Test1()
{
	int a[] = { 4, 2, 6, 1, 3, 5, 15, 7, 16, 14 };
	AVLTree<int, int> t;
	for (size_t i = 0; i < (sizeof(a) / sizeof(a[0])); ++i)
	{
		t.Insert(a[i], i);
	}
	t.Inorder();
	t.IsBlanceTree();
}