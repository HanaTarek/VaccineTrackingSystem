#include<iostream>
#include "User.h"
#include"Request.h"
#include"Vaccine.h"
#include "forward_list"

using namespace std;

void SeedData(unordered_map<string, User>& users, list<Request>& requests, Vaccine& vacc) {
    User admin;
    User worker;

    admin.setNationalId("1");
    admin.setPassword("123");
    admin.setRole("admin");
    admin.setVaccine(vacc);
    admin.addRequest(Request::CreateRequest(requests, admin.getNationalId(), 2, admin.getVaccine()));
    users.insert(make_pair(admin.getNationalId(), admin));

    worker.setNationalId("2");
    worker.setPassword("123");
    worker.setRole("worker");
    worker.setVaccine(vacc);
    worker.addRequest(Request::CreateRequest(requests, worker.getNationalId(), 2, worker.getVaccine()));
    users.insert(make_pair(worker.getNationalId(), worker));

    for (int i = 0; i < 50; ++i) {
        int tensDigit = i / 10;
        int unitDigit = i % 10;
        User u;
        u.setVaccine(vacc);
        u.setRole("user");
        u.setNationalId("302061201038" + to_string(tensDigit) + to_string(unitDigit));
        u.setPassword("123");
        u.setGender(i % 3 ? 'm' : 'f');


        if (i % 3 == 0) {
            auto it = Request::CreateRequest(requests, u.getNationalId(), 2, u.getVaccine());
            u.addRequest(it);
            it = Request::CreateRequest(requests, u.getNationalId(), 2, u.getVaccine());
            u.addRequest(it);
        }
        else if (i % 3 == 1) {
            auto it = Request::CreateRequest(requests, u.getNationalId(), 1, u.getVaccine());
            u.addRequest(it);
        }
        else {
            auto it = Request::CreateRequest(requests, u.getNationalId(), 2, u.getVaccine());
            u.addRequest(it);
        }


        users.insert(make_pair(u.getNationalId(), u));
    }
}

unordered_map<string, User>::iterator GetUserIterator(unordered_map<string, User>& users, const string& username) {
    unordered_map<string, User>::iterator it;

    it = users.find(username);

    return it;
}

bool UserExists(unordered_map<string, User>& users, unordered_map<string, User>::iterator it) {
    if (it == users.end()) {
        cout << "National id doesn't exist in system\n";
        return false;
    }

    return true;
}

User* Login(unordered_map<string, User>& users, string& nationalId, string& password) {
    auto it = GetUserIterator(users, nationalId);

    if (!UserExists(users, it)) {
        return NULL;
    }

    if (it->second.getPassword() != password) {
        cout << "Password incorrect \n";
        return NULL;
    }

    cout << "Logged in successfully \n";
    return &(it->second);
}

User* Signup(unordered_map<string, User>& users, Vaccine& vacc, list<Request>& requests) {
    User u;
    string nationalIdInput;

    cout << "Please enter national id: ";
    cin >> nationalIdInput;

    auto it = GetUserIterator(users, nationalIdInput);

    while (UserExists(users, it)) {
        cout << "National Id Already Exists. Please input another one: ";
        cin >> nationalIdInput;
        it = GetUserIterator(users, nationalIdInput);
    }

    u.setNationalId(nationalIdInput);
    u.AddData(requests, vacc);
    u.setVaccine(vacc);
    u.setRole("user");
    users.insert(make_pair(u.getNationalId(), u));

    return &users.find(u.getNationalId())->second;
}

void PrintWelcomePrompt() {
    cout << "Welcome to the vaccine tracking system" << "\n\n";
    cout << "Choose an option from the following" << "\n\n";
    cout << "1. Login \n2. Signup\n";
}

User* LoginWithPrompts(unordered_map<string, User>& users) {
    string nationalId;
    string password;

    cout << "Enter your national ID: ";
    cin >> nationalId;
    cout << "Enter your password: ";
    cin >> password;

    User* loginResult = Login(users, nationalId, password);
    if (!loginResult)
        loginResult = LoginWithPrompts(users);

    return loginResult;
}

void AdminMenuWithPrompts(unordered_map<string, User>& users, User* loggedInUser, list<Request>& requests) {
    int input;
    cout << "1. Delete user data\n2. View user data.\n3. View Requests\n4. View Statistics\n";
    cin >> input;

    if (input != 1 && input != 2 && input != 3 && input != 4) {
        AdminMenuWithPrompts(users, loggedInUser, requests);
        return;
    }

    if (input == 1) {
        User::DeleteData(users, *loggedInUser, requests);
    }
    else if (input == 2) {
        User::ViewData(users, *(loggedInUser));
    }
    else if (input == 3) {

        for (auto req : requests) {
            cout << req.getUserNationalId() << " " << req.getState() << "\n";
        }

    }
    else if (input == 4) {
        int* stats = User::GetStatistics(users);
        unsigned long totalNumberOfUsers = users.size();

        string messages[] = { "Percentage of males: ", "Percentage of females: ",
                             "Percentage of fully vaccinated: ", "Percentage of partially vaccinated: ",
                             "Percentage of unvaccinated: " };

        cout << "Total Users count: " << totalNumberOfUsers << endl;

        for (int i = 0; i < 5; ++i) {
            cout << messages[i] << (float)(*(stats + i)) * 100 / totalNumberOfUsers << "%\n";
        }
    }
}

void UserMenuWithPrompts(unordered_map<string, User>& users, User* loggedInUser, list<Request>& requests) {
    int vaccinationStatus = (*loggedInUser).isFullyVaccinated();
    int numberOfPendingRequests = Request::GetNumberOfUserRequestsWithStatus((*loggedInUser).getRequests(), 1);
    bool creatingRequestAllowed = vaccinationStatus != 1 && numberOfPendingRequests == 0;

    if (vaccinationStatus == 1) {
        cout << "You are fully vaccinated\n";
    }

    if (numberOfPendingRequests > 0) {
        cout << "You already have pending requests\n";
    }

    if (creatingRequestAllowed) {
        cout << "1. Submit Request\n";
    }

    cout << "2. View user data.\n3. Update user data\n4. Delete user data\n";

    int input;
    cin >> input;

    if (!creatingRequestAllowed && input == 1) {
        cout << "You are not allowed to create a new request\n";
        UserMenuWithPrompts(users, loggedInUser, requests);

        return;
    }

    if (input == 1) {
        (*loggedInUser).addRequest(Request::CreateRequest(requests, (*loggedInUser).getNationalId(), 1, (*loggedInUser).getVaccine()));
        cout << "Request submitted successfully\n";
    }

    if (input == 2) {
        User::ViewOneUserData(*loggedInUser);
    }

    if (input == 3) {
        (*loggedInUser).UpdateData_Prompts();
    }

    if (input == 4) {
        User::DeleteData(users, *loggedInUser, requests);
    }
}

int main() {
    unordered_map<string, User> users;
    list<Request> requests;
    User* loggedInUser;
    Vaccine vacc = Vaccine("Astrazeneca", "UK", 2);

    SeedData(users, requests, vacc);

    while (true) {
        PrintWelcomePrompt();

        string loginInput;
        cin >> loginInput;

        while (loginInput != "1" && loginInput != "2") {
            cout << "Sorry, invalid input. Please try again. \n";
            cin >> loginInput;
        }

        if (loginInput == "1") {
            loggedInUser = LoginWithPrompts(users);
        }
        else {
            loggedInUser = Signup(users, vacc, requests);
        }

        cout << "Your role is: " << (*loggedInUser).getRole() << "\n";

        if ((*loggedInUser).getRole() == "admin") {
            AdminMenuWithPrompts(users, loggedInUser, requests);
        }
        else if ((*loggedInUser).getRole() == "worker") {
            cout << "Populating your patients data for the day\n";
            vector<Request> v = User::PopulateTodayPatients(requests);
            cout << "Populating data done. Number of patients today is " << v.size() << "\n";

            cout << "Enter the IDs presented to you. When finally done, enter 'n' as an id.\n";
            User::CheckPatients(users, requests, *loggedInUser, v);
            User::EndList(users, requests, *loggedInUser, v);
        }
        else {
            UserMenuWithPrompts(users, loggedInUser, requests);
        }

        cout << "GoodBye!" << endl;
    }
}