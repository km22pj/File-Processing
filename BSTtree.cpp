#include <iostream>

template <class _Tp> class Node;
template <class _Tp> class BST;


template <class _Tp>
class Node {
public: // Member types
	typedef _Tp				  __key_type;			// typedef : 기존 데이터 타입의 새로운 별칭 생성. _Tp타입을 __key_type이라고 부름
	typedef Node<__key_type>* __node_pointer_type;

public: // Member variables
	__key_type			__key_;
	__node_pointer_type	__left_;
	__node_pointer_type	__right_;
	int	height;	// 트리의 높이 멤버변수 추가
	int size;	// 트리의 크기 멤버변수 추가

public: // Constructor
	Node() : __key_(__key_type()), __left_(nullptr), __right_(nullptr), height(1), size(1) {}		// 매개변수 없으면 __key_를 0으로 초기화. int() => 0
	Node(const __key_type& key) : __key_(key), __left_(nullptr), __right_(nullptr), height(1), size(1) {}

	friend std::ostream& operator<<(std::ostream& os, const __node_pointer_type& np) {
		os << np->__key_;
		return os;
	}
};



template <class _NodePtr>
unsigned __height(_NodePtr __x) {
	if (__x == nullptr) return 0;	// Null이면 0 반환
	return __x->height;				// 아니면 높이 반환
}

template <class _NodePtr>
unsigned __size(_NodePtr __x) {
	if (__x == nullptr) return 0;	// Null이면 0 반환
	return __x->size;				// 아니면 크기 반환
}


template <class _NodePtr>
_NodePtr __tree_min(_NodePtr __x) {
	_NodePtr p = __x;
	if (p == nullptr) return nullptr;	// 해당 노드가 Null 이면 nullptr반환
	while (p->__left_ != nullptr) {		// left 노드가 Null일때까지 내려가 해당 트리의 최소값을 가진 노드를 찾음
		p = p->__left_;
	}
	return p;
}


template <class _NodePtr>
_NodePtr __tree_max(_NodePtr __x) {
	_NodePtr p = __x;
	if (p == nullptr) return nullptr;	// 해당 노드가 Null 이면 nullptr반환
	while (p->__right_ != nullptr) {	// right 노드가 Null일때까지 내려가 해당 트리의 최대값을 가진 노드를 찾음
		p = p->__right_;
	}
	return p;
}



template <class _NodePtr>
void __inorder(_NodePtr __x) {
	if (__x == nullptr) return;				// __x가 nullptr라면 출력하지않고 중단
	std::cout << "<";
	__inorder(__x->__left_);				// 해당 노드의 left 노드를 먼저 방문
	std::cout << " " << __x << " ";			// 해당 노드 키 출력
	__inorder(__x->__right_);				// 해당 노드의 right 노드를 방문
	std::cout << ">";
}


// 새로운 노드 생성하기위해 추가한 함수
template <class _NodePtr, class _Tp>
_NodePtr getNodeBST(){
	_NodePtr newNode = new Node<_Tp>();
	return newNode;
}


// 처음 루트노드를 삽입할때 __root를 참조하여 처음노드를 삽입해주어야 하므로 _NodePtr& 타입 매개변수가 필요
template <class _NodePtr, class _Tp>
std::pair<_NodePtr, bool> __insertBST(_NodePtr &__root, const _Tp& key) { 
	_NodePtr st[1000];								// 지나온 노드들을 저장하는 배열. 스택처럼 사용하며 1000개를 넘지 않음
	int path_len = 0;								// 지나온 노드 개수를 세는 변수
	_NodePtr q = nullptr;							// p의 부모노드를 가리키는 q
	_NodePtr p = __root;							// 루트를 가리키는 p
	
	// 비어있는 트리에 노드를 처음 삽입하는 경우
	if (p == nullptr) {								
		_NodePtr newNode = getNodeBST<_NodePtr, _Tp>();	// 해당 키로 새로운 노드 생성
		newNode->__key_ = key;
		__root = newNode;							// 새로운 노드를 루트에 삽입
		return std::make_pair(__root, true);
	}	

	while (p != nullptr) {							// p가 nullptr가 아닐 때까지 트리를 타고 내려감
		if (key == p->__key_) {
			return std::make_pair(p, false);		// 키가 이미 존재하는 경우 false 반환
		}

		q = p;										// 부모노드를 가리키던 q가 자식노드 p를 가리키도록 함
		st[path_len++] = q;							// 탐색하면서 지나간 노드 q 저장
		if (key < q->__key_) {						// 삽입하려는 키가 q의 키보다 작은 경우
			p = p->__left_;							// p를 p->left로 이동
		}
		else {
			p = p->__right_;						// q의 키보다 큰 경우 p를 p->right로 이동
		}
	}

	// 비어있지 않은 트리에 노드를 삽입할 경우
	_NodePtr newNode = getNodeBST<_NodePtr, _Tp>(); // 해당 키로 새로운 노드 생성
	newNode->__key_ = key;

	if (key < q->__key_) {							// 삽입하려는 키가 q의 키보다 작은 경우
		q->__left_ = newNode;						// q->left에 삽입
	}
	else { 
		q->__right_ = newNode;						// q의 키도다 큰 경우 q->right에 삽입
	}	

	for (int i = path_len - 1; i >= 0; i--) {		// 노드를 삽입할 때 영향을 받은 노드들을 저장해 놓은 st의 아래층부터 순차적으로 접근
		_NodePtr current_node = st[i];				// st에 존재하는 모든 노드의 멤버변수 height와 size 갱신하기 위한 반복문
		int hleft = __height(current_node->__left_);
		int hright = __height(current_node->__right_);
		current_node->height = 1 + (hleft > hright ? hleft : hright); // 왼쪽 오른쪽 높이 중에 더 큰값에 1을 더한 값을 현재 높이로 갱신
		int sleft = __size(current_node->__left_);
		int sright = __size(current_node->__right_);
		current_node->size = 1 + sleft + sright;	// 왼쪽 오른쪽 노드 개수에 1을 더한 값을 현재 사이즈로 갱신
	}
	return std::make_pair(newNode, true);
}



template <class _NodePtr, class _Tp>
_NodePtr __eraseBST(_NodePtr& __root, const _Tp& key) {
	_NodePtr st[1000];								// 지나온 노드들을 저장하는 스택. 1000개를 넘지 않음
	int path_len = 0;								// 지나온 노드 개수를 세는 변수
	_NodePtr q = nullptr;							// p의 부모노드를 가리키는 q
	_NodePtr p = __root;							// 루트를 가리키는 p 

	while (p != nullptr and p->__key_ != key) {		// p가 nullptr가 아니면서 key값이 아닐 때까지 트리를 타고 내려감
		q = p;										// q가 자식노드 p를 가리킴
		st[path_len++] = q;							// 탐색하면서 지나간 노드 저장
		if (key < q->__key_) {						// 삭제하려는 키가 p의 키보다 작은 경우
			p = p->__left_;							// p를 p->left로 이동
		}
		else {
			p = p->__right_;						// p의 키보다 큰 경우 p를 p->right로 이동
		}
	}
	if (p == nullptr) return p;						// p가 nullptr이면 찾지 못했으므로 nullptr를 그대로 반환

	// 1. 삭제할 노드가 루트 노드인 경우
	if (q == nullptr) {
		if (p->__left_ == nullptr) {
			__root = p->__right_;					// 루트의 left 자식이 없을 경우 right 자식을 루트로 지정
		}
		else if (p->__right_ == nullptr) {
			__root = p->__left_;					// 루트의 right 자식이 없을 경우 left 자식을 루트로 지정
		}
		// 루트의 자식이 2개일 경우
		else {
			// 루트의 오른쪽 높이가 더 크거나, 높이가 같지만 오른쪽 노드 수가 더 많은 경우에
			// 오른쪽에서 최솟값을 찾아 루트노드로 대체함
			if (__height(__root->__left_) < __height(__root->__right_) or
				(__height(__root->__left_) == __height(__root->__right_) and
					__size(__root->__left_) < __size(__root->__right_))) {
				_NodePtr tmpNode = __tree_min(__root->__right_);	// 루트의 right에서 최솟값을 tmpNode로 가져옴
				__root->__key_ = tmpNode->__key_;					// tmpNode 키 값을 루트의 키 값으로 교체
				q = p;										// q가 자식노드 p를 가리킴
				st[path_len++] = q;							// 현재 위치의 노드 포인터를 스택에 저장
				p = p->__right_;							// p는 p->right로 이동
				while (p->__left_ != nullptr) {				// 아래층에 있는 최솟값 tmpNode는 삭제함
					q = p;	
					st[path_len++] = q;	
					p = p->__left_;
				}

				if (q->__right_ == p) {						// q->right가 p일 경우. while문이 한번도 실행 안된경우
					q->__right_ = p->__right_;				// q->right에 p의 right 노드를 연결
				}
				else {										// q->left가 p일 경우. while문이 한번이라도 실행된 경우
					q->__left_ = p->__right_;				// q->left에 p의 right 노드를 연결
				}
			}
			// 루트의 왼쪽 높이가 더 크거나, 높이가 같지만 왼쪽 노드 수가 더 많은 경우
			// 왼쪽에서 최댓값을 찾아 루트노드로 대체함
			else {
				_NodePtr tmpNode = __tree_max(__root->__left_);	// 루트의 left 에서 최댓값을 tmpNode로 가져옴
				__root->__key_ = tmpNode->__key_;				// tmpNode 키 값을 루트의 키 값으로 교체
				q = p;										// q가 자식노드 p를 가리킴
				st[path_len++] = q;							// 현재 위치의 노드 포인터를 스택에 저장
				p = p->__left_;								// p는 p->left로 이동
				while (p->__right_ != nullptr) {			// 아래층에 있는 최댓값 tmpNode는 삭제함
					q = p;
					st[path_len++] = q;
					p = p->__right_;
				}

				if (q->__left_ == p) {						// q->left가 p일 경우. while문이 한번도 실행 안된경우
					q->__left_ = p->__left_;				// q->left가 p의 left 노드를 연결
				}
				else {										// q->right가 p일 경우. while문이 한번이라도 실행된 경우
					q->__right_ = p->__left_;				// q->right에 p의 left 노드를 연결
				}
			}
		}
	}

	// 2. 삭제할 노드가 루트노드가 아닐 경우
	else {
		if (p->__left_ == nullptr) {			// p의 자식이 0개 or right 자식만 있고
			if (q->__left_ == p) {				// 삭제할 노드가 q의 left이면
				q->__left_ = p->__right_;		// q->left에 p의 right 노드를 연결
			}
			else {								// 삭제할 노드가 q의 right이면
				q->__right_ = p->__right_;		// q->right에 p의 right 노드를 연결
			}
		}
		else if (p->__right_ == nullptr) {		// p의 자식이 0개 or left 자식만 있고
			if (q->__left_ == p) {				// 삭제할 노드가 q의 left이면
				q->__left_ = p->__left_;		// q->left에 p의 left 노드를 연결
			}
			else {								// 삭제할 노드가 q의 right이면
				q->__right_ = p->__left_;		// q->right에 p의 left 노드를 연결
			}
		}
		// p의 자식이 2개인 경우
		else {
			// p의 오른쪽 높이가 더 크거나, 높이가 같지만 오른쪽 노드 수가 더 많은 경우
			// 오른쪽에서 최솟값을 찾아 루트노드로 대체함
			if (__height(p->__left_) < __height(p->__right_) or
				(__height(p->__left_) == __height(p->__right_) and
					__size(p->__left_) < __size(p->__right_))) {
				_NodePtr tmpNode = __tree_min(p->__right_); // p의 right에서 최솟값을 tmpNode로 가져옴
				p->__key_ = tmpNode->__key_;				// tmpNode 키 값을 삭제할 p의 키 값으로 교체
				q = p;										// q가 자식노드 p를 가리킴
				st[path_len++] = q;							// 현재 위치의 노드 포인터를 스택에 저장
				p = p->__right_;							// p는 p->right으로 이동
				while (p->__left_ != nullptr) {				// 아래층에 있는 최솟값 tmpNode는 삭제함
					q = p;
					st[path_len++] = q;
					p = p->__left_;
				}

				if (q->__right_ == p) {						// q->right가 p일 경우. while문이 한번도 실행 안된경우
					q->__right_ = p->__right_;				// q->right에 p의 right 노드를 연결
				}
				else {										// q->left가 p일 경우. while문이 한번이라도 실행된 경우
					q->__left_ = p->__right_;				// q->left에 p의 right 노드를 연결
				}
			}
			// 루트의 왼쪽 높이가 더 크거나, 높이가 같지만 왼쪽 노드 수가 더 많은 경우
			// 왼쪽에서 최댓값을 찾아 루트노드로 대체함
			else {
				_NodePtr tmpNode = __tree_max(p->__left_);	// p의 left에서 최댓값을 tmpNode로 가져옴
				p->__key_ = tmpNode->__key_;				// tmpNode 키 값을 삭제할 p의 키 값으로 교체
				q = p;										// q가 자식노드 p를 가리킴
				st[path_len++] = q;							// 현재 위치의 노드 포인터를 스택에 저장
				p = p->__left_;								// p는 p->left으로 이동
				while (p->__right_ != nullptr) {			// 아래층에 있는 최댓값 tmpNode는 삭제함
					q = p;
					st[path_len++] = q;
					p = p->__right_;
				}

				if (q->__left_ == p) {						// q->left가 p일 경우. while문이 한번도 실행 안된경우
					q->__left_ = p->__left_;				// q->left에 p의 left 노드를 연결
				}
				else {										// q->right가 p일 경우. while문이 한번이라도 실행된 경우
					q->__right_ = p->__left_;				// q->right에 p의 left 노드를 연결
				}
			}
		}

	}
	for (int i = path_len - 1; i >= 0; i--) {				// 노드를 삭제할때 영향을 받는 노드들을 저장해 놓은 st의 아래층부터 순차적으로 접근
		_NodePtr current_node = st[i];						// st에 존재하는 모든 노드의 멤버변수 height와 size 갱신
		int hleft = __height(current_node->__left_);
		int hright = __height(current_node->__right_);
		current_node->height = 1 + (hleft > hright ? hleft : hright); // 왼쪽 오른쪽 높이 중에 더 큰값에 1을 더한 값을 현재 높이로 갱신
		int sleft = __size(current_node->__left_);
		int sright = __size(current_node->__right_);
		current_node->size = 1 + sleft + sright;			// 왼쪽 오른쪽 노드 개수에 1을 더한 값을 현재 사이즈로 갱신
	}
	return p;	// 삭제할 노드포인터 p 반환
}


// 비어있는 공간을 가리키는 Dangling pointer를 방지하기 위해 __x를 참조 타입으로 설정
template <class _NodePtr>
void __clear(_NodePtr& __x) {
	// 후위 순회로 노드의 메모리를 해제
	if (__x == nullptr) {
		return;
	}
	__clear(__x->__left_);
	__clear(__x->__right_);
	delete __x;
	__x = nullptr;
}



template <class _Tp>
class BST {
public: // Member types
	typedef _Tp						key_type;
	typedef std::size_t				size_type;		// unsigned integer
	typedef Node<key_type>* pointer;
	typedef const Node<key_type>* const_pointer;

private: // Member variables
	pointer	__root_;

public: // Constructor
	BST() : __root_(nullptr) {}

public: // Capacity
	size_type height() const {
		return __height(__root_);	//  __height함수 사용
	}

	size_type size() const {
		return __size(__root_);		// __size함수 사용
	}

public: // Lookup
	void inorder() const {
		__inorder(__root_);			// __inorder함수 사용
		std::cout << std::endl;
	}

public: // Modifier
	std::pair<const_pointer, bool> insert(const key_type& key) {
		return __insertBST(__root_, key);	// __insertBST함수 사용
	}

	const_pointer erase(const key_type& key) {
		pointer __r = __eraseBST(__root_, key);		// __eraseBST함수 사용
		delete __r;		// 메모리를 해제해도 __r은 nullptr가 아니라 원래 노드의 주소를 여전히 가리키고 있음
		return __r;		// 삭제 되어도 nullptr가 아니므로 삭제 실패조건에 걸리지 않음
	}

	void clear() {
		__clear(__root_);	// __clear함수 사용
	}
};


int main() {
	BST<int>	tree;
	char		command;
	int			key;

	while (std::cin >> command >> key) {
		switch ((int)command) {
		case (int)'i':
			if (tree.insert(key).second == false) {	// 키가 이미 존재하여 삽입 실패했을 경우
				std::cerr << "i " << key << ": The key already exists" << std::endl;
				continue;
			}
			break;
		case (int)'d':
			if (tree.erase(key) == nullptr) {		// 삭제할 키가 존재하지 않을 경우
				std::cerr << "d " << key << ": The key does not exist" << std::endl;
				continue;
			}
			break;
		default:
			std::cerr << "Invalid command: " << command << std::endl;
			return (1);
			break;
		}
		tree.inorder();	// 중위순회로 트리 출력
	}

	// 프로그램 종료 전, 메모리 누수가 발생하지 않도록 할당받은 메모리 해제
	tree.clear();
	return (0);
}