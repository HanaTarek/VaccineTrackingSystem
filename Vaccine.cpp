#include "Vaccine.h"
#include<iostream>
using namespace std;

Vaccine::Vaccine() {
    this->name = "";
    this->country = "";
    this->reqDoses = 0;
}

Vaccine::Vaccine(string name, string country, int reqDoses) {
    this->name = name;
    this->country = country;
    this->reqDoses = reqDoses;
}

void Vaccine::UpdateData(string name, string country, int reqDoses) {
    setName(name);
    setCountry(country);
    setReqDoses(reqDoses);
}