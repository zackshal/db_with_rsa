#ifndef STUDENT_H
#define STUDENT_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

class Student {
public:
    Student();

    // Методы для установки данных студента
    void setPersonalInfo(const string& surname, const string& name, const string& patronymic,
        const string& faculty, const string& department, const string& group,
        const string& birth, const string& mark_book_number, int year_of_enroll, bool sex);
    void fixSurname();
    void fixName();
    void fixPatronymic();
    bool validInputGroupAndNum();


    // Методы для добавления и получения оценок
    void addMark(const string& subject, int mark, int semester);

    // Методы для чтения и записи данных студента в файл
   
    void inputInfo();
    // Метод для вывода информации о студенте
    void printInfo() const;
    bool isValidMarkBookFormat(const std::string& markBookNum);
    bool isValidMarkbookSize(int semester) const;
    bool isValidYearFormat(int year, const std::string& markBookNum);
    void addMarksForYear(const std::string& subject, int mark);
    int getCurrentYear() const;
    const string& getSurname() const;
    const string& getName() const;
    const string& getPatronymic() const;
    const string& getFaculty() const;
    const string& getDepartment() const;
    const string& getGroup() const;
    const string& getBirth() const;
    const string& getMark_Book_Number() const;
    bool getSex() const;
   
    void saveStudentData();
    int getYearOfEnroll() const;
    bool isValidDateOfBirth();
    void inputDateOfBirth();
    int getSemesters() const;
    int calculateSemesters();
    void setYearOfEnroll(int year);
    void setGroup(const std::string& group);
    static bool compareByYearOfEnroll(const Student& first, const Student& next);
    static void sortStudentsByYear(vector<Student>& students);
    friend std::ostream& operator<<(std::ostream& os, const Student& student);
    friend std::istream& operator>>(std::istream& is, Student& student);
    void updateMarksByMarkBook(const string& markBookNum, const map<string, int>& newMarks);
    void changeStudentData();
   


    static void addStudent(vector<Student>& group);
    static void addMark(vector<Student>& group, string markBookNumber);
    static void printStudents(const vector<Student>& group);
    static void checkStudent(vector<Student> group);
    static void deleteStudent(vector<Student>& group);
    static void readFromFile(vector<Student>& group, ifstream& file);
    static void writeToFile(vector<Student>& group, ofstream& file);

    static void encryprtion(string filename);
    void decryption(string filename);
private:
    string surname;
    string name;
    string patronymic;
    string faculty;
    string department;
    string group;
    string birth;
    string mark_book_number;
    int year_of_enroll;
    
    bool sex;
    vector<map<string, int>*> markbook;
     
    int semesters;

};

#endif // STUDENT_H
