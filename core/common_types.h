#pragma once
#include <map>
#include <string>
#include <variant>
#include <vector>

// Define a variant type that can hold different data types
using CoreVariant = std::variant<std::string, double, int, bool>;

// Define a map that can hold different data types
using CoreVariantMap = std::map<std::string, CoreVariant>;

// Helper functions for variant conversion
inline std::string variantToString(const CoreVariant& variant) {
    if (std::holds_alternative<std::string>(variant)) {
        return std::get<std::string>(variant);
    } else if (std::holds_alternative<double>(variant)) {
        return std::to_string(std::get<double>(variant));
    } else if (std::holds_alternative<int>(variant)) {
        return std::to_string(std::get<int>(variant));
    } else if (std::holds_alternative<bool>(variant)) {
        return std::get<bool>(variant) ? "true" : "false";
    }
    return "";
}
