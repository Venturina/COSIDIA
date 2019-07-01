#ifndef OBJECT_CONTEXT_HPP
#define OBJECT_CONTEXT_HPP

class ObjectContext
{
public:
    virtual ObjectContext copyContext() { return *this;};
private:
};


#endif /* OBJECT_CONTEXT_HPP */