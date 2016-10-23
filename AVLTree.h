#pragma once

template <class K,class V>
struct AVLTreeNode
{
	//key value
	K _key;
	V _value;
	// 左孩子 右孩子 父节点
	AVLTreeNode<K, V>* _left;
	AVLTreeNode<K, V>* _right;
	AVLTreeNode<K, V>* _parent;
	int _bf;														//平衡因子

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
		//1.空树
		if (_root == NULL)
		{
			_root = new Node(key, value);
			return true;
		}
		//查找位置

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
				//已经存在，则返回。
				return false;
			}
		}
		//插入节点                                                             
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
			
		//更新平衡因子,调整树
		cur = tmp;
		while (parent)
		{
			if (cur == parent->_right)
				++parent->_bf;
			else
				--parent->_bf;
			//1_bf == 0   直接退出
			//2 _bf == 1/-1 则上层需要调整，继续向上调整
			//2_bf == 2/-2  需要旋转调整树    
			if (parent->_bf == 0)
			{
				break;
			}
			else if ((parent->_bf == 1) || (parent->_bf == -1))
			{
				cur = parent;
				parent = cur->_parent;
			}
			else                                                       //平衡因子等于2或者-2，需要旋转
			{
				if (parent->_bf == -2 )						
				{
					if (cur->_bf == -1)							//先左后右   旋转
						_RotateR(parent);
					else
						_RotateLR(parent);
				}
				else
				{
					if (cur->_bf == 1)							//先右后左  旋转
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
			cout << "平衡因子有问题:";
			cout<< root->_key <<"->"<<root->_bf<< endl;
		}
			
		return _IsBlanceTree(root->_left) && _IsBlanceTree(root->_right);
	}
	void _RotateL(Node* parent)
	{
		//1提升右孩子
		//2右孩子的  左 子树交给父节点的 右
		//3父节点成为右孩子的左
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
		//1 提升左孩子
		//2左孩子的右子树变为父节点的左
		//3父节点变为左孩子的右
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

		//根据subLR的平衡因子调整其他节点的平衡因子
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

		//根据subRL的平衡因子调整其他节点的平衡因子
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