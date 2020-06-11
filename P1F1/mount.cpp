#include "mount.h"
#include <string>

using namespace std;

MOUNT::MOUNT()
{

}

MOUNT::MOUNT(string path, string name, string id)
{
    this->path = path;
    this->name = name;
    this->id = id;
}
