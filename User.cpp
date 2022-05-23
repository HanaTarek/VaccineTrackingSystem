#include "User.h"
#include<iostream>
#include "string"
#include <vector>
#include "unordered_map"
#include "list"

using namespace std;

User::User(string fName, string LName, string nationalId, string password, string governate,
    string role, char gender, int age, Vaccine vaccine) {
    this->fName = fName;
    this->LName = LName;
    this->nationalId = nationalId;
    this->password = password;
    this->governate = governate;
    this->role = role;
    this->gender = gender;
    this->age = age;
    this->vaccine = vaccine;
}
User::User() {
    this->fName = "";
    this->LName = "";
    this->nationalId = "";
    this->password = "";
    this->governate = "";
    this->role = "";
    this->gender = 'o';
    this->age = 0;
}

void User::AddData(list<Request>& requests, Vaccine& vacc) {
    string s;
    char c;
    int i;
    cout << "Welcome to the vaccine system" << endl;
    cout << "Fill this form" << endl;
    cout << "-----------------------------" << endl;

    cout << "Please enter first name : ";
    cin >> s; setFName(s);

    cout << "Please enter last name : ";
    cin >> s; setLName(s);

    cout << "Please enter password : ";
    cin >> s; setPassword(s);

    cout << "Please enter governate : ";
    cin >> s; setGovernate(s);

    cout << "Please enter gender (m/f) : ";
    cin >> c; setGender(c);

    cout << "Please enter age (whole number) : ";
    cin >> i; setAge(i);

    cout << "Please enter number of doses taken : ";
    cin >> i;

    while (i < 0 || i > 2) {
        cout << "Correct values are either 0, 1 or 2. Please try again";
        cin >> i;
    }

    for (int j = 0; j < i; ++j) {
        auto it = Request::CreateRequest(requests, getNationalId(), 2, vacc);
        addRequest(it);
    }
}

void User::UpdateData(string fName, string LName, string nationalId, string password, string governate, string role, char gender, int age, bool vaccinated, Vaccine vaccine, int numOfDoses) {
    setFName(fName);
    setLName(LName);
    setNationalId(nationalId);
    setPassword(password);
    setGovernate(governate);
    setRole(role);
    setGender(gender);
    setVaccine(vaccine);
}
void User::UpdateData_Prompts() {
    char wish;
    int choice;
    string s;
    cout << "Do you wish to update any attributes? (y/n) \n";
    cin >> wish;
    while (wish == 'y') {
        cout << "PRESS 1 To change first name \n";
        cout << "PRESS 2 To change last name \n";
        cout << "PRESS 3 To change password \n";
        cout << "PRESS 4 To change governate \n";
        cin >> choice;
        switch (choice) {
        case 1:
            cout << "Enter new first name : ";
            cin >> s;
            setFName(s);
            break;
        case 2:
            cout << "Enter new last name : ";
            cin >> s;
            setLName(s);
            break;
        case 3:
            cout << "Enter new password : ";
            cin >> s;
            setPassword(s);
            break;
        case 4:
            cout << "Enter new governate : ";
            cin >> s;
            setGovernate(s);
            break;
        }
        cout << "Values updated successflly \n";
        cout << "Do you wish to update anything else (y/n)? \n";
        cin >> wish;
    }
}

void User::DeleteData(unordered_map<string, User>& users, User& user, list<Request>& requests) {
    if (IsAdmin(user)) {
        int input = 0;

        cout << "---------------------------------------------------" << endl;
        cout << endl;

        cout << "1) Delete a certain user " << endl;
        cout << "2) Delete all users " << endl;
        cin >> input;


        while (input != 1 && input != 2) {
            cout << "You entered an invalid number. Please try again." << endl;
            cin >> input;
        }

        switch (input) {
        case 1:
        {
            string n_id;
            cout << "---------------------------------------------------" << endl;

            cout << " enter the id of the user you want to delete ";
            cin >> n_id;
            DeleteOne(users, user, n_id, requests);
            break;
        }
        case 2:
        {
            DeleteAll(users, user, requests);
            break;
        }
        default: {}
        }
    }
    else {
        DeleteOne(users, user, user.nationalId, requests);
    }
}
void User::DeleteOne(unordered_map<string, User>& users, const User& currentUser, const string& nationalId,
    list<Request>& requests) {
    std::unordered_map<string, User>::iterator it;

    if (!IsAdmin(currentUser) && currentUser.nationalId != nationalId) {
        cout << "You don't have enough permissions to delete another user\n";
        return;
    }

    it = users.find(nationalId);

    if (it == users.end()) {
        cout << "User Doesn't Exist\n";
        return;
    }

    Request::DeleteAllUserRequests(requests, it->second.getRequests());

    users.erase(it);
    cout << "User with national id: " << nationalId << " deleted successfully\n";
}

void User::DeleteAll(unordered_map<string, User>& users, User& currentUser, list<Request>& requests) {
    int count = 0;

    if (!IsAdmin(currentUser)) {
        cout << "You are not authorized to delete all users\n";
        return;
    }

    auto it = users.begin();

    while (it != users.end()) {
        auto user = it->second;

        if (user.getRole() == "user") {
            count++;
            Request::DeleteAllUserRequests(requests, user.getRequests());
            users.erase(it);
        }
    }

    cout << "Deleted " << count << " users successfully\n";
}

bool User::IsAdmin(const User& user) {
    if (user.role != "admin") {
        return false;
    }

    return true;
}


bool User::IsWorker(User& user) {
    if (user.role != "worker") {
        return false;
    }

    return true;
}

void User::CheckPatients(unordered_map<string, User>& users, list<Request>& requests, User& u, vector<Request>& v)
{
    // di htb2a fl main bns2al al admin kol shwya lw 3ayz ydakhal users anhom gom
    if (IsWorker(u)) {
        string nid;
        char ans;

        while (true) {
            cout << "Enter The National ID of The Patient: ";
            cin >> nid;

            if (nid == "n")
            {
                break;
            }

            cout << "Is the national id entered: " << nid << " correct? ";
            cin >> ans;

            if (tolower(ans) == 'n')
            {
                cout << "Please RE-enter National Id";
                continue;
            }

            auto pred = [&nid](Request req) {
                return req.getUserNationalId() == nid;
            };

            auto it = std::find_if(v.begin(), v.end(), pred);

            if (it != v.end()) {
                auto userIt = users.find(nid);

                auto reqIt = Request::CreateRequest(requests, nid, 2, const_cast<Vaccine&>(it->getVaccine()));
                userIt->second.addRequest(reqIt);

                reqIt = Request::DeleteUserRequestWithStatus(requests, userIt->second.getRequests(), 1);
                userIt->second.deleteRequest(reqIt);

                v.erase(it);
                cout << "Patient Fulfilled" << endl;
            }
            else {
                cout << "This Patient Is Not Due Today" << endl;
            }
        }
    }
}

vector<Request> User::PopulateTodayPatients(list<Request>& requests) {
    vector<Request> list1;
    list<Request>::iterator it;

    for (it = requests.begin(); it != requests.end(); it++) {
        if (list1.size() == 20)
            break;

        if (it->getState() == 1) {
            list1.push_back(*it);
        }
    }

    return list1;
}

void User::EndList(unordered_map<string, User>& users, list<Request>& requests, User& u, vector<Request>& v)
{
    //di htb2a fl main abl ml program y2fl bnshel al fl list w nzbt al status bta3hom
    if (IsWorker(u)) {
        char ans;
        cout << "Are you sure there are no other patients today? ";
        cin >> ans;

        if (tolower(ans) == 'n')
        {
            return;
        }

        for (Request& r : v) {
            auto userIt = users.find(r.getUserNationalId());

            Request::CreateRequest(requests, r.getUserNationalId(), 3, const_cast<Vaccine&>(r.getVaccine()));

            auto reqIt = Request::DeleteUserRequestWithStatus(requests, userIt->second.getRequests(), 1);
            userIt->second.deleteRequest(reqIt);

            cout << "Marking as missed: " << r.getUserNationalId() << endl;
        }

        v.clear();
    }
}

int* User::GetStatistics(unordered_map<string, User>& users) {
    int maleCount = 0;
    int femaleCount = 0;
    int fullyVaccinatedCount = 0;
    int partiallyVaccinatedCount = 0;
    int unvaccinatedCount = 0;

    auto it = users.begin();

    while (it != users.end()) {
        User u = it->second;

        if (u.gender == 'm')
            maleCount++;

        if (u.gender == 'f')
            femaleCount++;

        int vaccinated = u.isFullyVaccinated();

        switch (vaccinated) {
        case 1:
            fullyVaccinatedCount++;
            break;
        case 0:
            partiallyVaccinatedCount++;
            break;
        case -1:
            unvaccinatedCount++;
            break;
        default:
            cout << "Unrecognized vaccination state";
        }

        it++;
    }

    static int stats[5];

    stats[0] = maleCount;
    stats[1] = femaleCount;
    stats[2] = fullyVaccinatedCount;
    stats[3] = partiallyVaccinatedCount;
    stats[4] = unvaccinatedCount;

    return stats;
}

void User::ViewData(unordered_map<string, User>& users, User& user) {
    if (user.role == "user") {
        ViewOneUserData(user);
    }
    else {
        int input = 0;
        cout << "---------------------------------------------------" << endl;
        cout << endl;

        cout << "1) view a certain user " << endl;
        cout << "2) view all users " << endl;
        cin >> input;

        if (input == 1) {
            string n_id;
            cout << "---------------------------------------------------" << endl;

            cout << "Enter the id of the user you want to view: ";
            cin >> n_id;

            auto it = users.find(n_id);

            if (it == users.end()) {
                cout << "User doesn't exist";
                ViewData(users, user);

                return;
            }

            ViewOneUserData(it->second);
        }
        else if (input == 2) {
            auto it = users.begin();

            while (it != users.end()) {
                ViewOneUserData(it->second);
                cout << "================================\n";

                it++;
            }
        }
    }
}

void User::ViewOneUserData(User& user) {
    cout << "First Name :  " << user.fName << " \n";
    cout << "Last Name :  " << user.LName << " \n";
    cout << "National ID:  " << user.nationalId << " \n";
    cout << "Gender:  " << user.gender << " \n";
    cout << "Age:  " << user.age << " \n";
    cout << "Governate:  " << user.governate << " \n";
    cout << "Gender:  " << user.gender << " \n";
    cout << "Vaccination status: " << user.getVaccinationStatus() << "\n";
}