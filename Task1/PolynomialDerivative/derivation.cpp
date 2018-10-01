#include <list>
#include <algorithm>
#include <iostream>

#include "derivation.h"
#include "celement.h"

std::string derivation(std::string poly) {
    std::list<cElement> Poly_list;

// Processing input
    while (poly.length() > 0)
    {
        std::string buffer = "";
        long coef(0);
        unsigned long degree(0);
        // processing poly - last element moving to buffer
        if (poly.find("+") != std::string::npos || poly.substr(1,poly.length()-1).find("-") != std::string::npos)
        {
            if (static_cast<int>(poly.find_last_of("+")) > static_cast<int>(poly.find_last_of("-"))) buffer = poly.substr(poly.find_last_of("+"));
            if (static_cast<int>(poly.find_last_of("+")) < static_cast<int>(poly.find_last_of("-"))) buffer = poly.substr(poly.find_last_of("-"));
            poly = poly.substr(0,poly.find_last_of(buffer) - buffer.length() + 1);
        } else {
            buffer = poly;
            poly = "";
        }
        // processing with the element in buffer
        // no x in buffer
        if (buffer.find("x") == std::string::npos)
        {
            degree = 0;
            try {
                coef = std::stol(buffer);
            } catch (const std::exception &) {
                std::cout << "Error! Wrong argument! The function will give no result!" << std::endl;
                return "";
            }
            continue;
        }
        // processing ^ in buffer
        if (buffer.find("^") == std::string::npos)
        {
            degree = 1;
        } else {
            try {
                degree = std::stoul(buffer.substr(buffer.find("^")+1));
            } catch (const std::exception &) {
                std::cout << "Error! Wrong argument! The function will give no result!" << std::endl;
                return "";
            }
        }
        // processing * in buffer
        if (buffer.find("*") == std::string::npos)
        {
            if (buffer.find("-") != std::string::npos)
            {
                coef = -1;
            } else {
                coef = 1;
            }
        } else {
            try {
                coef = std::stol(buffer.substr(0,buffer.find("*")));
            } catch (const std::exception &) {
                std::cout << "Error! Wrong argument! The function will give no result!" << std::endl;
                return "";
            }
        }

        // add element to poly_list

        if (!Poly_list.empty())
        {
            bool backElement = true;
            for (auto it = Poly_list.begin(); it != Poly_list.end(); it++)
            {
                if (degree > it->GetDegree())
                {
                    backElement = false;
                    Poly_list.emplace(it, cElement (coef, degree));
                    break;
                }
                if (degree == it->GetDegree())
                {
                    backElement = false;
                    it->SetCoef(it->GetCoef() + coef);
                    break;
                }
            }
            if (backElement) Poly_list.emplace_back(cElement (coef, degree));
        } else {
            Poly_list.emplace_back(cElement (coef, degree));
        }
    }


// Output Result
    std::string ResultDerivation = "";                                      // initialization ResultDerivation
    if(!Poly_list.empty())
    {
        bool firstelem = {true};
        std::for_each(Poly_list.begin(), Poly_list.end(), [&ResultDerivation, &firstelem] (cElement &it)
        {
            // finding derivation of the element
            it.Derivation();
            // processing sign
            if (!firstelem && it.GetCoef() >= 0) ResultDerivation += "+";   // if the element is not first and coef > 0, so we need "...+..."
            firstelem = false;
            // processing coefficient and degree == 0
            if (it.GetDegree() != 0)
            {
                if (it.GetCoef() != 1 && it.GetCoef() != -1)
                {
                    ResultDerivation += std::to_string(it.GetCoef())+"*x";  // coef != 1 or -1, it means "...coef*x..."
                } else {
                    if (it.GetCoef() == 1)  ResultDerivation += "x";        // coef = 1, it means "...+x..."
                    if (it.GetCoef() == -1) ResultDerivation += "-x";       // coef = -1, it means "...-x..."
                }
            } else {ResultDerivation += std::to_string(it.GetCoef());}      // degree is 0, there is no x in this element
            // processing degree
            if (it.GetDegree() != 1 && it.GetDegree() != 0)
            {
                ResultDerivation += "^"+std::to_string(it.GetDegree());     // degree is not 0 or 1, it means "...x^degree..."
            }
        });
    }
    if (ResultDerivation == "") ResultDerivation = "0";
    return ResultDerivation;
}
