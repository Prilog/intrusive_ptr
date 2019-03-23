#include <iostream>
#include <string>
#include <vector>

#include "intrusive_ptr.h"

using namespace std;

struct A{
    string arg;
    int cnt;

    A() {
        arg = "KRATOS";
        cnt = 0;
    }

    A(string a) {
        arg = a;
        cnt = 0;
    }

    void add_ref() {
        cnt++;
    }

    void release() {
        cnt--;
    }
    int get_refcnt() {
        return cnt;
    }
};

struct B : A {
    B() {
        arg = "BOI";
    }
};

struct C : B {
    C() {
        arg = "THOR";
    }
};

bool base_test() {
    bool res = true;

    A* pupa = new A("PUPA");
    A* lupa = new A("LUPA");

    intrusive_ptr<A> ptr1(pupa);
    intrusive_ptr<A> ptr2(lupa);

    res &= (pupa->get_refcnt() == 1);
    res &= (lupa->get_refcnt() == 1);

    res &= (ptr1 != ptr2);

    ptr1.~intrusive_ptr();
    ptr2.~intrusive_ptr();

    res &= (pupa->get_refcnt() == 0);
    res &= (lupa->get_refcnt() == 0);

    return res;
}

bool comparison_test() {
    bool res = true;

    A* pupa = new A("PUPA");
    A* lupa = new A("LUPA");

    intrusive_ptr<A> ptr1(pupa);
    intrusive_ptr<A> ptr2(lupa);
    intrusive_ptr<A> ptr3(pupa);

    res &= (ptr1 == ptr3);
    res &= (ptr1 != ptr2);
    res &= (ptr2 != ptr3);
    res &= (ptr1 == pupa);
    res &= (ptr2 == lupa);
    res &= (pupa == ptr1);
    res &= (lupa == ptr2);

    return res;
}

bool a_lot_of_objects_test() {
    bool res = true;
    A* leha = new A("LEHA");
    vector <intrusive_ptr<A>> v2;
    for (int i = 0; i < 100; i++) {
        v2.push_back(intrusive_ptr<A>(leha));
        res &= (leha->get_refcnt() == (i + 1));
    }

    for (int i = 99; i >= 0; i--) {
        v2[i].~intrusive_ptr();
        res &= (leha->get_refcnt() == i);
    }
    return res;
}

bool reset_test() {
    bool res = true;

    A* pupa = new A("PUPA");
    A* lupa = new A("LUPA");

    intrusive_ptr<A> ptr1(pupa);
    intrusive_ptr<A> ptr2(lupa);

    res &= (ptr1 != ptr2);

    ptr1.reset(lupa);

    res &= (ptr1 == ptr2);

    ptr1.reset();
    ptr2.reset();

    res &= (ptr1 == ptr2);
    res &= (ptr1.get() == nullptr);

    res &= (pupa->get_refcnt() == 0);
    res &= (lupa->get_refcnt() == 0);

    return res;
}

bool pointer_test() {
    bool res = true;

    A* pupa = new A("PUPA");
    A* lupa = new A("LUPA");

    intrusive_ptr<A> ptr1(pupa);
    intrusive_ptr<A> ptr2(lupa);

    res &= (ptr1.get() == pupa);
    res &= (ptr2.get() == lupa);

    res &= (ptr1->arg == "PUPA");
    res &= (ptr2->arg == "LUPA");

    res &= ((*ptr1).arg == "PUPA");
    res &= ((*ptr2).arg == "LUPA");

    return res;
}

bool bool_test() {
    bool res = true;

    A* pupa = new A("PUPA");
    A* lupa = new A("LUPA");

    intrusive_ptr<A> ptr1(pupa);
    intrusive_ptr<A> ptr2(lupa);

    res &= bool(ptr1);
    res &= bool(ptr2);

    res &= !(intrusive_ptr<A>());

    ptr1.reset();

    res &= (!ptr1);

    return res;
}

bool swap_test() {
    bool res = true;

    A* pupa = new A("PUPA");
    A* lupa = new A("LUPA");

    intrusive_ptr<A> ptr1(pupa);
    intrusive_ptr<A> ptr2(lupa);

    swap(ptr1, ptr2);

    res &= (pupa->get_refcnt() == 1);
    res &= (lupa->get_refcnt() == 1);

    res &= (ptr1->arg == "LUPA");
    res &= (ptr2->arg == "PUPA");

    return res;
}

bool dynamic_cast_test() {
    bool res = true;

    B* loki = new B();
    C* thor = new C();

    intrusive_ptr<B> ptr1(loki);
    intrusive_ptr<C> ptr2(thor);

    intrusive_ptr<A> ptr3 = dynamic_pointer_cast<A, B>(ptr1);
    intrusive_ptr<A> ptr4 = dynamic_pointer_cast<A, C>(ptr2);

    res &= (ptr1->arg == ptr3->arg);
    res &= (ptr2->arg == ptr4->arg);

    return res;
}

void run_test(bool f(), string name) {
    cout << "RUNNING: " << name << endl;
    try {
        if (f()) {
            cout << "DONE" << endl;
        } else {
            cout << "FAIL" << endl;
        }
    } catch(exception& e) {
        cout << e.what() << endl << "EXCEPTION FOUND" << endl;
    }
}

int main() {
    run_test(&base_test, "base_test");
    run_test(&comparison_test, "cmp_test");
    run_test(&a_lot_of_objects_test, "multi_ptr_test");
    run_test(&reset_test, "reset_test");
    run_test(&pointer_test, "pointer_test");
    run_test(&bool_test, "bool_test");
    run_test(&swap_test, "swap_test");
    run_test(&dynamic_cast_test, "dynamic_cast_test");
    return 0;
}