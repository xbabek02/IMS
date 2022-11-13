#ifndef __WITHID_HPP__
#define __WITHID_HPP__

class WithId
{
public:
    WithId();
    WithId(const WithId &item);
    bool operator==(WithId &);
    void SetID_generator(int value);
    void SetID(int id);
    int GetID() const;

protected:
    static int ID_generator;
    int ID;
};

#endif