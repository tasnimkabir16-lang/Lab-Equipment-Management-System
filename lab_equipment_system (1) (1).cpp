
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <limits>

using namespace std;

string readLine(const string &prompt) {
    cout << prompt;
    string s;
    getline(cin, s);
    return s;
}

int readInt(const string &prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "Invalid number, try again.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string todayDate() {
    time_t t = time(nullptr);
    tm *lt = localtime(&t);
    ostringstream oss;
    oss << (lt->tm_year + 1900) << "-"
        << setw(2) << setfill('0') << (lt->tm_mon + 1) << "-"
        << setw(2) << setfill('0') << lt->tm_mday;
    return oss.str();
}

class Equipment;
class IssueRequest;
class ReturnRecord;
class User {
protected:
    string userID;
    string name;
    string email;
    string password;
    string role;

public:
    User() {}
    User(string id, string n, string e, string p, string r)
        : userID(id), name(n), email(e), password(p), role(r) {}

    virtual ~User() {}

    string getUserID() const { return userID; }
    string getName() const { return name; }
    string getEmail() const { return email; }
    string getRole() const { return role; }

    bool authenticate(const string &emailIn, const string &passIn) const {
        return (email == emailIn && password == passIn);
    }

    void updateProfile() {
        cout << "\n--- Update Profile (" << name << ") ---\n";
        string newName = readLine("New name (leave blank to keep '" + name + "'): ");
        if (!newName.empty()) name = newName;

        string newEmail = readLine("New email (leave blank to keep '" + email + "'): ");
        if (!newEmail.empty()) email = newEmail;

        string newPass = readLine("New password (leave blank to keep current): ");
        if (!newPass.empty()) password = newPass;

        cout << "Profile updated successfully.\n";
    }

    virtual void printSummary() const {
        cout << userID << " | " << name << " | " << email << " | Role: " << role << "\n";
    }
};

class Student : public User {
    string studentID;

public:
    Student() {}
    Student(string id, string n, string e, string p, string sID)
        : User(id, n, e, p, "Student"), studentID(sID) {}

    string getStudentID() const { return studentID; }

};

class LabOfficer : public User {
    string officerID;

public:
    LabOfficer() {}
    LabOfficer(string id, string n, string e, string p, string oID)
        : User(id, n, e, p, "LabOfficer"), officerID(oID) {}

    string getOfficerID() const { return officerID; }
};

// ===========================================================
// DepartmentAdmin
// ===========================================================
class DepartmentAdmin : public User {
    string adminID;

public:
    DepartmentAdmin() {}
    DepartmentAdmin(string id, string n, string e, string p, string aID)
        : User(id, n, e, p, "DepartmentAdmin"), adminID(aID) {}

    string getAdminID() const { return adminID; }
};
class Equipment {
public:
    string equipmentID;
    string name;
    string category;
    string status;
    int quantity;

    Equipment() {}
    Equipment(string id, string n, string cat, string st, int qty)
        : equipmentID(id), name(n), category(cat), status(st), quantity(qty) {}

    void printRow() const {
        cout << left << setw(10) << equipmentID
             << setw(20) << name
             << setw(15) << category
             << setw(12) << status
             << setw(6) << quantity << "\n";
    }
};

class IssueRequest {
public:
    string requestID;
    string issueDate;
    string dueDate;
    string status;
    string studentID;
    string equipmentID;
    int quantity;

    IssueRequest() {}
    IssueRequest(string rid, string sID, string eID, int qty, string due)
        : requestID(rid), issueDate(todayDate()), dueDate(due),
          status("Pending"), studentID(sID), equipmentID(eID), quantity(qty) {}

    void printRow() const {
        cout << left << setw(10) << requestID
             << setw(10) << studentID
             << setw(10) << equipmentID
             << setw(6) << quantity
             << setw(12) << issueDate
             << setw(12) << dueDate
             << setw(10) << status << "\n";
    }
};

class ReturnRecord {
public:
    string returnID;
    string returnDate;
    string condition;
    bool overdueStatus;
    string requestID;

    ReturnRecord() {}
    ReturnRecord(string rid, string reqID, string cond, bool overdue)
        : returnID(rid), returnDate(todayDate()), condition(cond),
          overdueStatus(overdue), requestID(reqID) {}

    void printRow() const {
        cout << left << setw(10) << returnID
             << setw(10) << requestID
             << setw(12) << returnDate
             << setw(10) << condition
             << setw(10) << (overdueStatus ? "Yes" : "No") << "\n";
    }
};

vector<Student> students;
vector<LabOfficer> officers;
vector<DepartmentAdmin> admins;
vector<Equipment> equipmentList;
vector<IssueRequest> requests;
vector<ReturnRecord> returns;

int equipCounter = 1;
int reqCounter = 1;
int retCounter = 1;

Equipment* findEquipment(const string &id) {
    for (auto &e : equipmentList) if (e.equipmentID == id) return &e;
    return nullptr;
}

IssueRequest* findRequest(const string &id) {
    for (auto &r : requests) if (r.requestID == id) return &r;
    return nullptr;
}

Student* findStudentByID(const string &id) {
    for (auto &s : students) if (s.getStudentID() == id || s.getUserID() == id) return &s;
    return nullptr;
}

void generateIssueReturnReport() {
    cout << "\n===== Issue & Return Report =====\n";
    cout << left << setw(10) << "ReqID" << setw(10) << "StudID"
         << setw(10) << "EquipID" << setw(6) << "Qty"
         << setw(12) << "IssueDate" << setw(12) << "DueDate"
         << setw(10) << "Status" << "\n";
    for (auto &r : requests) r.printRow();
}

void generateDamageReport() {
    cout << "\n===== Damage Report =====\n";
    cout << left << setw(10) << "RetID" << setw(10) << "ReqID"
         << setw(12) << "RetDate" << setw(10) << "Condition"
         << setw(10) << "Overdue" << "\n";
    bool any = false;
    for (auto &rr : returns) {
        if (rr.condition == "Damaged" || rr.condition == "Lost") {
            rr.printRow();
            any = true;
        }
    }
    if (!any) cout << "(No damaged or lost items recorded.)\n";
}

void viewBorrowingHistoryReport(const string &studentID) {
    cout << "\n===== Borrowing History for Student " << studentID << " =====\n";
    cout << left << setw(10) << "ReqID" << setw(10) << "EquipID"
         << setw(6) << "Qty" << setw(12) << "IssueDate"
         << setw(12) << "DueDate" << setw(10) << "Status" << "\n";
    bool any = false;
    for (auto &r : requests) {
        if (r.studentID == studentID) {
            r.printRow();
            any = true;
        }
    }
    if (!any) cout << "(No borrowing history found.)\n";
}

void trackOverdueReport() {
    cout << "\n===== Overdue Report =====\n";
    string today = todayDate();
    bool any = false;
    for (auto &r : requests) {
        if (r.status == "Issued" && r.dueDate < today) {
            r.printRow();
            any = true;
        }
    }
    if (!any) cout << "(No overdue items.)\n";
}

void addEquipment() {
    cout << "\n--- Add Equipment ---\n";
    string name = readLine("Equipment name: ");
    string category = readLine("Category: ");
    int qty = readInt("Quantity: ");

    string id = "EQ" + to_string(equipCounter++);
    equipmentList.push_back(Equipment(id, name, category, "Available", qty));
    cout << "Equipment added with ID: " << id << "\n";
}

void updateEquipment() {
    cout << "\n--- Update Equipment ---\n";
    string id = readLine("Enter Equipment ID to update: ");
    Equipment *e = findEquipment(id);
    if (!e) { cout << "Equipment not found.\n"; return; }

    string name = readLine("New name (blank = keep '" + e->name + "'): ");
    if (!name.empty()) e->name = name;

    string category = readLine("New category (blank = keep '" + e->category + "'): ");
    if (!category.empty()) e->category = category;

    string qtyStr = readLine("New quantity (blank = keep " + to_string(e->quantity) + "): ");
    if (!qtyStr.empty()) e->quantity = stoi(qtyStr);

    string status = readLine("New status (blank = keep '" + e->status + "'): ");
    if (!status.empty()) e->status = status;

    cout << "Equipment updated.\n";
}

void deleteEquipment() {
    cout << "\n--- Delete Equipment ---\n";
    string id = readLine("Enter Equipment ID to delete: ");
    auto it = remove_if(equipmentList.begin(), equipmentList.end(),
                         [&](const Equipment &e) { return e.equipmentID == id; });
    if (it == equipmentList.end()) {
        cout << "Equipment not found.\n";
    } else {
        equipmentList.erase(it, equipmentList.end());
        cout << "Equipment deleted.\n";
    }
}

void checkAvailability() {
    cout << "\n--- Check Availability ---\n";
    string id = readLine("Enter Equipment ID: ");
    Equipment *e = findEquipment(id);
    if (!e) { cout << "Equipment not found.\n"; return; }
    cout << e->name << " -> Status: " << e->status << ", Quantity available: " << e->quantity << "\n";
}

void listAllEquipment() {
    cout << "\n===== Equipment Inventory =====\n";
    cout << left << setw(10) << "ID" << setw(20) << "Name"
         << setw(15) << "Category" << setw(12) << "Status" << setw(6) << "Qty" << "\n";
    for (auto &e : equipmentList) e.printRow();
}

void manageInventory() {
    int choice;
    do {
        cout << "\n--- Manage Inventory ---\n"
             << "1. Add Equipment\n"
             << "2. Update Equipment\n"
             << "3. Delete Equipment\n"
             << "4. Check Availability\n"
             << "5. List All Equipment\n"
             << "0. Back\n";
        choice = readInt("Choice: ");
        switch (choice) {
            case 1: addEquipment(); break;
            case 2: updateEquipment(); break;
            case 3: deleteEquipment(); break;
            case 4: checkAvailability(); break;
            case 5: listAllEquipment(); break;
            case 0: break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

void listAllUsers() {
    cout << "\n===== All Users =====\n";
    for (auto &s : students) s.printSummary();
    for (auto &o : officers) o.printSummary();
    for (auto &a : admins) a.printSummary();
}

void deleteUserFlow() {
    string id = readLine("Enter userID to delete: ");
    auto s = remove_if(students.begin(), students.end(),
                        [&](const Student &s) { return s.getUserID() == id; });
    students.erase(s, students.end());

    auto o = remove_if(officers.begin(), officers.end(),
                        [&](const LabOfficer &o) { return o.getUserID() == id; });
    officers.erase(o, officers.end());

    auto a = remove_if(admins.begin(), admins.end(),
                        [&](const DepartmentAdmin &a) { return a.getUserID() == id; });
    admins.erase(a, admins.end());

    cout << "If a user with that ID existed, it has been removed.\n";
}

void manageUsers() {
    int choice;
    do {
        cout << "\n--- Manage Users ---\n"
             << "1. List All Users\n"
             << "2. Delete a User\n"
             << "0. Back\n";
        choice = readInt("Choice: ");
        switch (choice) {
            case 1: listAllUsers(); break;
            case 2: deleteUserFlow(); break;
            case 0: break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

void viewReportsMenu() {
    int choice;
    do {
        cout << "\n--- View Reports ---\n"
             << "1. Issue & Return Report\n"
             << "2. Damage Report\n"
             << "3. Overdue Report\n"
             << "4. Borrowing History (by Student ID)\n"
             << "0. Back\n";
        choice = readInt("Choice: ");
        switch (choice) {
            case 1: generateIssueReturnReport(); break;
            case 2: generateDamageReport(); break;
            case 3: trackOverdueReport(); break;
            case 4: {
                string sid = readLine("Student ID: ");
                viewBorrowingHistoryReport(sid);
                break;
            }
            case 0: break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

void requestEquipment(Student &student) {
    cout << "\n--- Request Equipment ---\n";
    listAllEquipment();
    string equipID = readLine("Enter Equipment ID to request: ");
    Equipment *e = findEquipment(equipID);
    if (!e) { cout << "Equipment not found.\n"; return; }

    int qty = readInt("Quantity needed: ");
    if (qty > e->quantity) {
        cout << "Not enough quantity available (only " << e->quantity << " left).\n";
        return;
    }

    string dueDate = readLine("Requested due date (YYYY-MM-DD): ");

    string reqID = "REQ" + to_string(reqCounter++);
    IssueRequest newReq(reqID, student.getStudentID(), equipID, qty, dueDate);
    requests.push_back(newReq);

    cout << "Request submitted with ID: " << reqID << " (status: Pending)\n";
}

void returnEquipment(Student &student) {
    cout << "\n--- Return Equipment ---\n";
    string reqID = readLine("Enter Request ID being returned: ");
    IssueRequest *req = findRequest(reqID);
    if (!req || req->studentID != student.getStudentID()) {
        cout << "No matching issued request found for this student.\n";
        return;
    }
    if (req->status != "Issued") {
        cout << "This request is not currently in 'Issued' status (current: " << req->status << ").\n";
        return;
    }

    string condition = readLine("Condition of item on return (Good/Damaged/Lost): ");
    bool overdue = req->dueDate < todayDate();

    string retID = "RET" + to_string(retCounter++);
    ReturnRecord rec(retID, reqID, condition, overdue);
    returns.push_back(rec);

    req->status = "Closed";

    cout << "Return recorded with ID: " << retID
         << (overdue ? " (Marked OVERDUE)" : "") << "\n";
    cout << "Awaiting Lab Officer verification and inventory update.\n";
}

void viewBorrowingHistory(Student &student) {
    viewBorrowingHistoryReport(student.getStudentID());
}

void studentMenu(Student &student) {
    int choice;
    do {
        cout << "\n=== Student Menu (" << student.getName() << ") ===\n"
             << "1. Request Equipment\n"
             << "2. Return Equipment\n"
             << "3. View Borrowing History\n"
             << "4. Update Profile\n"
             << "0. Logout\n";
        choice = readInt("Choice: ");
        switch (choice) {
            case 1: requestEquipment(student); break;
            case 2: returnEquipment(student); break;
            case 3: viewBorrowingHistory(student); break;
            case 4: student.updateProfile(); break;
            case 0: cout << "Logging out...\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

void approveRequestFlow() {
    cout << "\n--- Approve Request ---\n";
    generateIssueReturnReport();
    string reqID = readLine("Enter Request ID to approve: ");
    IssueRequest *req = findRequest(reqID);
    if (!req) { cout << "Request not found.\n"; return; }
    if (req->status != "Pending") { cout << "Request is not Pending.\n"; return; }

    req->status = "Approved";

    Equipment *e = findEquipment(req->equipmentID);
    if (e && e->quantity >= req->quantity) {
        e->quantity -= req->quantity;
        if (e->quantity == 0) e->status = "Issued";
        req->status = "Issued";
        cout << "Request approved and equipment issued. Inventory updated.\n";
    } else {
        cout << "Approved, but insufficient stock to issue right now.\n";
    }
}

void rejectRequestFlow() {
    cout << "\n--- Reject Request ---\n";
    generateIssueReturnReport();
    string reqID = readLine("Enter Request ID to reject: ");
    IssueRequest *req = findRequest(reqID);
    if (!req) { cout << "Request not found.\n"; return; }
    if (req->status != "Pending") { cout << "Request is not Pending.\n"; return; }

    string reason = readLine("Reason for rejection: ");
    req->status = "Rejected";
    cout << "Request " << reqID << " rejected. Reason logged: " << reason << "\n";
}

void verifyReturnedItem() {
    cout << "\n--- Verify Returned Item ---\n";
    cout << left << setw(10) << "RetID" << setw(10) << "ReqID"
         << setw(12) << "RetDate" << setw(10) << "Condition"
         << setw(10) << "Overdue" << "\n";
    for (auto &rr : returns) rr.printRow();

    string retID = readLine("Enter Return ID to verify: ");
    auto it = find_if(returns.begin(), returns.end(),
                       [&](const ReturnRecord &r) { return r.returnID == retID; });
    if (it == returns.end()) { cout << "Return record not found.\n"; return; }

    cout << "Verified condition: " << it->condition << "\n";

    IssueRequest *req = findRequest(it->requestID);
    if (req) {
        Equipment *e = findEquipment(req->equipmentID);
        if (e && it->condition == "Good") {
            e->quantity += req->quantity;
            e->status = "Available";
            cout << "Inventory updated: " << req->quantity << " unit(s) returned to stock.\n";
        } else if (e) {
            e->status = "Damaged";
            cout << "Item marked damaged/lost — not returned to available stock.\n";
        }
    }
    cout << "Verification complete.\n";
}

void recordDamage() {
    cout << "\n--- Record Damage ---\n";
    listAllEquipment();
    string equipID = readLine("Enter Equipment ID with damage: ");
    Equipment *e = findEquipment(equipID);
    if (!e) { cout << "Equipment not found.\n"; return; }

    string notes = readLine("Damage notes: ");
    e->status = "Damaged";
    cout << "Equipment " << equipID << " marked as Damaged. Notes recorded: " << notes << "\n";
}

void labOfficerMenu(LabOfficer &officer) {
    int choice;
    do {
        cout << "\n=== Lab Officer Menu (" << officer.getName() << ") ===\n"
             << "1. Approve Request\n"
             << "2. Reject Request\n"
             << "3. Verify Returned Item\n"
             << "4. Record Damage\n"
             << "5. Update Profile\n"
             << "0. Logout\n";
        choice = readInt("Choice: ");
        switch (choice) {
            case 1: approveRequestFlow(); break;
            case 2: rejectRequestFlow(); break;
            case 3: verifyReturnedItem(); break;
            case 4: recordDamage(); break;
            case 5: officer.updateProfile(); break;
            case 0: cout << "Logging out...\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

void adminMenu(DepartmentAdmin &admin) {
    int choice;
    do {
        cout << "\n=== Department Admin Menu (" << admin.getName() << ") ===\n"
             << "1. Manage Inventory\n"
             << "2. Manage Users\n"
             << "3. View Reports\n"
             << "4. Update Profile\n"
             << "0. Logout\n";
        choice = readInt("Choice: ");
        switch (choice) {
            case 1: manageInventory(); break;
            case 2: manageUsers(); break;
            case 3: viewReportsMenu(); break;
            case 4: admin.updateProfile(); break;
            case 0: cout << "Logging out...\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

// ===========================================================
// Registration / Login
// ===========================================================
int userCounter = 1;

void registerUser() {
    cout << "\n--- Register ---\n"
         << "1. Student\n2. Lab Officer\n3. Department Admin\n";
    int roleChoice = readInt("Select role: ");

    string name = readLine("Name: ");
    string email = readLine("Email: ");
    string password = readLine("Password: ");
    string userID = "U" + to_string(userCounter++);

    if (roleChoice == 1) {
        string studentID = readLine("Student ID: ");
        students.push_back(Student(userID, name, email, password, studentID));
        cout << "Student registered. userID = " << userID << "\n";
    } else if (roleChoice == 2) {
        string officerID = readLine("Officer ID: ");
        officers.push_back(LabOfficer(userID, name, email, password, officerID));
        cout << "Lab Officer registered. userID = " << userID << "\n";
    } else if (roleChoice == 3) {
        string adminID = readLine("Admin ID: ");
        admins.push_back(DepartmentAdmin(userID, name, email, password, adminID));
        cout << "Department Admin registered. userID = " << userID << "\n";
    } else {
        cout << "Invalid role selection.\n";
    }
}

void loginUser() {
    cout << "\n--- Login ---\n";
    string email = readLine("Email: ");
    string password = readLine("Password: ");

    for (auto &s : students) {
        if (s.authenticate(email, password)) {
            cout << "Login successful. Welcome, " << s.getName() << "!\n";
            studentMenu(s);
            return;
        }
    }
    for (auto &o : officers) {
        if (o.authenticate(email, password)) {
            cout << "Login successful. Welcome, " << o.getName() << "!\n";
            labOfficerMenu(o);
            return;
        }
    }
    for (auto &a : admins) {
        if (a.authenticate(email, password)) {
            cout << "Login successful. Welcome, " << a.getName() << "!\n";
            adminMenu(a);
            return;
        }
    }
    cout << "Invalid email or password.\n";
}

void seedData() {
    students.push_back(Student("U1", "Rafi Islam", "rafi@example.com", "pass123", "S1001"));
    officers.push_back(LabOfficer("U2", "Nadia Karim", "nadia@example.com", "pass123", "OFC01"));
    admins.push_back(DepartmentAdmin("U3", "Dr. Hasan", "hasan@example.com", "pass123", "ADM01"));
    userCounter = 4;

    equipmentList.push_back(Equipment("EQ" + to_string(equipCounter++), "Digital Multimeter", "Electronics", "Available", 10));
    equipmentList.push_back(Equipment("EQ" + to_string(equipCounter++), "Oscilloscope", "Electronics", "Available", 4));
    equipmentList.push_back(Equipment("EQ" + to_string(equipCounter++), "Soldering Iron", "Tools", "Available", 15));
}

int main() {
    seedData();

    cout << "==========================================\n";
    cout << " Lab Equipment Issue & Return Management \n";
    cout << "==========================================\n";
    cout << "(Seeded demo accounts — all passwords are 'pass123')\n";
    cout << "  Student      : rafi@example.com\n";
    cout << "  Lab Officer  : nadia@example.com\n";
    cout << "  Dept Admin   : hasan@example.com\n";

    int choice;
    do {
        cout << "\n===== Main Menu =====\n"
             << "1. Register\n"
             << "2. Login\n"
             << "0. Exit\n";
        choice = readInt("Choice: ");
        switch (choice) {
            case 1: registerUser(); break;
            case 2: loginUser(); break;
            case 0: cout << "Goodbye!\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);

    return 0;
}
