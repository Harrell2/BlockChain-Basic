
#ifndef ECCalc
#define ECCalc
#include <vector>
// Finish Later

class ElipticCurve
{
public:
    int c;
    int mod;

    ElipticCurve(int constant, int primeMod)
    {
        c = constant;
        mod = primeMod;
    }

    bool isPoint(int x, int y)
    {
        return (y * y) % mod == ((x * x * x) + c) % mod;
    }

    int modInverse(int num)
    {
         num %= mod;
         for (int i = 1; i < mod; i++)
         {
             if ((i * num) % mod == 1) return i;
         }
         return -1;
    }

    std::vector<int> addPoint(std::vector<int> P, std::vector<int> Q)
    {
        std::vector<int> R;
        int slope;

        int x1 = P[0];
        int y1 = P[1];
        int x2 = Q[0];
        int y2 = Q[1];

        if (x1 == x2 && y2 != y1) return R;

        if (x1 == x2) slope = 3 * ((x1 * x1) % mod) * modInverse(2 * y1 % mod) % mod;
        else slope = 1;


        return R;
    }


};
#endif