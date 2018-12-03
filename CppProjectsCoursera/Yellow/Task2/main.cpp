#include <iostream>
#include <cstdint>

using namespace std;

int main() {
    uint64_t N = 0, R = 0;
    cin >> N >> R;
    uint64_t mas = 0;
    for (uint64_t i = 0; i < N; ++i)
    {
        uint64_t w = 0, h = 0, d = 0;
        cin >> w >> h >> d;
        mas += R*w*h*d;
    }
    cout << mas;
    return 0;
}
