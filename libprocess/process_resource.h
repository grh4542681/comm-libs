#ifndef __PROCESS_RESOURCE_H__
#define __PROCESS_RESOURCE_H__

namespace infra::process {

class Resource : virtual public base::Object {
public:
    Resource() { }
    ~Resource() { }
};

};

#endif
