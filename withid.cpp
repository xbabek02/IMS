#include "withid.hpp"

int WithId::ID_generator = 1;

WithId::WithId() : ID(ID_generator++)
{
}

WithId::WithId(const WithId &item)
{
    ID = item.ID;
}

bool WithId::operator==(WithId &second)
{
    if (ID == second.ID)
    {
        return true;
    }
    return false;
}

void WithId::SetID_generator(int value)
{
    ID_generator = value;
}

void WithId::SetID(int id)
{
    ID = id;
}

int WithId::GetID() const
{
    return ID;
}