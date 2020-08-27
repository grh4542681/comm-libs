#ifndef __MODEL_RUNABLE_H__
#define __MODEL_RUNABLE_H__

namespace infra::model {

class Runable : virtual public Object {
public:
    Runable() { }
    virtual ~Runable() { }

    virtual void run() = 0;
};

}

#endif
