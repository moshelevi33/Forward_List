//
// Created by Moshelevi on 9/1/2020.
//
#ifndef LIST_SINGLYLINKEDLIST_H
#define LIST_SINGLYLINKEDLIST_H

//TODO: 1)implement get_alloc
//TODO: 2)Implement all constructors(and some with allocator which i need to learn)
////TODO: 3)Iterators function and const Iteartor

//This class implements the forward_list API -http://www.cplusplus.com/reference/forward_list/forward_list/.
//Method that ends with Rec, means that the method using recursion.
//There are method that do the same thing, but one is recursive and one is linear, did it for fun!.


#include <utility>
#include <bits/allocator.h>
#include <iterator>
template <typename  T , class A = std::allocator<T>>
class SinglyLinkedList;
//In the range version (1), the new contents are elements constructed
// from each of the elements in the range between first and last, in the same order.
//there are Overloads of assign, look in the implementaion
template <class InputIterator>
void Assign (InputIterator first, InputIterator last);
//return the number of elements in the list



int Size();
//element: A copy of the element(using the copy constructor) will be inserted in the start of the list.
template <typename  T>
void PushFront(const T& element) ;

//element: Rvalue. this is good for PushFront(A()), in the normal pushback we copy the element using the copy constructor
//here we dont have to copy a temporary object,for not copy,T needs to have move constructor.
template <typename  T>
void  PushFront(T&& element) ;

//Construct the object in the function using the object constructors and using the same order of Args.
//Args: params to T constructor.
//inserting the object in the start of the list
template <class... Args>
void EmplaceFront (Args&&... args);

//Removing the first element
//Undefined behavior on empty List.
void PopFront();

//element: the element that will be inserted to the list
//insert the given element to the end of the list.
template <typename  T>
void PushBack(const T& element) ;

//Taking a Rvalue and do the same logic.
template <typename  T>
void PushBack (T&& val) ;

//Construct the object in the function using the object constructors and using the same order of Args.
//Args: params to T constructor.
//inserting the object in the end of the list
template <class... Args>
void EmplaceBack (Args&&... args);

//Merges two sorted lists into one. The lists should be sorted into ascending order.
//This operation is stable: for equivalent elements in the two lists, the elements from *this shall
// always precede the elements from other,
// and the order of equivalent elements of *this and other does not change.
//	comparison function object (i.e. an object that satisfies the requirements of Compare) which returns true
//	if the first argument is less than (i.e. is ordered before) the second.
// bool cmp(const Type1 &a, const Type2 &b);
//you can call this function without gviving comp, and the defual < will be used.
template <class Compare, class T>
void Merge( SinglyLinkedList<T>& other, Compare comp );

//reverse the order of the list
void Reverse();

//Sorts the elements in the list, altering their position within the container.
//The sorting is performed by applying an algorithm that uses either operator<  or comp
// to compare elements. This comparison shall produce a strict
// weak ordering of the elements (i.e., a consistent transitive comparison, without considering its reflexiveness).
template <class Compare>
void Sort (Compare comp);


template <typename  T>
struct Node{
    Node* next;
    T* element;
    Node( Node* next, T* element){
        this->next = next;
        this->element = element;
    }
    ~Node(){
        delete(element);
    }




};
template <typename  T , class A >
class SinglyLinkedList{
    typedef A allocator_type;
    typedef typename A::value_type value_type;
    typedef typename A::reference reference;
    typedef typename A::const_reference const_reference;
    typedef typename A::difference_type difference_type;
    typedef typename A::size_type size_type;
    typedef typename A::pointer pointer;
    typedef std::forward_iterator_tag iterator_category; //or another tag


    class AssignDidNotSucceed{};
    class Empty{};
    typedef Node<T> Node;

    Node* head;
    int size ;

    //curr: Node in the list(cant be nullptr)
    //prev: one before curr
    //put curr in his place
    template<typename Compare>
    static void SortNode(Node* head,Node* prev, Node* curr , Compare comp){
        prev->next = curr ->next;
        InsertSortedNode(head,curr,comp);
    }

    void PushStart(T* copy){
        Node* temp = head->next;
        head-> next = new Node(temp, copy);
        this->size++;
    }

    void PushEnd(T* copy){
        Node* last = GetLastNode(head);
        last -> next = new Node(nullptr,copy);
        this->size++;
    }

int AuxSizeRec(Node* node){
        if(!node->next){
            return 0;
        }
        return 1+ AuxSizeRec(node->next);
    }


    void AuxDeleteRec(Node* curr){
        if(!curr){
            return;
        }
        AuxDeleteRec(curr->next);
        delete (curr);
    }


    static void AuxPushBackRec(Node* demo , Node* curr ,T* element){
        if(!curr){
            demo->next = new Node(nullptr,element);
            return;
        }
        AuxPushBackRec(demo->next,curr->next,element);
    }

    static void ChangeLastAndForward(Node* & last, Node* & curr){
        last -> next = curr;
        curr = curr -> next;
        last = last->next;
    }

    // The list state is empty, but no deletion
    void DenoteEmpty(){
        head->next = nullptr;
        size = 0;
    }
    static bool cmp( T &a, const T &b){
        return a < b;
    }

    struct Pred{
        const T& val;
        explicit Pred(const T& value): val(value){};
        bool operator()(const T& val2){
            return val == val2;
        }
    };

    //to_insert: the node that will be inserted, cant be nullptr
    template <class Compare>
    void InsertSortedNode(Node* to_insert,Compare cmp){
        InsertSortedNode(head, to_insert,cmp);
        size++;
    }

    template <class Compare>
    static void InsertSortedNode(Node* head, Node* to_insert ,Compare cmp){
        Node* curr = head->next;
        Node* prev = head;
        while(curr) {
            if (cmp( *curr->element , *to_insert->element)) {
                prev = curr;
                curr = curr->next;
            }
            else{
                prev->next = to_insert;
                to_insert->next = curr;
                return;
            }

        }
       prev->next = to_insert;
       to_insert->next = nullptr;
    }

    void AuxReverseRec(Node* head){
        //empty list
        if(!head->next){
            return;
        }
        //one elemnet list
        if(!head->next->next){
            return;
        }

        AuxReverseRec(head->next);
        Node* last = GetLastNode(head);
        Node* first = head->next;
        Node* second = first->next;

        last->next = first;
        first->next = nullptr;
        head->next = second;
    }
    //for empty list return the demo node
    //return the last NODE of the list
     Node* GetLastNode(Node* head){
        Node* ptr = head;
        while(ptr->next){
            ptr = ptr ->next;
        }
        return ptr;
    }




      void InsertAfterNode( Node* node , T* element){
        Node* curr  = new Node(node->next,element);
        node->next = curr;
        size++;
    }

    //Insert element in the end of the list and update the last node
    //last: pointer to the last element
    //val: the element that will be inserted
    //If it doesnt succeed the list is deleted
     void InsertLast(Node*& last , const T& val){
        T *copy;
        try {
            copy = new T(val);
        }
        catch(std::bad_alloc&){
            throw AssignDidNotSucceed();
        }
        InsertAfterNode(last,copy);
        last = last->next;
    }

    void RemoveNode(Node* prev, Node* curr ){
        if(!curr){
            return;
        }
        prev->next = curr -> next;
        delete (curr);
        size--;
    }
    template <typename Compare>
    static void AuxSort(Node* head,Compare comp){
        if(!head->next){
            return;
        }
        AuxSort(head->next,comp);
        SortNode(head,head,head->next,comp);

    }
public:



    SinglyLinkedList(){
        //creating a demo node, so we dont have to check if the list empty when inserting element
        //head->next is the first node in the list.
        head = new Node(nullptr, nullptr);
        size = 0;
    }

    bool Empty() const noexcept{
        return (size == 0);
    }


    size_type MaxSize () const noexcept{
        return size_t(-1)/sizeof(T);
    }


void Swap(SinglyLinkedList& lst) {
    std::swap(lst.head->next,head->next);
    std::swap(this->size,lst.size);
}

    void Clear(){
        Node* tmp;
        Node* curr = head->next;
        while(curr){
            tmp = curr;
            curr = curr->next;
            delete (tmp);
        }
        DenoteEmpty();
    }

    void ClearRec(){

        AuxDeleteRec(head->next);
        DenoteEmpty();
    }

    template <class InputIterator>
    void Assign (InputIterator first, InputIterator last) {

        if (first == last) {
            Clear();
            return;
        }
        SinglyLinkedList lst ;
        Node* last_node = lst.head;
        while(first != last ){
            lst.InsertLast(last_node,*first);
            ++first;
        }
        Clear();
        Swap(lst);

    }
    // The new contents are n elements, each initialized to a copy of val.
    void Assign (size_type n, const T& val){
        SinglyLinkedList lst ;
        Node* last_node = lst.head;
        for(size_type i = 0; i < n; i++){
            lst.InsertLast(last_node,val);
        }
        Clear();
        Swap(lst);
    }
    // the new contents are copies of the values
    // passed as initializer list, in the same order.
    void Assign (std::initializer_list<value_type> il){

        SinglyLinkedList lst ;
        Node* last_node = lst.head;
        for(auto it = il.begin(); it != il.end() ; ++it){
            lst.InsertLast(last_node,*it);
        }
        Clear();
        Swap(lst);
    }


    int Size() const{
        return size;
    }
    int SizeRec(){
        return AuxSizeRec(head);
    }

    //retunn the first element in th list
    //if the list is emety, then Empty exception is thrown
    reference Front(){
        if(size == 0){
            throw Empty();
        }
        return * ( (head->next)->element);
    }
    const_reference Front() const{
        if(size == 0){
            throw Empty();
        }
        return * ( (head->next)->element);
    }
    //Remove all the elements that equal to val.
    void Remove (const value_type& val){
        Pred pred(val);
        remove_if(pred);

    }

    template <class Predicate>
    void remove_if (Predicate pred){
        Node* curr = head->next;
        Node* prev = head;
        while(curr){
            if( pred (* curr->element)){
                RemoveNode(prev,curr);
                curr = prev->next;
            }
            else {
                prev = curr;
                curr = curr->next;
            }
        }
    }

    void PushFront(const T& element) {
        T* copy =  new T(element);
        PushStart(copy);
    }

     void  PushFront(T&& element){
        T* copy =  new T(std::move(element) );
        PushStart(copy);
    }
    template <class... Args>
    void EmplaceFront (Args&&... args){
        PushStart(new  T(std::forward<Args>(args)...));

    }

    void PopFront(){
        if(head->next){
            Node* second_node = head->next->next;
            delete (head->next);
            head -> next = second_node;
            this->size--;
        }
        //if list is empty, then nothing happend.
    }


    void PushBack(const T& element){
        T* copy =  new T(element);
        PushEnd(copy);
    }

    void PushBack (T&& element){
        T* copy =  new T(std::move(element));
        PushEnd(copy);
    }

    void PushBackRec(const T& element){
        T* copy =  new T(element);
        AuxPushBackRec(head,head->next,copy);
        size++;
    }

    template <class... Args>
    void EmplaceBack (Args&&... args){
        PushEnd(new  T(std::forward<Args>(args)...));
    }




    template <class Compare>
    static void AuxMergeRec(Node* this_head , Node* other_head,Compare comp){
        //if both are demo nodes, so both lists are empty
        if(!this_head->next && !other_head->next){
            return;
        }
        else if(!this_head -> next && other_head->next) {
           this_head->next = other_head->next;
            return;
        }
        AuxMergeRec(this_head->next ,other_head ,comp );
        //recursion assumption: this_head->next is the start of the list, hence it is the demo node of the "returned list"
        //but it may be a valid Node.
        //so we only need to insert this_head->next in the  sorted place
        Node* to_insert = this_head->next;
        this_head->next = this_head->next ? this_head->next->next : nullptr;
        InsertSortedNode(this_head,to_insert,cmp);
    }

    void MergeRec( SinglyLinkedList& other ){
        if(this == &other){
            return;
        }
        AuxMergeRec (this->head,other.head,cmp);
        size = this->size + other.size;
        other.DenoteEmpty();

    }

    void Merge( SinglyLinkedList& other ){
        Merge(other ,cmp);
    }

    template <class Compare>
    void Merge( SinglyLinkedList& other, Compare comp ){
        if(this == &other)
            return;
        Node* this_last = head;
        Node* this_curr = head->next;
        Node* other_curr = other.head->next;
        while(this_curr && other_curr){
            if(comp(*other_curr->element,*this_curr->element)){
                ChangeLastAndForward(this_last , other_curr);
            }
            else{
                ChangeLastAndForward(this_last , this_curr);
            }
        }
        while(this_curr){
            ChangeLastAndForward(this_last , this_curr);
        }

        while(other_curr){
            ChangeLastAndForward(this_last , other_curr);
        }
        size = other.size + this->size;
        this_last->next = nullptr;
        other.DenoteEmpty();
    }

    void Reverse(){
        if(this->Size() <= 1){
            return;
        }

        Node* curr = head->next->next;
        Node* prev = head->next;
        prev->next = nullptr;
        Node* tmp;
        while(curr){
            tmp = curr->next;
            curr->next = prev;
            prev = curr;
            curr = tmp;
        }
        head->next = prev;
    }

    void ReverseRec(){
        AuxReverseRec( head);
    }

    void Sort(){
        AuxSort(head,cmp);
    }




  /*  SinglyLinkedList& operator=(const SinglyLinkedList& other){
        Assign(other.begin(),other.end());
    }
*/
  ~SinglyLinkedList(){
      Clear();
      delete head;
  }
    //two list are equal, if the size of the lists are the same, and the element(by their operator == ) are the same
    template <typename  K>
     friend bool operator==(const SinglyLinkedList<K>& lhs, const SinglyLinkedList<K>& rhs);

//https://stackoverflow.com/questions/7758580/writing-your-own-stl-container/7759622#7759622

    class iterator {
    private:
        Node* curr;
    public:


        explicit iterator(Node* n){
            curr = n;
        }
        iterator(const iterator& other){
            curr = other.curr;
        }


        iterator& operator=(const iterator& other){
            curr = other.curr;
        }
        bool operator==(const iterator& other) const{
            return  this->curr == other.curr;
        }
        bool operator!=(const iterator& other) const{
            return !this->operator==(other);
        }


        iterator& operator++(){
            curr = curr ->next;
            return *this;
        }

        reference operator*() const{
            return  * curr -> element;
        }
        pointer operator->() const{
            return   (curr -> element);
        }
    };


    iterator begin(){
        return iterator(head->next);
    }

    iterator end(){
        return iterator(nullptr);
    }
};
template <typename  K>
 bool operator==(const SinglyLinkedList<K>& lhs, const SinglyLinkedList<K>& rhs){
    Node<K>* lhs_curr = lhs.head->next;
    Node<K>* rhs_curr = rhs.head->next;
    if(lhs.size != rhs.size ){
        return  false;
    }
    while( !lhs_curr   ){
        if(*lhs_curr->element != *rhs_curr->element){
            return  false;
        }
        lhs_curr = lhs_curr->next;
       rhs_curr = rhs_curr ->next;
        }
    return true;
}

//const_iterstor end() const;
//const_iterator cend() const;
//const_iterator begin() const;
//const_iterator cbegin() const;

//TODO
/*
class const_iterator {
public:
    typedef typename A::difference_type difference_type;
    typedef typename A::value_type value_type;
    typedef typename const A::reference reference;
    typedef typename const A::pointer pointer;
    typedef std::random_access_iterator_tag iterator_category; //or another tag

    const_iterator ();
    const_iterator (const const_iterator&);
    const_iterator (const iterator&);
    ~const_iterator();

    const_iterator& operator=(const const_iterator&);
    bool operator==(const const_iterator&) const;
    bool operator!=(const const_iterator&) const;


    const_iterator& operator++();


    reference operator*() const;
    pointer operator->() const;
};

*/




#endif //LIST_SINGLYLINKEDLIST_H
