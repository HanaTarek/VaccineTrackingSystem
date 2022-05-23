#include<iostream>
#include "map"
#include "unordered_map"
#include "queue"
#include "list"
#include "Vaccine.h"

using namespace std;

class Request {
private:
    string userNationalId;
    int state;
    Vaccine vaccine;
    char* date;

public:
    Request();
    Request(const string& userNationalId, int state, const Vaccine& vaccine, char* date);

    static int GetNumberOfUserRequestsWithStatus(vector<list<Request>::iterator>& requestsIterators, int status);
    static list<Request>::iterator DeleteUserRequestWithStatus(list<Request>& requests, vector<list<Request>::iterator>& requestsIterators, int status);
    static list<Request>::iterator CreateRequest(list<Request>& request, string& userNationalId, int state, Vaccine& vaccine);
    static void DeleteAllUserRequests(list<Request>& requests, vector<list<Request>::iterator>& requestsIterators);

    string& getUserNationalId() {
        return userNationalId;
    }

    int getState() const {
        return state;
    }

    const Vaccine& getVaccine() const {
        return vaccine;
    }

    char* getDate() const {
        return date;
    }

    ~Request() {
    }
};
