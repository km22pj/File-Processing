
#include <iostream>
#include <utility> // std::pair
#include <algorithm> // std::max
#include <cassert> // assert

template <class _Tp> class Node;
template <class _Tp> class AVL;

typedef enum {
	NO = 0,
	LL = 1,
	LR = 2,
	RL = 3,
	RR = 4,
} ROTATION;


template <class _Tp>
class Node {
public: // Member types
	typedef _Tp					__key_type;
	typedef Node<__key_type>* __node_pointer_type;

public: // Member variables
	__key_type			__key_;
	__node_pointer_type	__left_;
	__node_pointer_type	__right_;
	// 추가 멤버 변수
	int height;		// 트리의 높이
	int size;		// 트리의 노드 개수
	int bf;			// 트리의 균형인수

public: // Constructor
	Node() : __key_(__key_type()), __left_(nullptr), __right_(nullptr), height(1), size(1), bf(0) {}
	Node(const __key_type& key) : __key_(key), __left_(nullptr), __right_(nullptr), height(1), size(1), bf(0) {}

	friend std::ostream& operator<<(std::ostream& os, const __node_pointer_type& np) {
		os << np->__key_;
		return os;
	}
};


template <class _NodePtr>
unsigned __height(_NodePtr __x) {
	return __x == nullptr ? 0 : 1 + std::max(__height(__x->__left_), __height(__x->__right_));
}

template <class _NodePtr>
unsigned __size(_NodePtr __x) {
	return __x == nullptr ? 0 : 1 + __size(__x->__left_) + __size(__x->__right_);
}


template <class _NodePtr>
_NodePtr __tree_min(_NodePtr __x) {
	assert(__x != nullptr);
	return __x->__left_ == nullptr ? __x : __tree_min(__x->__left_);
}


template <class _NodePtr>
_NodePtr __tree_max(_NodePtr __x) {
	assert(__x != nullptr);
	return __x->__right_ == nullptr ? __x : __tree_max(__x->__right_);
}


// 중위 순회 함수
template <class _NodePtr>
void __inorder(_NodePtr __x) {
	if (__x == nullptr)	return;		// __x가 nullptr면 출력하지않고 중단
	std::cout << "<";
	__inorder(__x->__left_);		// left 트리를 먼저 방문
	std::cout << " " << __x << " ";	// 해당 키 출력
	__inorder(__x->__right_);		// right 트리 방문
	std::cout << ">";
}




	// RR일때 사용하는 rotate_tree
template <class _NodePtr>
void __tree_left_rotate(_NodePtr& __root, _NodePtr __x) {		// __root는 __x의 부모노드, __X는 불균형 노드
	_NodePtr tmp = __x->__right_;								// __x-> right를 tmp로 설정
	if (__root == __x) {										// __x가 루트노드일 경우
		__root = __root->__right_;								// __root를 먼저 right로 옮겨놓음
		__x->__right_ = tmp->__left_;							// __x 의 right는 tmp의 left를 가리킴
		tmp->__left_ = __x;										// tmp의 left는 __x를 가리킴
	}	
	else {
		__x->__right_ = tmp->__left_;							// __x 의 right는 tmp의 left를 가리킴
		tmp->__left_ = __x;										// tmp의 left는 __x를 가리킴
		if (__root->__left_ == __x)	__root->__left_ = tmp;		// __root->left를 __x에서 tmp로 바꿈
		else __root->__right_ = tmp;							// __root->right를 __x에서 tmp로 바꿈
	}
	__x->height = 1 + std::max(__height(__x->__left_), __height(__x->__right_));	// __x의 높이 갱신
	tmp->height = 1 + std::max(__height(tmp->__left_), __height(tmp->__right_));	// tmp의 높이 갱신
	__x->size = 1 + __size(__x->__left_) + __size(__x->__right_);					// __x의 노드 개수 갱신
	tmp->size = 1 + __size(tmp->__left_) + __size(tmp->__right_);					// tmp의 노드 개수 갱신
	__x->bf = __height(__x->__left_) - __height(__x->__right_);						// __x의 bf 갱신
	tmp->bf = __height(tmp->__left_) - __height(tmp->__right_);						// tmo의 bf 갱신
}

	// LL일때 사용하는 rotate_tree
template <class _NodePtr>
void __tree_right_rotate(_NodePtr& __root, _NodePtr __x) {
	_NodePtr tmp = __x->__left_;								// __x-> left를 tmp로 설정
	if (__root == __x) {										// __x가 루트노드일 경우
		__root = __root->__left_;								// __root를 먼저 left로 옮겨놓음
		__x->__left_ = tmp->__right_;							// __x의 left는 tmp의 right를 가리킴 
		tmp->__right_ = __x;									// tmp의 right는 __x를 가리킴
	}
	else {														// __x가 루트노드가 아닐 경우
		__x->__left_ = tmp->__right_;							// __x-> left는 tmp의 right를 가리킴
		tmp->__right_ = __x;									// tmp의 right는 __x를 가리킴
		if (__root->__left_ == __x)	__root->__left_ = tmp;		// __root->left를 __x에서 tmp로 바꿈
		else __root->__right_ = tmp;							// __root->right를 __x에서 tmp로 바꿈
	}
	
	__x->height = 1 + std::max(__height(__x->__left_), __height(__x->__right_));	// __x의 높이 갱신
	tmp->height = 1 + std::max(__height(tmp->__left_), __height(tmp->__right_));	// tmp의 높이 갱신
	__x->size = 1 + __size(__x->__left_) + __size(__x->__right_);					// __x의 노드 개수 갱신
	tmp->size = 1 + __size(tmp->__left_) + __size(tmp->__right_);					// tmp의 노드 개수 갱신
	__x->bf = __height(__x->__left_) - __height(__x->__right_);						// __x의 bf 갱신
	tmp->bf = __height(tmp->__left_) - __height(tmp->__right_);						// tmo의 bf 갱신
}


// 로테이션 종류 파악하는 checkBalance() 함수
template <class _NodePtr>
ROTATION checkBalance(_NodePtr __root) {
	ROTATION rotationtype = NO;
	if(1 < __root->bf){												// bf가 1보다 큰경우 왼쪽 트리가 높음
		if (0 <= __root->__left_->bf) rotationtype = LL;			// 왼쪽 노드의 bf가 0 이상이면 LL
		else rotationtype = LR;										// 왼쪽 노드의 bf가 0 미만이면 LR
	}
	else if(__root->bf<-1){											// bf가 -1보다 작은경우 오른쪽 트리가 높음
		if (__root->__right_->bf <= 0) rotationtype = RR;			// 오른쪽 노드의 bf가 0 이하이면 RR
		else rotationtype = RL;										// 오른쪽 노드의 bf가 0 미만이면 RL
	}
	return rotationtype;
}


// 파악한 로테이션 종류로 회전을 시켜 균형을 맞추는 rotateTree() 함수
template <class _NodePtr>
void rotateTree(_NodePtr& __root, _NodePtr __x) {
	ROTATION rotationtype = checkBalance(__x);						// 로테이션 종류 저장
	if (rotationtype == NO) {
		return;
	}
	else if (rotationtype == LL) {
		__tree_right_rotate(__root, __x);							// LL이면 right rotation 수행
	}
	else if (rotationtype == RR) {
		__tree_left_rotate(__root, __x);							// RR이면 left rotation 수행
	}
	else if (rotationtype == LR) {
		__tree_left_rotate(__x, __x->__left_);						// LR이면 x->left를 left rotation 먼저 수행후
		__tree_right_rotate(__root, __x);							// right rotation 수행
	}
	else if (rotationtype == RL) {
		__tree_right_rotate(__x, __x->__right_);					// RL이면 x->right를 right rotation 먼저 수행후
		__tree_left_rotate(__root, __x);							// left rotation 수행
	}
}



// insertBST
template <class _NodePtr, class _Tp>		// 처음 삽입할때 루트가 직접 가리켜야 하므로 &타입 __root를 사용
std::pair<_NodePtr, bool> __insertBST(_NodePtr& __root, const _Tp& key, _NodePtr* st, int& path_len) { 
	_NodePtr q = nullptr;
	_NodePtr p = __root;


	// 비어있는 트리에 노드를 처음 삽입하는 경우
	if (p == nullptr) {
		_NodePtr newNode = new Node<_Tp>(key);	// key로 새로운 노드 생성
		__root = newNode;						// 새로운 노드를 루트에 삽입
		return std::make_pair(__root, true);	
	}

	// 비어있는 트리가 아닐 경우
	while (p != nullptr) {						// p가 nulltpr가 아닐 때까지 트리를 타고 내려감
		if (key == p->__key_) {
			return std::make_pair(p, false);	// key가 이미 존재하는 경우 false
		}
		q = p;
		st[path_len++] = q;						// 지나간 노드 q를 스택에 저장
		if (key < q->__key_) p = p->__left_;	// key가 q값보다 작으면 p를 p->left로 이동
		else p = p->__right_;					// key가 q값보다 크면 p를 p->right로 이동
	}

	_NodePtr newNode = new Node<_Tp>(key);		// key로 새로운 노드 생성
	if (key < q->__key_) q->__left_ = newNode;	// key가 q값보다 작으면 q의 left에 삽입
	else q->__right_ = newNode;					// key가 q값보다 크면 q의 right에 삽입

	return std::make_pair(newNode, true);

}


// insertAVL 삽입 알고리즘
template <class _NodePtr, class _Tp>
std::pair<_NodePtr, bool> __insertAVL(_NodePtr& __root, const _Tp& key) {

	// 지나온 경로의 노드들을 저장하는 스택과 길이을 지역변수로 설정
	_NodePtr st[20];							// AVL은 높이를 조정하므로 트리 높이가 20이상으로 높아지지 않음
	int path_len = 0;							// 지나온 경로 길이

	_NodePtr newNode;							// 삽입할 노드
	bool flag = 0;								// 삽입 성공 여부를 알려주는 flag. 0으로 초기화
	std::pair<_NodePtr, bool> result = __insertBST(__root, key, st, path_len);
	newNode = result.first;
	flag = result.second;
	if (flag == false) return std::make_pair(newNode, flag);// flag가 flase면 삽입 실패이므로 반환함

	for (int i = path_len - 1; i >= 0; i--) {				// 마지막에 삽입한 노드부터 차례대로 멤버 변수를 갱신함
		_NodePtr current_node = st[i];
		int hleft = __height(current_node->__left_);
		int hright = __height(current_node->__right_);
		current_node->height = 1 + std::max(hleft, hright);	// 왼쪽, 오른쪽 서브트리 중 더 큰값에 1을 더하여 갱신
		int sleft = __size(current_node->__left_);
		int sright = __size(current_node->__right_);
		current_node->size = 1 + sleft + sright;			// 왼쪽, 오른쪽 서브트리 노드 개수에 1을 더하여 갱신
		current_node->bf = hleft - hright;					// 균형인수 bf는 왼쪽 높이 - 오른쪽 높이로 갱신

		if (current_node->bf < -1 || current_node->bf > 1) {// 균형인수가 깨졌을 경우
			if (i == 0) rotateTree(__root, current_node);	// 현재 노드가 루트인 경우, 부모노드에 __root를 넘겨서 직접 수정함
			else {
				_NodePtr parent_node = st[i - 1];			// 루트가 아니면 부모노드는 st[i-1]
				rotateTree(parent_node, current_node);		// 불균형 노드는 current_node, 부모노드는 parent_node
			}
		}
	}
	return std::make_pair(newNode, flag);
}



// eraseBST
template <class _NodePtr, class _Tp>
_NodePtr __eraseBST(_NodePtr& __root, const _Tp& key, _NodePtr* st, int& path_len) {
	_NodePtr q = nullptr;
	_NodePtr p = __root;

	while (p != nullptr and p->__key_ != key) {	// 현재 트리에서 key값을 찾음
		q = p;
		st[path_len++] = q;
		if (key < q->__key_) p = p->__left_;
		else p = p->__right_;
	}
	if (p == nullptr) return p;					// key값을 찾지못하면 p가 nullptr. 그대로 반환
	
	// 삭제할 노드가 루트 노드인 경우
	if (q == nullptr) {
		if (p->__left_ == nullptr) { __root = p->__right_; }		// 루트의 left가 nullptr이면 right를 루트로 지정
		else if (p->__right_ == nullptr) { __root = p->__left_; }	// 루트의 right가 nullptr이면 left를 루트로 지정
		else {														// 루트의 자식이 2개인 경우
			// 루트의 오른쪽 트리 높이가 높거나, 높이가 같지만 오른쪽 노드 수가 더 많은 경우
			if (__height(__root->__left_) < __height(__root->__right_) or
				(__height(__root->__left_) == __height(__root->__right_) and
					__size(__root->__left_) < __size(__root->__right_))) {
				_NodePtr tmpNode = __tree_min(__root->__right_);	// 루트의 right에서 최솟값을 tmpNode로 설정
				__root->__key_ = tmpNode->__key_;					// 루트의 key를 tmpNode의 key로 교체
				q = p;												// q가 p를 가리킴
				st[path_len++] = q;									// 지나온 노드 스택에 저장
				p = p->__right_;									// p는 p->right로 이동
				while (p->__left_ != nullptr) {						// 최솟값 tmpNode 탐색
					q = p;
					st[path_len++] = q;								// 탐색하면서 지나온 경로 저장
					p = p->__left_;
				}
				if (q->__right_ == p)q->__right_ = p->__right_;		// while이 한번도 실행 안된경우 q의 right에 p의 right연결
				else q->__left_ = p->__right_;						// q의 left에 p의 right에 연결
			}

			// 루트의 왼쪽 트리 높이가 높거나, 높이가 같지만 왼쪽 노드 수가 더 많은 경우
			else {
				_NodePtr tmpNode = __tree_max(__root->__left_);		// 루트의 left에서 최댓값을 tmpNode로 설정
				__root->__key_ = tmpNode->__key_;					// 루트의 key를 tmpNode의 key로 교체
				q = p;												// q가 p를 가리킴
				st[path_len++] = q;									// 지나온 노드 스택에 저장
				p = p->__left_;										// p는 p->left로 이동
				while (p->__right_ != nullptr) {					// 최댓값 tmpNode는 탐색
					q = p;
					st[path_len++] = q;								// 탐색하면서 지나온 경로 저장
					p = p->__right_;
				}

				if (q->__left_ == p)q->__left_ = p->__left_;		// while문이 한번도 실행 안된경우 q의 left에 p의 left연결
				else q->__right_ = p->__left_;						// q의 right에 p의 left 노드를 연결	
			}
		}
	}

	// 삭제할 노드가 루트노드가 아닐 경우
	else {
		if (p->__left_ == nullptr) {								// p의 left노드가 없고
			if (q->__left_ == p) q->__left_ = p->__right_;			// 삭제할 노드가 q의 left이면 q->left에 p의 right 노드를 연결
			else q->__right_ = p->__right_; 						// 삭제할 노드가 q의 right이면 q->right에 p의 right 노드를 연결
		}
		else if (p->__right_ == nullptr) {							// p의 right노드가 없고
			if (q->__left_ == p) q->__left_ = p->__left_;			// 삭제할 노드가 q의 left이면 q->left에 p의 left 노드를 연결
			else q->__right_ = p->__left_;							// 삭제할 노드가 q의 right이면 q->right에 p의 left 노드를 연결			
		}
		// p의 자식이 2개인 경우
		else {
			// p의 오른쪽 트리가 더 높거나, 높이가 같지만 오른쪽 노드 수가 더 많은 경우
			if (__height(p->__left_) < __height(p->__right_) or
				(__height(p->__left_) == __height(p->__right_) and
					__size(p->__left_) < __size(p->__right_))) {
				_NodePtr tmpNode = __tree_min(p->__right_);			// p의 right에서 최솟값을 tmpNode로 설정
				p->__key_ = tmpNode->__key_;						// p의 key를 tmpNode의 key로 교체
				q = p;												// q가 p를 가리킴
				st[path_len++] = q;									// 지나온 노드 스택에 저장
				p = p->__right_;									// p는 p->right으로 이동
				while (p->__left_ != nullptr) {						// 최솟값 tmpNode는 탐색
					q = p;
					st[path_len++] = q;								// 탐색하면서 지나온 경로 저장
					p = p->__left_;
				}

				if (q->__right_ == p) q->__right_ = p->__right_;	// while문이 한번도 실행 안된경우 q의 right에 p의 right 연결
				else q->__left_ = p->__right_;						// q의 left에 p의 right 노드를 연결	
			}
			// p의 왼쪽 트리가 더 높거나, 높이가 같지만 왼쪽 노드 수가 더 많은 경우
			else {
				_NodePtr tmpNode = __tree_max(p->__left_);			// p의 left에서 최댓값을 tmpNode로 설정
				p->__key_ = tmpNode->__key_;						// p의 key를 tmpNode의 key로 교체
				q = p;												// q가 p를 가리킴
				st[path_len++] = q;									// 지나온 노드 스택에 저장
				p = p->__left_;										// p는 p->left으로 이동
				while (p->__right_ != nullptr) {					// 최댓값 tmpNode는 탐색
					q = p;
					st[path_len++] = q;								// 탐색하면서 지나온 경로 저장
					p = p->__right_;
				}
				if (q->__left_ == p) q->__left_ = p->__left_;		// while문이 한번도 실행 안된경우 q의left에 p의 left 연결
				else q->__right_ = p->__left_;						// q의 right에 p의 left 노드를 연결
			}
		}
	}
	return p;
}


// eraseAVL 삭제 알고리즘
template <class _NodePtr, class _Tp>
_NodePtr __eraseAVL(_NodePtr& __root, const _Tp& key) {

	// 지나온 경로의 노드들을 저장하는 스택과 길이을 지역변수로 설정
	_NodePtr st[20];							// AVL은 높이를 조정하므로 트리 높이가 20이상으로 높아지지 않음
	int path_len = 0;							// 지나온 경로 길이

	_NodePtr newNode = __eraseBST(__root, key, st, path_len);
	if (newNode == nullptr) return newNode;		// nullptr가 반환되면 삭제 실패

	for (int i = path_len - 1; i >= 0; i--) {	// 삭제하면서 지나온 경로의 노드를 차례대로 호출하여 멤버 변수를 갱신함
		_NodePtr current_node = st[i];
		int hleft = __height(current_node->__left_);
		int hright = __height(current_node->__right_);
		current_node->height = 1 + std::max(hleft, hright);	// 왼쪽, 오른쪽 서브트리 중 더 큰값에 1을 더하여 갱신
		int sleft = __size(current_node->__left_);
		int sright = __size(current_node->__right_);
		current_node->size = 1 + sleft + sright;			// 왼쪽, 오른쪽 서브트리 노드 개수에 1을 더하여 갱신
		current_node->bf = hleft - hright;					// 균형인수 bf는 왼쪽 높이 - 오른쪽 높이로 갱신

		if (current_node->bf < -1 || current_node->bf > 1) {// 균형인수가 깨졌을 경우
			if (i == 0) rotateTree(__root, current_node);	// 현재 노드가 루트, 부모노드 __root를 넘겨서 직접 수정함
			else {
				_NodePtr parent_node = st[i - 1];			// 루트가 아니면 부모노드는 st[i-1]
				rotateTree(parent_node, current_node);		// 불균형 노드는 current_node, 부모노드는 parent_node
			}
		}
	}
	return newNode;

}

// Dangling pointer를 방지하기 위해 __x를 참조 타입설정
template <class _NodePtr>
void __clear(_NodePtr& __x) {
	if (__x == nullptr) {
		return;
	}
	__clear(__x->__left_);
	__clear(__x->__right_);
	delete __x;
	__x = nullptr;
}


template <class _Tp>
class AVL {
public: // Member types
	typedef _Tp						key_type;
	typedef std::size_t				size_type;
	typedef Node<key_type>* pointer;
	typedef const Node<key_type>* const_pointer;

private: // Member variables
	pointer	__root_;

public: // Constructor
	AVL() : __root_(nullptr) {}

public: // Capacity
	size_type height() const {
		return __height(__root_);
	}

	size_type size() const {
		return __size(__root_);
	}

public: // Lookup
	void inorder() const {
		__inorder(__root_);
		std::cout << std::endl;
	}

public: // Modifier
	std::pair<const_pointer, bool> insert(const key_type& key) {
		return __insertAVL(__root_, key);
	}

	const_pointer erase(const key_type& key) {
		pointer __r = __eraseAVL(__root_, key);
		delete __r;
		return __r;
	}

	void clear() {
		__clear(__root_);
	}
};



int main() {
	AVL<int>	tree;
	char		command;
	int			key;

	while (std::cin >> command >> key) {
		switch ((int)command) {
		case (int)'i':
			if (tree.insert(key).second == false) {
				std::cerr << "i " << key << ": The key already exists" << std::endl;
				continue;
			}
			break;
		case (int)'d':
			if (tree.erase(key) == nullptr) {
				std::cerr << "d " << key << ": The key does not exist" << std::endl;
				continue;
			}
			break;
		default:
			std::cerr << "Invalid command: " << command << std::endl;
			return (1);
		}
		tree.inorder();	// 중위순회로 트리 출력
	}

	// 프로그램 종료 전, 메모리 누수가 발생하지 않도록 할당받은 메모리를 해제
	tree.clear();	
	return (0);
}