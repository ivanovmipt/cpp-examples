#include <iostream>

struct Granny { // Diamond problem
    int g;
    static void g_func() {};
};


struct Mother: virtual Granny {
    int m;
    int x;
};

struct Father: virtual Granny {
    int f;
    int x;
};

struct Son: Mother, Father {
    /* // before Granny
    son {
        mother { m }
        father { f }
        s
    }

    Son s;
    Father& f = s; // move pointer
    static_cast<Son&>(f); // return pointer
    */

   /* // after Granny (before g_func())
   son {
        mother { 
            granny { g }
            m
        }
        father {
            granny { g }
            f
        }
        s
    }
   */

   // std::cout << x; // CE
   // std::cout << g; // CE
   // static void g_func(); // CE (can use fields)
   // cast and slice - CE
   // private doesn't affect

    /* // after virtual Granny
   son {
        mother { 
            granny*
            m
        }
        (padding 4)
        father {
            granny*
            f
        }
        s
        granny { g }
    }
    (padding 4)

    total: 40 bytes
   */

    // static_cast<Son&> granny; // CE (not allowed now)

   int s;
};

int main() {
    Son s;
    s.Mother::g_func(); // ok

    // if many pointers we make one pointer to virtual table in stack memory

    return 0;
}
