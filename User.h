#pragma once

#include<iostream>
#include "Vaccine.h"
#include "unordered_map"
#include "queue"
#include "list"
#include "Request.h"

using namespace std;

class User
{
private:
    string fName;
    string LName;
    string nationalId;
    string password;
    string governate;
    string role;
    char gender;
    int age;
    Vaccine vaccine;
    vector<list<Request>::iterator> requestPointers;

    static bool IsAdmin(const User& user);
    static void DeleteOne(unordered_map<string, User>& users, const User& currentUser, const string& nationalId,
        list<Request>& requests);
    static void DeleteAll(unordered_map<string, User>& users, User& currentUser, list<Request>& requests);
    static bool IsWorker(User& user);
public:
    //constructors
    User();
    User(string fName, string LName, string nationalId, string password, string governate, string role, char gender, int age, Vaccine vaccine);

    //methods
    void AddData(list<Request>& requests, Vaccine& vacc);
    static void ViewData(unordered_map<string, User>& users, User& user);
    static void ViewOneUserData(User& user);
    void UpdateData(string fName, string LName, string nationalId, string password, string governate, string role, char gender, int age, bool vaccinated, Vaccine vaccine, int numOfDoses); // updates everything
    void UpdateData_Prompts(); //asks the user which attributes they wish to update and updates them
    static void DeleteData(unordered_map<string, User>& users, User& user, list<Request>& requests);
    static void CheckPatients(unordered_map<string, User>& users, list<Request>& requests, User& u, vector<Request>& v);
    static void EndList(unordered_map<string, User>& users, list<Request>& requests, User& u, vector<Request>& v);
    static vector<Request> PopulateTodayPatients(list<Request>& requests);
    static int* GetStatistics(unordered_map<string, User>& users);

    //setters
    void setFName(string s) {
        fName = s;
    }
    void setLName(string s) {
        LName = s;
    }
    void setNationalId(string s) {
        nationalId = s;
    }
    void setPassword(string s) {
        password = s;
    }
    void setGovernate(string s) {
        governate = s;
    }
    void setRole(string s) {
        role = s;
    }
    void setGender(char c) {
        gender = c;
    }
    void setAge(int i) {
        age = i;
    }
    void setVaccine(Vaccine& v) {
        vaccine = v;
    }

    void addRequest(list<Request>::iterator request) {
        requestPointers.push_back(request);
    }

    void deleteRequest(list<Request>::iterator request) {
        auto it = std::find(requestPointers.begin(), requestPointers.end(), request);

        if (it != requestPointers.end()) {
            requestPointers.erase(it);
        }
    }

    //getters
    string getFName() {
        return fName;
    }
    string getLName() {
        return LName;
    }
    string& getNationalId() {
        return nationalId;
    }
    string getPassword() {
        return password;
    }
    string getGovernate() {
        return governate;
    }
    string getRole() {
        return role;
    }
    char getGender() {
        return gender;
    }
    int getAge() {
        return age;
    }

    int isFullyVaccinated() {
        int numberOfDoses = Request::GetNumberOfUserRequestsWithStatus(getRequests(), 2);

        if (numberOfDoses == 0) {
            return -1;
        }
        else if (numberOfDoses == getVaccine().getReqDoses()) {
            return 1;
        }
        else {
            return 0;
        }
    }

    string getVaccinationStatus() {
        int vacc = isFullyVaccinated();
        string message;

        if (vacc == -1) {
            message = "Not vaccinated";
        }
        else if (vacc == 1) {
            message = "Fully vaccinated";
        }
        else {
            message = "Partially vaccinated";
        }

        return message;
    }

    Vaccine& getVaccine() {
        return vaccine;
    }

    vector<list<Request>::iterator>& getRequests() {
        return requestPointers;
    }
};

