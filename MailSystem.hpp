#pragma once


#include <unordered_map>
#include <vector>
#include <memory>
#include <string>
//#include "Message.hpp"
#include "User.hpp"

class Message;



class MailSystem {
private:
    std::unordered_map<int, std::vector<std::shared_ptr<Message>>> inbox;
    std::vector<std::shared_ptr<Message>> outbox;
    int next_message_id;

    // Вспомогательные методы
    bool IsDateOlder(const std::string& date1, const std::string& date2) const;
    void CleanupUserInbox(int user_id);

public:
    // Конструктор
    MailSystem();
    
    // Основные методы
    void SendMessage(std::shared_ptr<User> user, const std::string& subject, 
                     const std::string& text, const std::string& date);
    void SendMessageToMultiple(const std::vector<std::shared_ptr<User>>& users, 
                                 const std::string& subject, const std::string& text, 
                                 const std::string& date);
    
    // Работа с входящими сообщениями
    std::vector<std::shared_ptr<Message>> GetInbox(std::shared_ptr<User> user) const;
    
    // Управление сообщениями
    void ClearOldMessages(const std::string& cutoff_date);
    void DeleteMessage(User* user, int message_id);
    
    // Статистика
    int GetMessageCount(User* user) const;
    
    
    
};

