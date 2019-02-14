#include <list>
#include <algorithm>
#include <iostream>

#include "derivation.h"
#include "celement.h"

/*
 * This function gets derivative from the polynom
 */

std::string derivation(std::string poly) {
    std::list<cElement> Poly_list;                          // declaring Poly_List

// Processing input
    while (poly.length() > 0)                               // during the cycle we will cut poly, so in the end it will be ""
    {
        std::string buffer = "";                            // making buffer for work
        long coef(0);                                       // new coef
        unsigned long degree(0);                            // new degree
        // processing poly - last element moving to buffer
        if (poly.find("+") != std::string::npos || poly.substr(1,poly.length()-1).find("-") != std::string::npos)               // if we have minuses or pluses in poly - getting the last element (from last +- to the end)
        {
            if (static_cast<int>(poly.find_last_of("+")) > static_cast<int>(poly.find_last_of("-"))) buffer = poly.substr(poly.find_last_of("+"));
            if (static_cast<int>(poly.find_last_of("+")) < static_cast<int>(poly.find_last_of("-"))) buffer = poly.substr(poly.find_last_of("-"));
            poly = poly.substr(0,poly.find_last_of(buffer) - buffer.length() + 1);                                              // deleting the part of poly, which is equal to buffer
        } else {
            buffer = poly;             // if there no +-  -  all is the buffer
            poly = "";
        }
        // processing with the element in buffer
        // no x in buffer
        if (buffer.find("x") == std::string::npos)
        {
            degree = 0;
            try {
                coef = std::stol(buffer);                   // try to translate string to long
            } catch (const std::exception &) {
                std::cout << "Error! Wrong argument! The function will give no result!" << std::endl;       // if stol failed - the string is bad
                return "";
            }
            continue;                                       // if stol didn't failed - string has only the number, derivative = 0, so this element is useless - we skip it
        }
        // processing ^ in buffer
        if (buffer.find("^") == std::string::npos)          // if there is no ^ - degree ==1, else try to read degree, if we failed - the string is bad
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
        if (buffer.find("*") == std::string::npos)          // if there is no * - coef = 1 or -1, if buffer has minus - coef = -1
        {                                                   //  else try to read coef, if we failed - the string is bad
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
            bool backElement = true;                                                // would be this element last in the Poly_List
            for (auto it = Poly_list.begin(); it != Poly_list.end(); it++)
            {
                if (degree > it->GetDegree())                                       // if degree of our new element is more, then degree of it - emplace element before it
                {
                    backElement = false;                                            // so the element is not last
                    Poly_list.emplace(it, cElement (coef, degree));                 // emplacing element before it
                    break;                                                          // getting out of the cycle
                }
                if (degree == it->GetDegree())                                      // if degrees of elements are equal - summarize them
                {
                    backElement = false;                                            // so the element is not last
                    it->SetCoef(it->GetCoef() + coef);                              // summarizing coefficients
                    break;                                                          // getting out of the cycle
                }
            }
            if (backElement) Poly_list.emplace_back(cElement (coef, degree));       // if degree < each_element.GetDegree then this is the last element right now
        } else {
            Poly_list.emplace_back(cElement (coef, degree));                // if there are no elements in list - add first
        }
    }


// Output Result
    std::string ResultDerivation = "";                                      // initialization ResultDerivation
    if(!Poly_list.empty())                                                  // if Poly_list is empty derivative = 0
    {
        bool firstelem = {true};
        std::for_each(Poly_list.begin(), Poly_list.end(), [&ResultDerivation, &firstelem] (cElement &it)        // getting derivative and keeping each element of Poly_list to Result String
        {
            // finding derivation of the element
            it.Derivation();                                                // getting derivative from one element
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
    if (ResultDerivation == "") ResultDerivation = "0";                     // if nothing is in derivative - derivative = 0
    return ResultDerivation;
}
