#include <sys/socket.h>
#include <unistd.h>
#include <system_error>

class RawSocket {
    public:
    RawSocket(int domain, int type, int protocol) {
        _desc = socket(domain, type, protocol);
        if (_desc < 0) {
            throw std::system_error(
                errno,
                std::system_category(),
                "ERROR during socket initialization"
            );
        }
    }
    ~RawSocket() {
        if (_desc >= 0) {
            printf("closing socket\n");
            close(_desc);
        }
    }
    int get_desc() {
        return _desc;
    }
    private:
    int _desc = -1;
};
