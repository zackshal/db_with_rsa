#include "student.h"
#include "encrypt.h"
#include <iostream>
#include <limits>
#include <regex>
#include <string>
#include <vector>
#include <Windows.h>
#include <fstream>
#include <cctype>
#include <cstdlib>

using namespace std;

int main() {
    Student student;
    vector<Student> group;

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    cout << "Выполнил Шалашов Захар, БББО-01-23, 25 вариант." << endl;
    cout << "|---------------------------------------------" << endl;

    string answer;
    string markBookNumber;

    do {
        cout << "|---------------------------------------------" << endl;
        cout << "|Выберите действие:" << endl;
        cout << "|1. Добавить студента" << endl;
        cout << "|2. Добавить оценку студенту" << endl;
        cout << "|3. Вывести информацию о студентах" << endl;
        cout << "|4. Изменить определённые данные о студенте" << endl;
        cout << "|5. Удалить студента" << endl;
        cout << "|6. Загрузить данные из исходника" << endl;
        cout << "|7. Выгрузить в результирующий файл" << endl;
        cout << "|8. Шифрование данных" << endl;
        cout << "|9. Сортировка студентов по году поступления" << endl;
        cout << "|---------------------------------------------" << endl;
        cout << "|Ваш выбор: ";
        cin >> answer;

        std::ofstream outputFile;
        std::ifstream inputFile;

        switch (stoi(answer)) {
        case 1:
            Student::addStudent(group);
            break;
        case 2:
            cout << "Введите номер зачетки студента" << endl;
            cin >> markBookNumber;
            Student::addMark(group, markBookNumber);
            break;
        case 3:
            Student::printStudents(group);
            break;
        case 4:
            Student::checkStudent(group);
            break;
        case 5:
            Student::deleteStudent(group);
            break;
        case 6:
            inputFile.open("input.txt");
            Student::readFromFile(group, inputFile);
            inputFile.close();
            break;
        case 7:
            outputFile.open("output.txt", std::ios::out | std::ios::binary);
            Student::writeToFile(group, outputFile);
            outputFile.close();
            break;
        case 8:
            setlocale(LC_ALL, "ru");
            decryption("output.txt.enc");
            cout << "Файл расшифрован: " << endl;
            inputFile.open("output.txt");
            Student::readFromFile(group, inputFile);
            inputFile.close();
            encryption("output.txt");
            cout << "Файл зашифрован." << endl;
            break;
        case 9:
            cout << "---------------------------------------------" << endl;
            cout << "Отсортированные студенты по году поступления: \n";
            Student::sortStudentsByYear(group);
            for (const auto& student : group) {
                cout << student.getSurname() << " "
                    << student.getName()
                    << " (Год поступления: "
                    << student.getYearOfEnroll() << ")\n";
            }
            break;
        default:
            cout << "Неверный выбор. Попробуйте снова." << endl;
            break;
        }
    } while (true);

    return 0;
}
