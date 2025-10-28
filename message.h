#ifndef MESSAGE_H
#define MESSAGE_H
#include <string>
class Message
{
public:
    Message();
protected:
    std::string sender,subject,text;
};

#endif // MESSAGE_H
