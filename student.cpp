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
        cout << "Введите номер зачетной книжки (формат: номерXXXX): ";
        cin >> mark_book_number;

        if (!currentStudent.isValidMarkBookFormat(mark_book_number)) {
            cout << "Неверный формат номера зачетной книжки. Попробуйте снова." << endl;
            continue;
        }

        bool validYear = false;
        do {
            cout << "Введите год поступления: ";
            cin >> year_of_enroll;

            if (!currentStudent.isValidYearFormat(year_of_enroll, mark_book_number)) {
                cout << "Последние две цифры года поступления и первые две цифры зачетной книжки не совпадают." << endl;
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
        // Если оценка уже есть, обновляем её
        (*markbook[semester - 1])[subject] = mark;
    }
    else {
        // Если оценки для этого предмета еще нет, добавляем новую
        // Проверяем, что не превышено максимальное количество семестров
        (*markbook[semester - 1])[subject] = mark;
    }
}

void Student::readFromFile(std::vector<Student>& group, std::ifstream& file) {
    if (!file.is_open()) {
        std::cout << "Упс... Проблема. Файл не открыт для чтения. Попробуйте еще раз." << std::endl;
        return;
    }

    std::string line;
    Student student;
    while (std::getline(file, line)) {
        std::cout << "DEBUG: Reading line: " << line << std::endl; // Добавленный вывод для отладки

        if (line.find("Факультет: ") != std::string::npos) {
            std::istringstream ss(line.substr(11));
            ss >> student.faculty; // Обрезаем "Факультет: "
        }
        else if (line.find("Кафедра: ") != std::string::npos) {

            std::istringstream ss(line.substr(9));
            ss >> student.department; // Обрезаем "Кафедра: "
        }
        else if (line.find("Год поступления: ") != std::string::npos) {
            std::istringstream ss(line.substr(18));
            ss >> student.year_of_enroll; // Обрезаем "Год поступления: "
        }
        else if (line.find("Фамилия: ") != std::string::npos) {
            std::istringstream ss(line.substr(9));
            ss >> student.surname;
        }
        else if (line.find("Имя: ") != std::string::npos) {
            std::istringstream ss(line.substr(5));
            ss >> student.name; // Обрезаем "Имя: "
        }
        else if (line.find("Отчество: ") != std::string::npos) {
            std::istringstream ss(line.substr(10));
            ss >> student.patronymic; // Обрезаем "Отчество: "
        }
        else if (line.find("Дата рождения: ") != std::string::npos) {
            std::istringstream ss(line.substr(15));
            ss >> student.birth; // Обрезаем "Дата рождения: "
        }
        else if (line.find("Номер зачётной книжки: ") != std::string::npos) {
            std::istringstream ss(line.substr(25));
            ss >> student.mark_book_number; // Обрезаем "Номер зачётной книжки: "
        }
        else if (line.find("Оценки:") != std::string::npos) {
            std::map<std::string, int> marks;
            while (std::getline(file, line) && !line.empty()) {
                size_t pos = line.find(':');
                std::string subject = line.substr(0, pos);
                int mark = std::stoi(line.substr(pos + 2)); // Обрезаем ": "
                marks[subject] = mark;
            }
            student.markbook.push_back(new std::map<std::string, int>(marks));
            cout << "Данные сохранены." << endl;
        }
        else if (line.empty()) {
            group.push_back(student);
            student = Student(); // Сбрасываем данные для следующего студента
        }
    }

    for (const auto& student : group) {
        std::cout << "Фамилия: " << student.surname << std::endl;
        std::cout << "Имя: " << student.name << std::endl;
        std::cout << "Отчество: " << student.patronymic << std::endl;
        std::cout << "Факультет: " << student.faculty << std::endl;
        std::cout << "Кафедра: " << student.department << std::endl;
        std::cout << "Группа: " << student.group << std::endl;
        std::cout << "Дата рождения: " << student.birth << std::endl;
        std::cout << "Номер зачётной книжки: " << student.mark_book_number << std::endl;
        std::cout << "Год поступления: " << student.year_of_enroll << std::endl;
        std::cout << "Оценки:" << std::endl;
        for (const auto* marks : student.markbook) {
            for (const auto& pair : *marks) {
                std::cout << pair.first << ": " << pair.second << std::endl;
            }
        }
        std::cout << std::endl;
    }
}





void Student::writeToFile(vector<Student>& group, std::ofstream& file) {
    // Реализация записи данных в файл
    if (!file.is_open()) {
        std::cout << "Уп-с... Проблемка. Ваш файл не открыт для чтения. Попробуйте ещё разок!" << std::endl;
        return;
    }


    //Student student1;
    //student1.faculty = "прог инжа";
    //student1.department = "говна";
    //student1.year_of_enroll = 2020;
    //student1.surname = "К";
    //student1.name = "А";
    //student1.patronymic = "Ю";
    //student1.birth = "01.03.2002";
    //student1.mark_book_number = "20И1488";
    //// Добавляем оценки для студента
    //student1.markbook.push_back(new std::map<std::string, int>{ {"Компьютерная безопасность", 5} });
    //group.push_back(student1);

    //Student student2;
    //student2.faculty = "Прог инжа";
    //student2.department = "Говна";
    //student2.year_of_enroll = 2020;
    //student2.surname = "К";
    //student2.name = "А";
    //student2.patronymic = "Ю";
    //student2.birth = "01.03.2002";
    //student2.mark_book_number = "20И1481";
    //// Добавляем оценки для студента
    ////student2.markbook.push_back(new std::map<std::string, int>());
    //group.push_back(student2);

    for (const auto& student : group) {
        file << "Факультет: " << student.faculty << std::endl;
        file << "Кафедра: " << student.department << std::endl;
        file << "Год поступления: " << student.year_of_enroll << std::endl;
        file << "Фамилия: " << student.surname << std::endl;
        file << "Имя: " << student.name << std::endl;
        file << "Отчество: " << student.patronymic << std::endl;
        file << "Дата рождения: " << student.birth << std::endl;
        file << "Номер зачётной книжки: " << student.mark_book_number << std::endl;
        file << "Группа: " << student.group << endl;
        file << "Оценки:" << std::endl;
        for (int i = 0; i < student.markbook.size(); ++i) {
            file << "Семестр #" << i + 1 << std::endl;
            for (const auto& pair : *student.markbook[i]) {
                file << pair.first << ": " << pair.second << std::endl;
            }
        }
        file << std::endl;
    }

    std::cout << "Данные сохранены" << std::endl;

    file.close();
}



void Student::inputInfo() {
    std::string surname, name, patronymic, faculty, department, group, birth, markBookNum;
    int year, mark;
    bool sex;

    cin.ignore();
    std::cout << "Введите фамилию: ";
    std::getline(std::cin, surname);
    std::cout << "Введите имя: ";
    std::getline(std::cin, name);
    std::cout << "Введите отчество: ";
    std::getline(std::cin, patronymic);
    std::cout << "Введите факультет: ";
    std::getline(std::cin, faculty);
    std::cout << "Введите кафедру: ";
    std::getline(std::cin, department);

    while (true) {
        cout << "Введите группу: ";
        getline(cin, group);
        if (group.empty()) {
            cout << "Название группы не может быть пустым. Попробуйте снова." << endl;
        }
        else {
            break;
        }
    }

    std::cout << "Введите дату рождения (ДД.ММ.ГГГГ): ";
    std::getline(std::cin, birth);
    std::cout << "Введите номер зачетной книжки: ";
    std::getline(std::cin,  markBookNum);

    while (true) {
        cout << "Введите год поступления: ";
        cin >> year;
        if (year < 0 || cin.fail()) {
            cout << "Неверный формат года. Попробуйте снова." << endl;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очистка буфера ввода
        }
        else {
            cin.ignore();
            break;
        }
    }

    std::cout << "Введите пол (мужской - 1, женский - 0): ";
    cin >> sex;
   
    setPersonalInfo(surname, name, patronymic, faculty, department, group, birth, markBookNum, year, sex);
    semesters = calculateSemesters();
    
}


void Student::printInfo() const {
    std::cout << "Фамилия: " << surname << std::endl;
    std::cout << "Имя: " << name << std::endl;
    std::cout << "Отчество: " << patronymic << std::endl;
    std::cout << "Факультет: " << faculty << std::endl;
    std::cout << "Кафедра: " << department << std::endl;
    std::cout << "Группа: " << group << std::endl;
    std::cout << "Дата рождения: " << birth << std::endl;
    std::cout << "Номер зачетной книжки: " << mark_book_number << std::endl;
    std::cout << "Год поступления: " << year_of_enroll << std::endl;
    std::cout << "Пол: " << (sex ? "Мужской" : "Женский") << std::endl;

    std::cout << "Оценки:" << std::endl;
    for (int i = 0; i < markbook.size(); i++) {
        cout << "Семестр #" << i + 1 << endl << "-" << endl;
        for (const auto& pair : *markbook[i]) {
            std::cout << " |- " << pair.first << " -> " << pair.second << std::endl;
        }
    } 
   
}

bool Student::isValidMarkBookFormat(const std::string& markBookNum)
{
    std::regex markBookPattern(R"(^\d{2}[А-Яа-я]\d{4}$)");
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
    // todo: что-то
    //// Проверяем, что количество семестров не превышает максимальное значение
    //if (semesters >= 9) {
    //    std::cout << "Ошибка: превышено максимальное количество семестров для одного студента." << std::endl;
    //    return;
    //}

    //// Добавляем оценку в текущую зачетную книжку
    //if (!markbook.empty()) {
    //    // Проверяем, есть ли уже оценка для этого предмета
    //    bool found = false;
    //    for (auto& markSet : markbook) {
    //        if (markSet.find(subject) != markSet.end()) {
    //            // Если оценка уже есть, обновляем её
    //            markSet[subject] = mark;
    //            found = true;
    //            break;
    //        }
    //    }
    //    // Если оценки для этого предмета еще нет, добавляем новую
    //    if (!found) {
    //        map<string, int> newMark;
    //        newMark.insert({ subject, mark });
    //        *markbook.push_back(newMark);
    //    }
    //}
    //else {
    //    std::cout << "Ошибка: студенту не назначена ни одна зачетная книжка." << std::endl;
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
        std::cout << "Пол: Мужской" << std::endl;
    }
    else {
        std::cout << "Пол: Женский" << std::endl;
    }
    return sex;
}

void Student::deleteStudent(vector<Student>& group)
{
    string markbookToDelete;
    cout << "Введите номер зачётной книжки студента для удаления: ";
    cin >> markbookToDelete;

    // Поиск студента по номеру зачётной книжки и удаление его из вектора
    for (auto it = group.begin(); it != group.end(); ++it) {
        if (it->mark_book_number == markbookToDelete) {
            group.erase(it);
            std::cout << "Студент с зачёткой " << markbookToDelete << " удален." << endl;
            return;
        }
    }

    std::cout << "Студент с таким номером зачётной книжки не найден." << std::endl;
}



void Student::addMark(vector<Student>& group, string markBookNumber)
{
    for (Student student : group)
    {
        if (student.getMark_Book_Number() == markBookNumber)
        {
            string subject;
            cin.ignore();
            cout << "Введите предмет: ";
            getline(cin, subject);

            int mark;
            cout << "Введите оценку: ";
            cin >> mark;

            int semester_num = 0;
            while (true) {
                cout << "Введите номер семестра: ";
                cin >> semester_num;
                if (semester_num >= 1 && semester_num <= student.markbook.size())
                {
                    if (student.isValidMarkbookSize(semester_num)) {
                        student.addMark(subject, mark, semester_num);
                        cout << "Оценка добавлена" << endl;
                        break;
                    }
                    else {
                        std::cout << "Ошибка: превышено максимальное количество предметов в этом семестре" << std::endl;
                        return;
                    }
                }
                else {
                    cout << "В зачетной книжке отсутствует такой семестр" << endl;
                }
            }                

            return;
        }
    }
    cout << "Студент с указанным номером зачетки не найден." << endl;
}

void Student::saveStudentData()
{
    cout << "Данные студента успешно изменены:" << endl;
    cout << "Фамилия: " << this->surname << endl;
    cout << "Имя: " << this->name << endl;
    cout << "Отчество: " << this->patronymic << endl;
    cout << "Факультет: " << this->faculty << endl;
    cout << "Кафедра: " << this->department << endl;
    cout << "Группа: " << this->group << endl;
    cout << "Дата рождения: " << this->birth << endl;
    cout << "Номер зачетной книжки: " << this->mark_book_number << endl;
    cout << "Год поступления: " << this->year_of_enroll << endl;
    cout << "Пол: " << (this->sex ? "мужской" : "женский") << endl;
}

int Student::getYearOfEnroll() const
{
    return year_of_enroll;
}

bool Student::isValidDateOfBirth()
{
    std::regex dateOfBirthPattern(R"(^(0[1-9]|[12]\d|3[01])\.(0[1-9]|1[0-2])\.(19|20)\d{2}$)");

    if (!std::regex_match(birth, dateOfBirthPattern)) {
        return false; // Неправильный формат
    }

    // Парсим дату рождения
    int day = std::stoi(birth.substr(0, 2));
    int month = std::stoi(birth.substr(3, 2));
    int year = std::stoi(birth.substr(6, 4));

    // Проверяем, что день, месяц и год находятся в разрешенных пределах
    if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1900 || year > getCurrentYear()) {
        return false; // Неправильные значения дня, месяца или года
    }

    // Проверяем февраль в високосные года и невисокосные
    bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (month == 2 && ((day > 29 && isLeapYear) || (day > 28 && !isLeapYear))) {
        return false; // Неверное число дней в феврале
    }

    // Проверка корректности количества дней в месяце
    if ((day > 30) && (month == 4 || month == 6 || month == 9 || month == 11)) {
        return false; // Неверное число дней в месяце
    }

    return true;
}

void Student::inputDateOfBirth()
{
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Игнорируем символ новой строки в буфере
    std::getline(std::cin, birth);

    while (!isValidDateOfBirth()) {
        std::cout << "Неверный формат даты рождения. Введите дату рождения (ДД.ММ.ГГГГ) заново: ";
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
    // Получаем текущий год
    int currentYear = getCurrentYear();

    // Рассчитываем количество лет, прошедших с года поступления
    int yearsSinceEnrollment = currentYear - year_of_enroll;

    // Рассчитываем общее количество семестров
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
    //// Проверяем, есть ли у студента такая зачётная книжка
    //if (mark_book_number == markBookNum) {
    //    // Если у студента есть такая зачётная книжка, обновляем оценки
    //    for (const auto& newMark : newMarks) {
    //        // Проверяем, есть ли уже оценка для этого предмета
    //        bool found = false;
    //        for (auto& markSet : markbook) {
    //            if (markSet.find(newMark.first) != markSet.end()) {
    //                // Если оценка уже есть, обновляем её
    //                markSet[newMark.first] = newMark.second;
    //                found = true;
    //                break;
    //            }
    //        }
    //        // Если оценки для этого предмета еще нет, добавляем новую
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
    cout << "Выберите данные для изменения:" << endl;
    cout << "1. Фамилия" << endl;
    cout << "2. Имя" << endl;
    cout << "3. Отчество" << endl;
    cout << "4. Факультет" << endl;
    cout << "5. Кафедра" << endl;
    cout << "6. Группа" << endl;
    cout << "7. Дата рождения" << endl;
    cout << "8. Номер зачетной книжки" << endl;
    cout << "9. Год поступления" << endl;
    cout << "10. Пол" << endl;

    int choice;
    cin >> choice;
    cin.ignore(); // Очищаем буфер ввода

    switch (choice) {
    case 1: {
        cout << "Введите новую фамилию: ";
        getline(cin, this->surname);
        break;
    }
    case 2: {
        cout << "Введите новое имя: ";
        getline(cin, this->name);
        break;
    }
    case 3: {
        cout << "Введите новое отчество: ";
        getline(cin, this->patronymic);
        break;
    }
    case 4: {
        cout << "Введите новый факультет: ";
        getline(cin, this->faculty);
        break;
    }
    case 5: {
        cout << "Введите новую кафедру: ";
        getline(cin, this->department);
        break;
    }
    case 6: {
        cout << "Введите новую группу: ";
        getline(cin, this->group);
        break;
    }
    case 7: {
        cout << "Введите новую дату рождения (ДД.ММ.ГГГГ): ";
        getline(cin, this->birth);
        break;
    }
    case 8: {
        cout << "Введите новый номер зачетной книжки: ";
        getline(cin, this->mark_book_number);
        break;
    }
    case 9: {
        cout << "Введите новый год поступления: ";
        cin >> this->year_of_enroll;
        break;
    }
    case 10: {
        cout << "Введите новый пол (мужской - 1, женский - 0): ";
        cin >> this->sex;
        break;
    }
    default: {
        cout << "Некорректный выбор." << endl;
        break;
    }
    }
    saveStudentData();
}

void Student::checkStudent(vector<Student> group)
{
    string markBookNumber;
    cout << "Введите номер зачетной книжки студента, чьи данные вы хотите изменить: ";
    cin >> markBookNumber;
    for (Student student : group)
    {
        if (student.getMark_Book_Number() == markBookNumber)
        {
            student.changeStudentData();
            return;;
        }
    }
    cout << "Студент с такой зачетной книжкой не найден." << endl;
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
        cout << "Неверный формат номера зачетной книжки или года поступления. Студент не добавлен." << endl;
    }
}



void Student::printStudents(const vector<Student>& group) {
    if (!group.empty()) {
        // Создаем копию вектора
        vector<Student> sortedGroup = group;

        // Сортируем копию вектора
        Student::sortStudentsByYear(sortedGroup);

        cout << "Информация о студентах:" << endl;
        for (const auto& student : sortedGroup) {
            student.printInfo();
            cout << "---------------------" << endl;
        }
    }
    else {
        cout << "В группе нет студентов." << endl;
    }
}

void saveStudentsToFile(const vector<Student>& group, const string& filename) {
    ofstream outFile(filename);
    if (outFile.is_open()) {
        for (const auto& student : group) {
            outFile << student << endl;
        }
        outFile.close();
        cout << "Данные студентов успешно сохранены в файл " << filename << "." << endl;
    }
    else {
        cout << "Не удалось открыть файл " << filename << " для записи." << endl;
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
        cout << "Данные студентов успешно загружены из файла " << filename << "." << endl;
    }
    else {
        cout << "Не удалось открыть файл " << filename << " для чтения." << endl;
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

