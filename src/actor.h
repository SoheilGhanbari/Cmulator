#include <vector>

using namespace std;


class actor
{
private:
    int interval;
    vector<float> bbox;
    static void  * moveRoutine(void *);
    static vector<float> curveMove(vector<float>, vector<float>);

public:
    actor(int, vector<float>);
    ~actor();
    void move(int);
};