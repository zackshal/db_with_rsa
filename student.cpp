#include "student.h"
#include <regex>
#include <fstream>
#include <cctype>
#include <chrono>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <iomanip>
#include <cstdlib>


Student::Student() : year_of_enroll(0), sex(false) {}

void Student::setPersonalInfo(const std::string& surname, const std::string& name, const std::string& patronymic,
    const std::string& faculty, const std::string& department, const std::string& group,
    const std::string& birth, const std::string& mark_book_number, int year_of_enroll,
    bool sex) {
    this->surname = surname;
    this->name = name;
    this->patronymic = patronymic;
    this->faculty = faculty;
    this->department = department;
    this->group = group;
    this->birth = birth;
    this->mark_book_number = mark_book_number;
    this->year_of_enroll = year_of_enroll;
    this->sex = sex;
    this->fixSurname();
    this->fixName();
    this->fixPatronymic();
    this->semesters = semesters;
}

void Student::fixSurname()
{
    for (int i = 0; i < surname.length();i++) {
        if (i == 0) {
            surname[i] = toupper(surname[i]);
        }
        else if (surname[i] == '-') {
            if (i + 1 < surname.length()) {
                i++;
                surname[i] = toupper(surname[i]);
            }
        }
        else surname[i] = tolower(surname[i]);

    }
    
}

void Student::fixName()
{
    for (int i = 0; i < name.length();i++) {
        if (i == 0) {
            name[i] = toupper(name[i]);
        }
        else if (name[i] == '-') {
            if (i + 1 < name.length()) {
                i++;
                name[i] = toupper(name[i]);
            }
        }
        else name[i]=tolower(name[i]);
    }
    
}

void Student::fixPatronymic()
{
    for (int i = 0; i < patronymic.length();i++) {
        if (i == 0) {
            patronymic[i]=toupper(patronymic[i]);
        }
        else if (patronymic[i] == '-') {
            if (i + 1 < patronymic.length() ){
                i++;
                patronymic[i]=toupper(patronymic[i]);
            }
        }
        else patronymic[i]=tolower(patronymic[i]);
    }
    
}

bool Student::validInputGroupAndNum()
{
    Student currentStudent;
    bool validInput = false;

    do {
        cout << "������� ����� �������� ������ (������: �����XXXX): ";
        cin >> mark_book_number;

        if (!currentStudent.isValidMarkBookFormat(mark_book_number)) {
            cout << "�������� ������ ������ �������� ������. ���������� �����." << endl;
            continue;
        }

        bool validYear = false;
        do {
            cout << "������� ��� �����������: ";
            cin >> year_of_enroll;

            if (!currentStudent.isValidYearFormat(year_of_enroll, mark_book_number)) {
                cout << "��������� ��� ����� ���� ����������� � ������ ��� ����� �������� ������ �� ���������." << endl;
                continue;
            }

            validYear = true;
        } while (!validYear);

        validInput = true;
    } while (!validInput);
    return false;
}

void Student::addMark(const string& subject, int mark, int semester) {
    auto it = (*markbook[semester - 1]).find(subject);
    if (it != (*markbook[semester - 1]).end()) {
        // ���� ������ ��� ����, ��������� �
        (*markbook[semester - 1])[subject] = mark;
    }
    else {
        // ���� ������ ��� ����� �������� ��� ���, ��������� �����
        // ���������, ��� �� ��������� ������������ ���������� ���������
        (*markbook[semester - 1])[subject] = mark;
    }
}

void Student::readFromFile(std::vector<Student>& group, std::ifstream& file) {
    if (!file.is_open()) {
        std::cout << "���... ��������. ���� �� ������ ��� ������. ���������� ��� ���." << std::endl;
        return;
    }

    std::string line;
    Student student;
    while (std::getline(file, line)) {
        std::cout << "DEBUG: Reading line: " << line << std::endl; // ����������� ����� ��� �������

        if (line.find("���������: ") != std::string::npos) {
            std::istringstream ss(line.substr(11));
            ss >> student.faculty; // �������� "���������: "
        }
        else if (line.find("�������: ") != std::string::npos) {

            std::istringstream ss(line.substr(9));
            ss >> student.department; // �������� "�������: "
        }
        else if (line.find("��� �����������: ") != std::string::npos) {
            std::istringstream ss(line.substr(18));
            ss >> student.year_of_enroll; // �������� "��� �����������: "
        }
        else if (line.find("�������: ") != std::string::npos) {
            std::istringstream ss(line.substr(9));
            ss >> student.surname;
        }
        else if (line.find("���: ") != std::string::npos) {
            std::istringstream ss(line.substr(5));
            ss >> student.name; // �������� "���: "
        }
        else if (line.find("��������: ") != std::string::npos) {
            std::istringstream ss(line.substr(10));
            ss >> student.patronymic; // �������� "��������: "
        }
        else if (line.find("���� ��������: ") != std::string::npos) {
            std::istringstream ss(line.substr(15));
            ss >> student.birth; // �������� "���� ��������: "
        }
        else if (line.find("����� �������� ������: ") != std::string::npos) {
            std::istringstream ss(line.substr(25));
            ss >> student.mark_book_number; // �������� "����� �������� ������: "
        }
        else if (line.find("������:") != std::string::npos) {
            std::map<std::string, int> marks;
            while (std::getline(file, line) && !line.empty()) {
                size_t pos = line.find(':');
                std::string subject = line.substr(0, pos);
                int mark = std::stoi(line.substr(pos + 2)); // �������� ": "
                marks[subject] = mark;
            }
            student.markbook.push_back(new std::map<std::string, int>(marks));
            cout << "������ ���������." << endl;
        }
        else if (line.empty()) {
            group.push_back(student);
            student = Student(); // ���������� ������ ��� ���������� ��������
        }
    }

    for (const auto& student : group) {
        std::cout << "�������: " << student.surname << std::endl;
        std::cout << "���: " << student.name << std::endl;
        std::cout << "��������: " << student.patronymic << std::endl;
        std::cout << "���������: " << student.faculty << std::endl;
        std::cout << "�������: " << student.department << std::endl;
        std::cout << "������: " << student.group << std::endl;
        std::cout << "���� ��������: " << student.birth << std::endl;
        std::cout << "����� �������� ������: " << student.mark_book_number << std::endl;
        std::cout << "��� �����������: " << student.year_of_enroll << std::endl;
        std::cout << "������:" << std::endl;
        for (const auto* marks : student.markbook) {
            for (const auto& pair : *marks) {
                std::cout << pair.first << ": " << pair.second << std::endl;
            }
        }
        std::cout << std::endl;
    }
}





void Student::writeToFile(vector<Student>& group, std::ofstream& file) {
    // ���������� ������ ������ � ����
    if (!file.is_open()) {
        std::cout << "��-�... ���������. ��� ���� �� ������ ��� ������. ���������� ��� �����!" << std::endl;
        return;
    }


    //Student student1;
    //student1.faculty = "���� ����";
    //student1.department = "�����";
    //student1.year_of_enroll = 2020;
    //student1.surname = "�";
    //student1.name = "�";
    //student1.patronymic = "�";
    //student1.birth = "01.03.2002";
    //student1.mark_book_number = "20�1488";
    //// ��������� ������ ��� ��������
    //student1.markbook.push_back(new std::map<std::string, int>{ {"������������ ������������", 5} });
    //group.push_back(student1);

    //Student student2;
    //student2.faculty = "���� ����";
    //student2.department = "�����";
    //student2.year_of_enroll = 2020;
    //student2.surname = "�";
    //student2.name = "�";
    //student2.patronymic = "�";
    //student2.birth = "01.03.2002";
    //student2.mark_book_number = "20�1481";
    //// ��������� ������ ��� ��������
    ////student2.markbook.push_back(new std::map<std::string, int>());
    //group.push_back(student2);

    for (const auto& student : group) {
        file << "���������: " << student.faculty << std::endl;
        file << "�������: " << student.department << std::endl;
        file << "��� �����������: " << student.year_of_enroll << std::endl;
        file << "�������: " << student.surname << std::endl;
        file << "���: " << student.name << std::endl;
        file << "��������: " << student.patronymic << std::endl;
        file << "���� ��������: " << student.birth << std::endl;
        file << "����� �������� ������: " << student.mark_book_number << std::endl;
        file << "������: " << student.group << endl;
        file << "������:" << std::endl;
        for (int i = 0; i < student.markbook.size(); ++i) {
            file << "������� #" << i + 1 << std::endl;
            for (const auto& pair : *student.markbook[i]) {
                file << pair.first << ": " << pair.second << std::endl;
            }
        }
        file << std::endl;
    }

    std::cout << "������ ���������" << std::endl;

    file.close();
}



void Student::inputInfo() {
    std::string surname, name, patronymic, faculty, department, group, birth, markBookNum;
    int year, mark;
    bool sex;

    cin.ignore();
    std::cout << "������� �������: ";
    std::getline(std::cin, surname);
    std::cout << "������� ���: ";
    std::getline(std::cin, name);
    std::cout << "������� ��������: ";
    std::getline(std::cin, patronymic);
    std::cout << "������� ���������: ";
    std::getline(std::cin, faculty);
    std::cout << "������� �������: ";
    std::getline(std::cin, department);

    while (true) {
        cout << "������� ������: ";
        getline(cin, group);
        if (group.empty()) {
            cout << "�������� ������ �� ����� ���� ������. ���������� �����." << endl;
        }
        else {
            break;
        }
    }

    std::cout << "������� ���� �������� (��.��.����): ";
    std::getline(std::cin, birth);
    std::cout << "������� ����� �������� ������: ";
    std::getline(std::cin,  markBookNum);

    while (true) {
        cout << "������� ��� �����������: ";
        cin >> year;
        if (year < 0 || cin.fail()) {
            cout << "�������� ������ ����. ���������� �����." << endl;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ������� ������ �����
        }
        else {
            cin.ignore();
            break;
        }
    }

    std::cout << "������� ��� (������� - 1, ������� - 0): ";
    cin >> sex;
   
    setPersonalInfo(surname, name, patronymic, faculty, department, group, birth, markBookNum, year, sex);
    semesters = calculateSemesters();
    
}


void Student::printInfo() const {
    std::cout << "�������: " << surname << std::endl;
    std::cout << "���: " << name << std::endl;
    std::cout << "��������: " << patronymic << std::endl;
    std::cout << "���������: " << faculty << std::endl;
    std::cout << "�������: " << department << std::endl;
    std::cout << "������: " << group << std::endl;
    std::cout << "���� ��������: " << birth << std::endl;
    std::cout << "����� �������� ������: " << mark_book_number << std::endl;
    std::cout << "��� �����������: " << year_of_enroll << std::endl;
    std::cout << "���: " << (sex ? "�������" : "�������") << std::endl;

    std::cout << "������:" << std::endl;
    for (int i = 0; i < markbook.size(); i++) {
        cout << "������� #" << i + 1 << endl << "-" << endl;
        for (const auto& pair : *markbook[i]) {
            std::cout << " |- " << pair.first << " -> " << pair.second << std::endl;
        }
    } 
   
}

bool Student::isValidMarkBookFormat(const std::string& markBookNum)
{
    std::regex markBookPattern(R"(^\d{2}[�-��-�]\d{4}$)");
    return std::regex_match(markBookNum, markBookPattern);

}

bool Student::isValidMarkbookSize(int semester) const
{
    return (*markbook[semester-1]).size() <= 10;
}


bool Student::isValidYearFormat(int year, const std::string& markBookNum)
{
    std::string yearStr = std::to_string(year);
    std::string lastTwoDigits = yearStr.substr(2, 2);
    std::string markBookYearPrefix = markBookNum.substr(0, 2);


    return lastTwoDigits==markBookYearPrefix;
}

void Student::addMarksForYear(const std::string& subject, int mark) {
    // todo: ���-��
    //// ���������, ��� ���������� ��������� �� ��������� ������������ ��������
    //if (semesters >= 9) {
    //    std::cout << "������: ��������� ������������ ���������� ��������� ��� ������ ��������." << std::endl;
    //    return;
    //}

    //// ��������� ������ � ������� �������� ������
    //if (!markbook.empty()) {
    //    // ���������, ���� �� ��� ������ ��� ����� ��������
    //    bool found = false;
    //    for (auto& markSet : markbook) {
    //        if (markSet.find(subject) != markSet.end()) {
    //            // ���� ������ ��� ����, ��������� �
    //            markSet[subject] = mark;
    //            found = true;
    //            break;
    //        }
    //    }
    //    // ���� ������ ��� ����� �������� ��� ���, ��������� �����
    //    if (!found) {
    //        map<string, int> newMark;
    //        newMark.insert({ subject, mark });
    //        *markbook.push_back(newMark);
    //    }
    //}
    //else {
    //    std::cout << "������: �������� �� ��������� �� ���� �������� ������." << std::endl;
    //}
}

int Student::getCurrentYear() const {
    std::time_t now;
    std::time(&now);
    std::tm local_now;
    localtime_s(&local_now, &now);
    return local_now.tm_year + 1900;
}
const string& Student::getSurname() const
{
    return surname;
}

const string& Student::getName() const
{
return name;
}

const string& Student::getPatronymic() const
{
return patronymic;
}

const string& Student::getFaculty() const
{
return faculty;
}

const string& Student::getDepartment() const
{
return department;
}

const string& Student::getGroup() const
{
return group;
}

const string& Student::getBirth() const
{
return birth;
}

const string& Student::getMark_Book_Number() const
{
return mark_book_number;
}

bool Student::getSex() const
{
    if (sex) {
        std::cout << "���: �������" << std::endl;
    }
    else {
        std::cout << "���: �������" << std::endl;
    }
    return sex;
}

void Student::deleteStudent(vector<Student>& group)
{
    string markbookToDelete;
    cout << "������� ����� �������� ������ �������� ��� ��������: ";
    cin >> markbookToDelete;

    // ����� �������� �� ������ �������� ������ � �������� ��� �� �������
    for (auto it = group.begin(); it != group.end(); ++it) {
        if (it->mark_book_number == markbookToDelete) {
            group.erase(it);
            std::cout << "������� � �������� " << markbookToDelete << " ������." << endl;
            return;
        }
    }

    std::cout << "������� � ����� ������� �������� ������ �� ������." << std::endl;
}



void Student::addMark(vector<Student>& group, string markBookNumber)
{
    for (Student student : group)
    {
        if (student.getMark_Book_Number() == markBookNumber)
        {
            string subject;
            cin.ignore();
            cout << "������� �������: ";
            getline(cin, subject);

            int mark;
            cout << "������� ������: ";
            cin >> mark;

            int semester_num = 0;
            while (true) {
                cout << "������� ����� ��������: ";
                cin >> semester_num;
                if (semester_num >= 1 && semester_num <= student.markbook.size())
                {
                    if (student.isValidMarkbookSize(semester_num)) {
                        student.addMark(subject, mark, semester_num);
                        cout << "������ ���������" << endl;
                        break;
                    }
                    else {
                        std::cout << "������: ��������� ������������ ���������� ��������� � ���� ��������" << std::endl;
                        return;
                    }
                }
                else {
                    cout << "� �������� ������ ����������� ����� �������" << endl;
                }
            }                

            return;
        }
    }
    cout << "������� � ��������� ������� ������� �� ������." << endl;
}

void Student::saveStudentData()
{
    cout << "������ �������� ������� ��������:" << endl;
    cout << "�������: " << this->surname << endl;
    cout << "���: " << this->name << endl;
    cout << "��������: " << this->patronymic << endl;
    cout << "���������: " << this->faculty << endl;
    cout << "�������: " << this->department << endl;
    cout << "������: " << this->group << endl;
    cout << "���� ��������: " << this->birth << endl;
    cout << "����� �������� ������: " << this->mark_book_number << endl;
    cout << "��� �����������: " << this->year_of_enroll << endl;
    cout << "���: " << (this->sex ? "�������" : "�������") << endl;
}

int Student::getYearOfEnroll() const
{
    return year_of_enroll;
}

bool Student::isValidDateOfBirth()
{
    std::regex dateOfBirthPattern(R"(^(0[1-9]|[12]\d|3[01])\.(0[1-9]|1[0-2])\.(19|20)\d{2}$)");

    if (!std::regex_match(birth, dateOfBirthPattern)) {
        return false; // ������������ ������
    }

    // ������ ���� ��������
    int day = std::stoi(birth.substr(0, 2));
    int month = std::stoi(birth.substr(3, 2));
    int year = std::stoi(birth.substr(6, 4));

    // ���������, ��� ����, ����� � ��� ��������� � ����������� ��������
    if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1900 || year > getCurrentYear()) {
        return false; // ������������ �������� ���, ������ ��� ����
    }

    // ��������� ������� � ���������� ���� � ������������
    bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (month == 2 && ((day > 29 && isLeapYear) || (day > 28 && !isLeapYear))) {
        return false; // �������� ����� ���� � �������
    }

    // �������� ������������ ���������� ���� � ������
    if ((day > 30) && (month == 4 || month == 6 || month == 9 || month == 11)) {
        return false; // �������� ����� ���� � ������
    }

    return true;
}

void Student::inputDateOfBirth()
{
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ���������� ������ ����� ������ � ������
    std::getline(std::cin, birth);

    while (!isValidDateOfBirth()) {
        std::cout << "�������� ������ ���� ��������. ������� ���� �������� (��.��.����) ������: ";
        getline(cin, birth);
    }

    this->birth = birth;

}

int Student::getSemesters() const
{
    return semesters;
}

int Student::calculateSemesters()
{ 
    // �������� ������� ���
    int currentYear = getCurrentYear();

    // ������������ ���������� ���, ��������� � ���� �����������
    int yearsSinceEnrollment = currentYear - year_of_enroll;

    // ������������ ����� ���������� ���������
    int totalSemesters = yearsSinceEnrollment * 2;

    for (int i = 0; i < totalSemesters; i++)
    {
        this->markbook.push_back(new std::map<std::string, int>());
    }

    return totalSemesters;
}

bool Student::compareByYearOfEnroll(const Student& first, const Student& next)
{
    
    return first.year_of_enroll<next.year_of_enroll;
}

void Student::sortStudentsByYear(vector<Student>& students)
{

    sort(students.begin(), students.end(), compareByYearOfEnroll);
}


void Student::updateMarksByMarkBook(const string& markBookNum, const map<string, int>& newMarks) {
   //todo
    //// ���������, ���� �� � �������� ����� �������� ������
    //if (mark_book_number == markBookNum) {
    //    // ���� � �������� ���� ����� �������� ������, ��������� ������
    //    for (const auto& newMark : newMarks) {
    //        // ���������, ���� �� ��� ������ ��� ����� ��������
    //        bool found = false;
    //        for (auto& markSet : markbook) {
    //            if (markSet.find(newMark.first) != markSet.end()) {
    //                // ���� ������ ��� ����, ��������� �
    //                markSet[newMark.first] = newMark.second;
    //                found = true;
    //                break;
    //            }
    //        }
    //        // ���� ������ ��� ����� �������� ��� ���, ��������� �����
    //        if (!found) {
    //            map<string, int> newMarkSet;
    //            newMarkSet.insert(newMark);
    //            markbook->push_back(newMarkSet);
    //        }
    //    }
    //}
}
void Student::changeStudentData()
{
    Student student;
    cout << "�������� ������ ��� ���������:" << endl;
    cout << "1. �������" << endl;
    cout << "2. ���" << endl;
    cout << "3. ��������" << endl;
    cout << "4. ���������" << endl;
    cout << "5. �������" << endl;
    cout << "6. ������" << endl;
    cout << "7. ���� ��������" << endl;
    cout << "8. ����� �������� ������" << endl;
    cout << "9. ��� �����������" << endl;
    cout << "10. ���" << endl;

    int choice;
    cin >> choice;
    cin.ignore(); // ������� ����� �����

    switch (choice) {
    case 1: {
        cout << "������� ����� �������: ";
        getline(cin, this->surname);
        break;
    }
    case 2: {
        cout << "������� ����� ���: ";
        getline(cin, this->name);
        break;
    }
    case 3: {
        cout << "������� ����� ��������: ";
        getline(cin, this->patronymic);
        break;
    }
    case 4: {
        cout << "������� ����� ���������: ";
        getline(cin, this->faculty);
        break;
    }
    case 5: {
        cout << "������� ����� �������: ";
        getline(cin, this->department);
        break;
    }
    case 6: {
        cout << "������� ����� ������: ";
        getline(cin, this->group);
        break;
    }
    case 7: {
        cout << "������� ����� ���� �������� (��.��.����): ";
        getline(cin, this->birth);
        break;
    }
    case 8: {
        cout << "������� ����� ����� �������� ������: ";
        getline(cin, this->mark_book_number);
        break;
    }
    case 9: {
        cout << "������� ����� ��� �����������: ";
        cin >> this->year_of_enroll;
        break;
    }
    case 10: {
        cout << "������� ����� ��� (������� - 1, ������� - 0): ";
        cin >> this->sex;
        break;
    }
    default: {
        cout << "������������ �����." << endl;
        break;
    }
    }
    saveStudentData();
}

void Student::checkStudent(vector<Student> group)
{
    string markBookNumber;
    cout << "������� ����� �������� ������ ��������, ��� ������ �� ������ ��������: ";
    cin >> markBookNumber;
    for (Student student : group)
    {
        if (student.getMark_Book_Number() == markBookNumber)
        {
            student.changeStudentData();
            return;;
        }
    }
    cout << "������� � ����� �������� ������� �� ������." << endl;
}
void Student::setYearOfEnroll(int year)
{
    year_of_enroll = year;
}

void Student::setGroup(const std::string& group) {
    this->group = group;
}

void Student::addStudent(vector<Student>&group) {
    
    Student student;
    student.inputInfo();

    if (student.isValidMarkBookFormat(student.getMark_Book_Number()) 
        && student.isValidYearFormat(student.getYearOfEnroll(), student.getMark_Book_Number())) {
        group.push_back(student);
    }
    else {
        cout << "�������� ������ ������ �������� ������ ��� ���� �����������. ������� �� ��������." << endl;
    }
}



void Student::printStudents(const vector<Student>& group) {
    if (!group.empty()) {
        // ������� ����� �������
        vector<Student> sortedGroup = group;

        // ��������� ����� �������
        Student::sortStudentsByYear(sortedGroup);

        cout << "���������� � ���������:" << endl;
        for (const auto& student : sortedGroup) {
            student.printInfo();
            cout << "---------------------" << endl;
        }
    }
    else {
        cout << "� ������ ��� ���������." << endl;
    }
}

void saveStudentsToFile(const vector<Student>& group, const string& filename) {
    ofstream outFile(filename);
    if (outFile.is_open()) {
        for (const auto& student : group) {
            outFile << student << endl;
        }
        outFile.close();
        cout << "������ ��������� ������� ��������� � ���� " << filename << "." << endl;
    }
    else {
        cout << "�� ������� ������� ���� " << filename << " ��� ������." << endl;
    }
}
void loadStudentsFromFile(vector<Student>& group, const string& filename) {
    ifstream inFile(filename);
    if (inFile.is_open()) {
        Student student;
        while (inFile >> student) {
            group.push_back(student);
        }
        inFile.close();
        cout << "������ ��������� ������� ��������� �� ����� " << filename << "." << endl;
    }
    else {
        cout << "�� ������� ������� ���� " << filename << " ��� ������." << endl;
    }
}

std::ostream& operator<<(std::ostream& os, const Student& student)
{
    os << student.surname << " " << student.name << " " << student.patronymic << " "
        << student.faculty << " " << student.department << " " << student.group << " "
        << student.birth << " " << student.mark_book_number << " " << student.year_of_enroll
        << " " << student.sex;
    return os;
}

std::istream& operator>>(std::istream& is, Student& student)
{
    is >> student.surname >> student.name >> student.patronymic >> student.faculty
        >> student.department >> student.group >> student.birth >> student.mark_book_number
        >> student.year_of_enroll >> student.sex;
    return is;
}
//void encryption(string filename) {
//    srand(time(NULL));
//    char* pass = new char[64];
//    for (int i = 0; i < 64; ++i) {
//        switch (rand() % 3) {
//        case 0:
//            pass[i] = rand() % 10 + '0';
//            break;
//        case 1:
//            pass[i] = rand() % 26 + 'A';
//            break;
//        case 2:
//            pass[i] = rand() % 26 + 'a';
//        }
//    }
//
//    ofstream file("key.txt", ios::binary);
//    file.write(pass, 65);
//    file.close();
//
//    string command = "openssl\\bin\\openssl.exe enc -aes-256-cbc -salt -in " + filename + " -out " + filename + ".enc -pass file:key.txt -pbkdf2";
//    system(command.c_str());
//
//    command = "openssl\\bin\\openssl.exe pkeyutl -encrypt -inkey rsa.public -pubin -in key.txt -out key.txt.enc";
//    system(command.c_str());
//
//    if (remove(filename.c_str()) != 0) {
//        cout << "[ERROR] - deleting file" << endl;
//    }
//
//    if (remove("key.txt") != 0) {
//        cout << "[ERROR] - deleting key" << endl;
//    }
//    return;
//}
//
//
//void decryption(string filename) {
//    string command = "openssl\\bin\\openssl.exe pkeyutl -decrypt -inkey rsa.private -in key.txt.enc -out key.txt";
//    system(command.c_str());
//    if (remove("key.txt.enc") != 0) {
//        cout << "[ERROR] - deleting key.enc" << endl;
//    }
//
//    command = "openssl\\bin\\openssl.exe enc -aes-256-cbc -d -in " + filename + " -out " +
//        filename.substr(0, filename.find(".enc")) + " -pass file:key.txt -pbkdf2";
//    system(command.c_str());
//
//    if (remove("key.txt") != 0) {
//        cout << "[ERROR] - deleting key" << endl;
//    }
//
//    if (remove(filename.c_str()) != 0) {
//        cout << "[ERROR] - deleting file.enc" << endl;
//    }
//    return;
//}

