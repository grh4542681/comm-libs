#include "io_fd.h"

namespace io {

bool operator<(const FD& a, const int b) {
    return a.GetFD() < b;
}

bool operator<(const int a, const FD& b) {
    return a < b.GetFD();
}

bool operator<(const FD& a, const FD& b) {
    return a.GetFD() < b.GetFD();
}

}
