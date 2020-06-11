#ifndef MOUNT_H
#define MOUNT_H
#include <string>

using namespace std;

class MOUNT
{
public:
    string path;
    string name;
    string id;

    MOUNT();
    MOUNT(string path, string name, string id);
};

#endif // MOUNT_H
