#include <iostream>
#include <string>
#include <unordered_map>
#include <stdexcept>

class unitconverter {
public:
    void addcategoryconverter(const std::string& category, unitconverter* converter) {
        categoryConverters[category] = converter; 
    }    

    double convert(const std::string & category, const std::string& from, const std::string & to, double value) {
        if (categoryConverters.find(category) != categoryConverters.end()) {
            return categoryConverters[category]->convert(from,to,value);
        } else {
            throw std::invalid_argument("Category not supported.");
        }
    }

    virtual double convert(const std::string& from, const std::string& to, double value) {
        return value;
    }

private:
    std::unordered_map<std::string, unitconverter*> categoryConverters;    
};

class Lengthconverter: public unitconverter {
public:
    Lengthconverter () {
        // Populate the conversion factors
        conversions["meter"]["kilometer"] = 0.001;
        conversions["meter"]["mile"] = 0.000621371;
        conversions["kilometer"]["meter"] = 1000;
        conversions["kilometer"]["mile"] = 0.621371;
        conversions["mile"]["meter"] = 1609.34;
        conversions["mile"]["kilometer"] = 1.60934;
    }

    double convert(const std::string & from, const std::string& to, double value) override {
        if (from == to) return value;
        if (conversions.find(from) != conversions.end() && conversions[from].find(to) != conversions[from].end()) {
            return value * conversions[from][to];
        } else {
            throw std::invalid_argument("Conversion is not supported between" + from + "and" + to);
        }
    }
private:
    std::unordered_map<std::string, std::unordered_map<std::string, double>> conversions;      
};

class weightconverter: public unitconverter {
public:
    weightconverter() {
        conversions["gram"]["kilogram"] = 0.001;
        conversions["gram"]["pound"] = 0.00220462;
        conversions["kilogram"]["gram"] = 1000;
        conversions["kilogram"]["pound"] = 2.20462;
        conversions["pound"]["gram"] = 453.592;
        conversions["pound"]["kilogram"] = 0.453592;
    }

    double convert(const std::string & from, const std::string& to, double value) override {
        if (from == to) return value;
        if (conversions.find(from) != conversions.end() && conversions[from].find(to) != conversions[from].end()) {
            return value * conversions[from][to];
        } else {
            throw std::invalid_argument("Conversion is not supported between" + from + "and" + to);
        }
    }
private:
    std::unordered_map<std::string, std::unordered_map<std::string, double>>conversions;        
};

class temperatureconverter: public unitconverter {
public:
    double convert(const std::string & from, const std::string& to, double value) override {
        if (from == to) return value;
        if (from == "celsius" && to == "fahrenheit") return value * 9/5 + 32;
        if (from == "fahrenheit" && to == "celsius") return (value - 32) * 5/9;
        if (from == "celsius" && to == "kelvin") return value + 273.15;
        if (from == "kelvin" && to == "celsius") return value - 273.15;
        if (from == "fahrenheit" && to == "kelvin") return (value - 32) * 5/9 + 273.15;
        if (from == "kelvin" && to == "fahrenheit") return (value - 273.15) * 9/5 + 32;
        throw std::invalid_argument("Conversion not supported between" + from + "and" + to);
    }     
};

int main() {
    unitconverter converter;
    Lengthconverter Lengthconverter;
    weightconverter weightconverter;
    temperatureconverter temperatureconverter;

    converter.addcategoryconverter("lenght", & Lengthconverter);
    converter.addcategoryconverter("weight", & weightconverter);
    converter.addcategoryconverter("temperature", & temperatureconverter);

    // Example usage 
    double value;
    std::string category, from, to;

    std::cout << "Enter category (length, weight, temperature):";
    std::cin >> category;
    std::cout << "Enter from unit:";
    std::cin >> from;
    std::cout << "Enter to unit:";
    std::cin >> to;
    std::cout << "Enter value:";
    std::cin >> value;

    try {
        double result = converter.convert(category, from, to, value);
        std::cout << "Converted value:" << result << std::endl;
    } catch (const std::invalid_argument & e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}