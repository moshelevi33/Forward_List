

#include "../SinglyLinkedList.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>



struct Foo{
    int* x;
    char c;

    Foo() {
        x = new int(0);
    }
    explicit Foo(int y,char ch){
        x = new int(y);
        c = ch;
    };
    Foo( const Foo & obj) {
        x = new int(*obj.x);
        c = obj.c;
    }

    Foo ( Foo && obj) noexcept {
        x = obj.x;
        c = obj.c;
        obj.x = nullptr;
    }

    Foo& operator=(const Foo& other){
        if(this != &other) {
            int* y = new int(*other.x);
            c = other.c;
            delete(x);
            this->x = y;
        }
        return *this;
    }





    bool operator <(const Foo& other) {
        return (*x < *other.x);
    }

    ~Foo(){
        delete(x);
    }
    friend bool operator==(const Foo& lhs, const Foo& rhs);
    friend bool operator!=(const Foo& lhs, const Foo& rhs);
};

 bool operator==(const Foo& lhs, const Foo& rhs){
     return ( *lhs.x == *rhs.x && lhs.c == rhs.c);

}

 bool operator!=(const Foo& lhs, const Foo& rhs){
     return !(lhs == rhs);
 }


bool CheckEqual( SinglyLinkedList<Foo>& ls ,  Foo arr[]){
    int i = 0;
    if(ls.Size() == 0){
        return false;
    }
    for(auto it = ls.begin() ; it != ls.end() ; ++it){

        if( ! (*it == arr[i]) )
            return  false;
    i++;
    }
    return true;
}





namespace {
    class SinglyLinkedListTest : public ::testing::Test {

    protected:
        SinglyLinkedList<Foo> list;
        SinglyLinkedList<Foo> ls;
        SinglyLinkedList<Foo> ls2;
        SinglyLinkedList<Foo> ls_one;
        Foo  foo_1_a;
        Foo  foo_2_b;
        Foo arr_ls[3];
        Foo arr_ls_reverse[3] ;
        SinglyLinkedList<Foo>:: iterator list_it;
       SinglyLinkedList<Foo>:: iterator ls_it;
        SinglyLinkedList<Foo>:: iterator ls2_it;

        SinglyLinkedListTest():ls_it(nullptr)  , list_it(nullptr), ls2_it(nullptr),arr_ls{Foo(1,'a'),Foo(2,'a'),Foo(3,'a')},arr_ls_reverse{ Foo(3,'a'),Foo(2,'a'),Foo(1,'a')}{};
        //at the start of every TEST_F
        void SetUp() override {

             foo_1_a = Foo(1,'a');
            foo_2_b = Foo(2,'b');
            ls.PushFront(Foo(3,'a'));
            ls.PushFront(Foo(2,'a'));
            ls.PushFront( foo_1_a);

            ls2.PushBack(Foo(0,'b'));
            ls2.PushBack(Foo(1,'b'));
            ls2.PushBack(Foo(3,'b'));
            ls2.PushBack(Foo(4,'b'));

            ls2_it = ls2.begin();
            ls_it =  ls.begin();
        }
        //At the end of every TEST_F
        virtual void TearDown() {
        }
        virtual ~SinglyLinkedListTest() {
        }
    };
}

TEST_F(SinglyLinkedListTest, SizeRec) {
    ASSERT_EQ(ls.SizeRec(), 3);
    ASSERT_EQ(list.SizeRec(), 0);
}

TEST_F(SinglyLinkedListTest, Front) {
    ASSERT_EQ(ls.Front(), foo_1_a);
    EXPECT_ANY_THROW(list.Front());
}

TEST_F(SinglyLinkedListTest, Clear){
     list.Clear();

    ls.Clear();
    ASSERT_EQ(ls.Size(),0);
    ls.EmplaceBack(1,'a');
    ASSERT_EQ(ls.Size(),1);
    ls.Clear();
    ASSERT_EQ(ls.Size(),0);
}

TEST_F(SinglyLinkedListTest, ClearRec){
    list.ClearRec();

    ls.ClearRec();
    ASSERT_EQ(ls.Size(),0);
    ls.EmplaceBack(1,'a');
    ASSERT_EQ(ls.Size(),1);
    ls.ClearRec();
    ASSERT_EQ(ls.Size(),0);
}

TEST_F(SinglyLinkedListTest, PushBackRec){
    ASSERT_EQ(list.Size(),0);
    list.PushBackRec( foo_1_a);
    ASSERT_EQ(list.Size(),1);
    list.PushBackRec( Foo(2,'a'));
    ASSERT_EQ(list.Size(),2);
    Foo arr[2] {Foo(1,'a'),Foo(2,'a')};
    ASSERT_TRUE(CheckEqual(list,arr));
}

TEST_F(SinglyLinkedListTest,EmplaceBack){
    ASSERT_EQ(list.Size(),0);
    list.EmplaceBack( foo_1_a);
    ASSERT_EQ(list.Size(),1);
    list.EmplaceBack( Foo(2,'a'));
    ASSERT_EQ(list.Size(),2);
    Foo arr[2] {Foo(1,'a'),Foo(2,'a')};
    ASSERT_TRUE(CheckEqual(list,arr));
}
TEST_F(SinglyLinkedListTest, PushFront){

    list.PushFront( foo_1_a);
    ASSERT_EQ(list.Size(),1);
    list.PushFront( Foo(2,'a'));
    ASSERT_EQ(list.Size(),2);
    Foo arr[2] {Foo(2,'a'),Foo(1,'a')};
    ASSERT_TRUE(CheckEqual(list,arr));
}

TEST_F(SinglyLinkedListTest, EmplaceFront){

    list.EmplaceFront( 1,'a');
    ASSERT_EQ(list.Size(),1);
    list.EmplaceFront(2,'a');
    ASSERT_EQ(list.Size(),2);
    Foo arr[2] {Foo(2,'a'),Foo(1,'a')};
    ASSERT_TRUE(CheckEqual(list,arr));
}

TEST_F(SinglyLinkedListTest, PopFront){
    list.PopFront();
    ASSERT_EQ(list.Size(),0);
    //0,1,3,4 ls2
    ls2.PopFront();
    ASSERT_EQ(ls2.Size(),3);
    Foo arr[3] {Foo(1,'b'),Foo(3,'b'),Foo(4,'b')};
    ASSERT_TRUE(CheckEqual(ls2,arr));

    ls2.PopFront();
    ASSERT_EQ(ls2.Size(),2);
     Foo arr2[2] {Foo(3,'b'),Foo(4,'b')};
    ASSERT_TRUE(CheckEqual(ls2,arr2));

    ls2.PopFront();
    ASSERT_EQ(ls2.Size(),1);
    Foo arr3[1] {Foo(4,'b')};
    ASSERT_TRUE(CheckEqual(ls2,arr3));

    ls2.PopFront();
    ASSERT_EQ(ls2.Size(),0);
}


TEST_F(SinglyLinkedListTest, Merge){
    ls.Merge(list);
    ASSERT_EQ(ls.Size(),3);
    ls.Merge(ls);
    ASSERT_EQ(ls.Size(),3);

    list.Merge(list);
    ASSERT_EQ(list.Size(),0);

    list.Merge(ls);
    ASSERT_EQ(list.Size(),3);
    ASSERT_TRUE(CheckEqual(list,arr_ls));
    //list = 1,2,3
    //ls_2 = 0,1,3,4
    //0 , 1_ls, 1_ls2,2,3_ls,3_ls2,4
    list.Merge(ls2);
    Foo arr[7] {Foo(0,'b'),Foo(1,'a'),Foo(1,'b'),Foo(2,'a'),Foo(3,'a'),
                Foo(3,'b'),Foo(4,'b')};
    ASSERT_EQ(list.Size(), 7);
    ASSERT_EQ(ls2.Size(), 0);
    ASSERT_TRUE(CheckEqual(list,arr));

}

TEST_F(SinglyLinkedListTest, MergeRec){
    ls.MergeRec(list);
    ASSERT_EQ(ls.Size(),3);
    ls.MergeRec(ls);
    ASSERT_EQ(ls.Size(),3);

    list.MergeRec(list);
    ASSERT_EQ(list.Size(),0);

    list.MergeRec(ls);
    ASSERT_EQ(list.Size(),3);
    ASSERT_TRUE(CheckEqual(list,arr_ls));
    //list = 1,2,3
    //ls_2 = 0,1,3,4
    //0 , 1_ls, 1_ls2,2,3_ls,3_ls2,4
    list.MergeRec(ls2);
    Foo arr[7] {Foo(0,'b'),Foo(1,'a'),Foo(1,'b'),Foo(2,'a'),Foo(3,'a'),
                Foo(3,'b'),Foo(4,'b')};
    ASSERT_EQ(list.Size(), 7);
    ASSERT_EQ(ls2.Size(), 0);
    ASSERT_TRUE(CheckEqual(list,arr));

}

TEST_F(SinglyLinkedListTest, Reverse){
    list.Reverse();

    ls.Reverse();
    Foo arr[3] {Foo(3,'a'),Foo(2,'a'),Foo(1,'a')};
    ASSERT_TRUE(CheckEqual(ls,arr));
}


TEST_F(SinglyLinkedListTest, ReverseRec){
    list.ReverseRec();

    ls.ReverseRec();
    Foo arr[3] {Foo(3,'a'),Foo(2,'a'),Foo(1,'a')};
    ASSERT_TRUE(CheckEqual(ls,arr));

}

TEST_F(SinglyLinkedListTest, AssignIterators){
     list.Assign(list.begin(),list.end());

     ls.Assign(ls.begin(),ls.end());
    ASSERT_EQ(ls.Size(), 3);
    ASSERT_TRUE(CheckEqual(ls,arr_ls));

    list.Assign(ls.begin(),ls.end());
    ASSERT_TRUE(list == ls);
    ASSERT_EQ(list.Size(), 3);

    list.Assign(ls2.begin(),ls2.end());
    ASSERT_TRUE(list == ls2);
    ASSERT_EQ(list.Size(), 4);
}


TEST_F(SinglyLinkedListTest, AssignValue){
    list.Assign(2,foo_1_a);
    ASSERT_EQ(list.Size(), 2);
    Foo arr[2]{foo_1_a,foo_1_a};
    ASSERT_TRUE(CheckEqual(list,arr));

    ls.Assign(2,foo_1_a);
    ASSERT_EQ(ls.Size(), 2);
    ASSERT_TRUE(CheckEqual(ls,arr));

    ls.Assign(7,foo_1_a);
    ASSERT_EQ(ls.Size(), 7);
    Foo arr2[7]{foo_1_a,foo_1_a,foo_1_a,foo_1_a,foo_1_a,foo_1_a,foo_1_a};
    ASSERT_TRUE(CheckEqual(ls,arr2));
}

TEST_F(SinglyLinkedListTest, AssignIntiliazeList){
    list.Assign({foo_1_a,foo_2_b});
    ASSERT_EQ(list.Size(), 2);
    Foo arr[2]{foo_1_a,foo_2_b};
    ASSERT_TRUE(CheckEqual(list,arr));


    ls.Assign({foo_1_a,foo_2_b,Foo(3,'a')});
    ASSERT_EQ(ls.Size(), 3);
    Foo arr3[3]{foo_1_a,foo_2_b,Foo(3,'a')};
    ASSERT_TRUE(CheckEqual(ls,arr3));


}

TEST_F(SinglyLinkedListTest, Remove){
     list.Remove(foo_1_a);

     ls.Remove(foo_1_a);
    ASSERT_EQ(ls.Size(), 2);
    Foo arr2[2]{Foo(2,'a'),Foo(3,'a')};
    ASSERT_TRUE(CheckEqual(ls,arr2) );

    ls.PushFront(foo_1_a);
    ls.EmplaceBack(Foo(1,'a'));
    ls.Remove(foo_1_a);
    ASSERT_EQ(ls.Size(), 2);
    ASSERT_TRUE(CheckEqual(ls,arr2) );

 }

TEST_F(SinglyLinkedListTest, Sort){
     list.Sort();
    ASSERT_EQ(list.Size(),0);

    list.PushBack(foo_1_a);
     ASSERT_EQ(list.Size(),1);
     Foo arr[1] {foo_1_a};
     ASSERT_TRUE(CheckEqual(list,arr));
    SinglyLinkedList<Foo> un_sorted_lst;
     un_sorted_lst.Assign( {Foo(0,'a'),Foo(3,'a'),Foo(2,'a'),Foo(1,'a'),Foo(4,'a'),Foo(2,'b'),Foo(2,'b'),Foo(0,'b')} );
     un_sorted_lst.Sort();
     Foo arr8[8]{Foo(0,'a'),Foo(0,'b'),Foo(1,'a'),Foo(2,'a'),Foo(2,'b'),Foo(2,'b'),Foo(3,'a'),Foo(4,'a')};
     ASSERT_TRUE(CheckEqual(un_sorted_lst,arr8));
 }