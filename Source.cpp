#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;

struct Student {
    char name[100];
    char birthday[11];
    char gender[10]; 
    int course;
    double mark;
    Student* next;
};

Student* add(Student* top, const string& Name, const string& Birthday, const string& Gender, int Course, double Mark) {
    Student* newStudent = new Student();
    strncpy_s(newStudent->name, Name.c_str(), 99);
    strncpy_s(newStudent->birthday, Birthday.c_str(), 10);
    strncpy_s(newStudent->gender, Gender.c_str(), 9);
    newStudent->course = Course;
    newStudent->mark = Mark;
    newStudent->next = nullptr;

    if (!top) {
        return newStudent;
    }
    else {
        Student* curr = top;
        while (curr->next) {
            curr = curr->next;
        }
        curr->next = newStudent;
    }
    return top;
}

void Print(Student* top) {
    while (top) {
        cout << top->name << "\t" << top->birthday << "\t" << top->gender << "\t"
            << top->course << "\t" << top->mark << endl;
        top = top->next;
    }
}

Student* Youngest(Student* top, int course) {
    Student* curr = top;
    Student* youngest = nullptr;

    while (curr) {
        if (curr->course == course && (!youngest || strcmp(curr->birthday, youngest->birthday) > 0)) {
            youngest = curr;
        }
        curr = curr->next;
    }
    return youngest;
}

Student* YoungList(Student* original) {
    Student* young_list = nullptr;

    for (int course = 1; course <= 4; course++) {
        Student* young = Youngest(original, course);
        if (young) {
            young_list = add(young_list, young->name, young->birthday, young->gender, young->course, young->mark);
        }
    }

    return young_list;
}

void Save(const string& filename, Student* top) {
    ofstream file(filename, ios::binary);
    if (file) {
        while (top) {
            file.write(reinterpret_cast<const char*>(top), sizeof(Student));
            top = top->next;
        }
    }
    else {
        cerr << "Ошибка записи файла: " << filename << endl;
    }
}

Student* Load(const string& filename) {
    ifstream file(filename, ios::binary);
    Student* top = nullptr;

    if (file) {
        Student temp;
        while (file.read(reinterpret_cast<char*>(&temp), sizeof(Student))) {
            top = add(top, temp.name, temp.birthday, temp.gender, temp.course, temp.mark);
        }
    }
    else {
        cerr << "Ошибка открытия файла: " << filename << endl;
    }

    return top;
}

void FreeMemory(Student* top) {
    while (top) {
        Student* temp = top;
        top = top->next;
        delete temp;
    }
}

int main() {
    setlocale(LC_ALL, "rus");
    Student* students = nullptr;
    Student* youngest_students = nullptr;

    students = Load("students.dat");

    cout << "Введите количество студентов: ";
    int count;
    cin >> count;
    cin.ignore();

    FreeMemory(students);
    students = nullptr;

    for (int i = 0; i < count; ++i) {
        string name, birthday, gender;
        int course;
        double mark;

        cout << "Студент #" << i + 1 << ":\n";
        cout << "Имя: ";
        getline(cin, name);
        cout << "Дата рождения (YYYY-MM-DD): ";
        getline(cin, birthday);
        cout << "Пол: ";
        getline(cin, gender);
        cout << "Курс: ";
        cin >> course;
        cout << "Оценка: ";
        cin >> mark;
        cin.ignore();

        students = add(students, name, birthday, gender, course, mark);
    }

    Save("students.dat", students);
    cout << "Все студенты:\n";
    Print(students);

    youngest_students = YoungList(students);
    Save("youngest_students.dat", youngest_students);

    cout << "Самые молодые студенты:\n";
    Print(youngest_students);

    FreeMemory(students);
    FreeMemory(youngest_students);

    return 0;
}
