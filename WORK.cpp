#include "student.h"
#include <iostream>
#include <limits>
#include <regex>
#include <string>
#include <vector>
#include <Windows.h>
#include <fstream>
#include <cctype>
#include <cstdlib>
#include <windows.h>


using namespace std;

int main() {
    Student student;
    vector<Student> group;
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    cout << "Выполнил Шалашов Захар, БББО-01-23, 25 вариант." << endl;
    cout << "|---------------------------------------------" << endl;


    string answer;
    string answ;
    do {
        cout << "|---------------------------------------------" << endl;
        cout << "|Выберите действие:" << endl;
        cout << "|1. Добавить студента" << endl;
        cout << "|2. Добавить оценку студенту" << endl;
        cout << "|3. Вывести информацию о студентах" << endl;
        cout << "|4. Изменение определённых данных о студенте" << endl;
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
        string markBookNumber;

        switch (stoi(answer)) {
        case 1:
            Student::addStudent(group);
            break;
        case 2:
            // ToDo: сделать ввод
            cout << "Введите номер зачетки студента" << endl;
            cin>>markBookNumber;
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
           
            break;
        case 7: 
            outputFile.open("output.txt", std::ios::out | std::ios::binary);
            Student::writeToFile(group, outputFile);
            break;
        case 8:
            /*setlocale(LC_ALL, "ru_RU.UTF-8");
            SetConsoleCP(CP_UTF8);
            SetConsoleOutputCP(CP_UTF8);

            cout << "Вы хотите зашифровать файл? (y/n): ";
            while (cin >> answ) {
                if (answ == "0") {
                    break;
                }
                if (answ == "y") {
                    student.writeToFile(group,outputFile );
                    Student::encryprtion("key.txt");
                    cout << "Данные успешно записаны в файл. Файл зашифрован.\n";
                    break;
                }
                if (answ == "n") {
                    student.writeToFile(group, outputFile);
                    cout << "Данные успешно записаны в файл.\n";
                    break;
                }
                cout << "Введите 'y', если хотите перезаписать данные в файл и зашифровать его.\n"
                    "Введите 'n', если хотите перезаписать данные без шифрования.\n"
                    "Если хотите прервать выполнение команды, введите '0'.\n";
            }*/

        case 9:
            cout << "-------------------------------------" << endl;
            cout << "Программа завершена." << endl;
            Student::sortStudentsByYear(group);
            std::cout << "Отсортированные студенты по году поступления:\n";
            for (const auto& student : group) {
                std::cout << student.getSurname() << " " << student.getName() << " (Год поступления: " << student.getYearOfEnroll() << ")\n";
            }
            return 0;
            break;
        default:
            cout << "Неверный выбор. Попробуйте снова." << endl;
            break;
        }
    } while (true);

    return 0;
}

