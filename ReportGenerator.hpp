#pragma once


#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include "Calendar.hpp"
#include "MailSystem.hpp"

class ReportGenerator {
private:
    std::shared_ptr<Calendar> calendar;
    std::unordered_map<std::string, std::string> stats;
    
    // Вспомогательные методы
    void CollectEventStatistics();
    void CollectDepartmentStatistics();
    void CollectMailStatistics();
    void CollectConflictStatistics();

public:
    // Конструкторы
    ReportGenerator(Calendar* cal);
    
    
    // Основные методы
    void GenerateSummary();
    std::string ToText() const;
    bool ExportToFile(const std::string& filename) const;
    
    // Дополнительные методы отчетов
    std::string GenerateDepartmentReport() const;
    std::string generateUserActivityReport() const;
    std::string GenerateEventDistributionReport() const;
    
    // Геттеры
    std::unordered_map<std::string, std::string> GetStats() const;
};

