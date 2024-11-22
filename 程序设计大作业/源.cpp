#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <unordered_set>
#include <iomanip> // 用于控制输出格式

using namespace std;

class Student {
public:
    string name;
    int studentID;
    int chineseScore;
    int mathScore;
    int englishScore;

public:
    Student(string& n, int id, int ch, int ma, int en)
        : name(n), studentID(id), chineseScore(ch), mathScore(ma), englishScore(en) {}

    string getName() {
        return name;
    }

    int getStudentID() {
        return studentID;
    }
    void setStudentID(int id) {
        studentID = id;
    }

    int getChineseScore() {
        return chineseScore;
    }
    void setChineseScore(int score) {
        chineseScore = score;
    }

    int getMathScore() {
        return mathScore;
    }
    void setMathScore(int score) {
        mathScore = score;
    }

    int getEnglishScore() {
        return englishScore;
    }
    void setEnglishScore(int score) {
        englishScore = score;
    }

    int getTotalScore() {
        return chineseScore + mathScore + englishScore;
    }

    int getScore(int index) {
        switch (index) {
        case 0: return chineseScore;
        case 1: return mathScore;
        case 2: return englishScore;
        }
    }

    void setScore(int index, int score) {
        switch (index) {
        case 0: chineseScore = score; break;
        case 1: mathScore = score; break;
        case 2: englishScore = score; break;
        }
    }

    string toString() {
        stringstream ss;
        ss << studentID << "\t" << name << "\t" << chineseScore << "\t" << mathScore << "\t" << englishScore << "\t" << getTotalScore();
        return ss.str();
    }

    static string Head() {
        return "学号\t姓名\t语文\t数学\t英语\t总成绩";
    }

    bool operator<(Student& other) {
        return getTotalScore() < other.getTotalScore();
    }
};

class Score {
public:
    vector<Student> students;
    unordered_set<int> studentIDs;

public:
    void addStudent(Student& student) {
        int studentID;
        while (studentIDs.find(student.getStudentID()) != studentIDs.end()) {
            cout << "学号 " << student.getStudentID() << " 已存在，请重新输入学号：";
            cin >> studentID;
            student.setStudentID(studentID);
        }
        students.push_back(student);
        studentIDs.insert(student.getStudentID());
    }

    void deleteStudent(int studentID) {
        auto it = find_if(students.begin(), students.end(), [&](Student& s) {
            return s.getStudentID() == studentID;
            });

        if (it != students.end()) {
            studentIDs.erase(it->getStudentID());
            students.erase(it);
            cout << "已删除学号为 " << studentID << " 的学生。" << endl;
        }
        else {
            cout << "未找到学号为 " << studentID << " 的学生。" << endl;
        }
    }

    void changeStudentScore(int studentID, int index, int newScore) {
        auto it = find_if(students.begin(), students.end(), [&](Student& s) {
            return s.getStudentID() == studentID;
            });

        if (it != students.end()) {
            it->setScore(index, newScore);
            cout << "已修改学号为：" << studentID << " 的成绩" << endl;
        }
        else {
            cout << "未找到学号为 " << studentID << " 的学生。" << endl;
        }
    }

    void searchByName(string& name) {
        int found = 1;
        for (auto& student : students) {
            if (student.getName() == name) {
                cout << Student::Head()<<endl;
                cout << student.toString()<<endl;
                found = 0;
            }
        }
        if(found){
            cout << "未找到学生：" << name << endl;
        }
    }

    void searchByStudentID(int studentID) {
        auto it = find_if(students.begin(), students.end(), [&](Student& s) {
            return s.getStudentID() == studentID;
            });

        if (it != students.end()) {
            cout << Student::Head() << endl;
            cout << it->toString() << endl;
        }
        else {
            cout << "未找到学号为 " << studentID << " 的学生。" << endl;
        }
    }


    void sortByTotalScore() {
        sort(students.begin(), students.end(), [&](Student& a, Student& b) {
            return a.getTotalScore() > b.getTotalScore();
            });
        cout << "按总成绩降序排名结果：" << endl;
        cout << Student::Head() << endl;
        for (auto& student : students) {
            cout << student.toString() << endl;
        }
    }

    void sortBySubjectScore(int index) {
        if (index >= 0 && index < 3) {
            sort(students.begin(), students.end(), [&](Student& a, Student& b) {
                return a.getScore(index) > b.getScore(index);
                });
            cout << "按第 " << index + 1 << " 科成绩降序排名结果：" << endl;
            cout << Student::Head() << endl;
            for (auto& student : students) {
                cout << student.toString() << endl;
            }
        }
        else {
            cout << "无效的科目索引：" << index << endl;
        }
    }

    void viewAllStudents() {
        cout << "所有学生信息：" << endl;
        cout << Student::Head() << endl;
        for (auto& student : students) {
            cout << student.toString() << endl;
        }
    }

    void sortByStudentID() {
        sort(students.begin(), students.end());
    }

    void exportToFile(string& filename) {
        ofstream file(filename);
        if (file.is_open()) {
            file << Student::Head() << endl;
            for (auto& student : students) {
                file << student.toString() << endl;
            }
            cout << "成绩单已导出到文件：" << filename << endl;
            file.close();
        }
        else {
            cout << "无法打开文件：" << filename << "，成绩单未导出。" << endl;
        }
    }

    void importFromFile(const string& filename) {
        students.clear();

        ifstream file(filename);
        if (file.is_open()) {
            string line;
            string header;
            getline(file, header); 

            while (getline(file, line)) {
                stringstream ss(line);
                int id, chinese, math, english, total;
                string name;

                ss >> id >> name >> chinese >> math >> english >> total;

                Student newStudent(name, id, chinese, math, english);
                students.push_back(newStudent);
            }
            cout << "已从文件加载成绩单：" << filename << endl;
            file.close();
        }
        else {
            cout << "无法打开文件：" << filename << "，没有加载任何成绩信息。" << endl;
        }
    }

    void calculateScore() {
        int totalChinese = 0, totalMath = 0, totalEnglish = 0;
        int nofStudent = students.size();
        int failCountChinese = 0, failCountMath = 0, failCountEnglish = 0;
        int excellentCountChinese = 0, excellentCountMath = 0, excellentCountEnglish = 0;
        int goodCountChinese = 0, goodCountMath = 0, goodCountEnglish = 0;
        double avgChinese = 0.0, avgMath = 0.0, avgEnglish = 0.0;

        for (auto& student : students) {
            totalChinese += student.getChineseScore();
            totalMath += student.getMathScore();
            totalEnglish += student.getEnglishScore();

            if (student.getChineseScore() < 60) failCountChinese++;
            if (student.getMathScore() < 60) failCountMath++;
            if (student.getEnglishScore() < 60) failCountEnglish++;

            if (student.getChineseScore() >= 85) excellentCountChinese++;
            else if (student.getChineseScore() >= 75) goodCountChinese++;

            if (student.getMathScore() >= 85) excellentCountMath++;
            else if (student.getMathScore() >= 75) goodCountMath++;

            if (student.getEnglishScore() >= 85) excellentCountEnglish++;
            else if (student.getEnglishScore() >= 75) goodCountEnglish++;
        }

        if (nofStudent > 0) {
            avgChinese = static_cast<double>(totalChinese) / nofStudent;
            avgMath = static_cast<double>(totalMath) / nofStudent;
            avgEnglish = static_cast<double>(totalEnglish) / nofStudent;
        }

        cout << "======================" << endl;
        cout << "各科平均成绩：" << endl;
        cout << "语文平均成绩： " << fixed << setprecision(2) << avgChinese << endl;
        cout << "数学平均成绩： " << fixed << setprecision(2) << avgMath << endl;
        cout << "英语平均成绩： " << fixed << setprecision(2) << avgEnglish << endl;
        cout << "======================" << endl;
        cout << "各科不及格比例：" << endl;
        cout << "语文不及格比例： " << fixed << setprecision(2) << static_cast<double>(failCountChinese) / nofStudent * 100 << "%" << endl;
        cout << "数学不及格比例： " << fixed << setprecision(2) << static_cast<double>(failCountMath) / nofStudent * 100 << "%" << endl;
        cout << "英语不及格比例： " << fixed << setprecision(2) << static_cast<double>(failCountEnglish) / nofStudent * 100 << "%" << endl;
        cout << "======================" << endl;
        cout << "各科优良中比例：" << endl;
        cout << "语文优秀比例： " << fixed << setprecision(2) << static_cast<double>(excellentCountChinese) / nofStudent * 100 << "%" << endl;
        cout << "语文良好比例： " << fixed << setprecision(2) << static_cast<double>(goodCountChinese) / nofStudent * 100 << "%" << endl;
        cout << "数学优秀比例： " << fixed << setprecision(2) << static_cast<double>(excellentCountMath) / nofStudent * 100 << "%" << endl;
        cout << "数学良好比例： " << fixed << setprecision(2) << static_cast<double>(goodCountMath) / nofStudent * 100 << "%" << endl;
        cout << "英语优秀比例： " << fixed << setprecision(2) << static_cast<double>(excellentCountEnglish) / nofStudent * 100 << "%" << endl;
        cout << "英语良好比例： " << fixed << setprecision(2) << static_cast<double>(goodCountEnglish) / nofStudent * 100 << "%" << endl;
        cout << "======================" << endl;
    }
};

class UI {
public:
    Score& scoreManager;
    UI(Score& sm) : scoreManager(sm) {}

    void showMenu() {
        system("cls");
        cout << "======================" << endl;
        cout << "学生成绩管理系统" << endl;
        cout << "======================" << endl;
        cout << "1. 添加学生" << endl;
        cout << "2. 删除学生" << endl;
        cout << "3. 修改学生成绩" << endl;
        cout << "4. 查询学生" << endl;
        cout << "5. 按总成绩排名" << endl;
        cout << "6. 按某科成绩排名" << endl;
        cout << "7. 查看所有学生信息" << endl;
        cout << "8. 导出成绩单到文件" << endl;
        cout << "9. 从文件导入成绩单" << endl;
        cout << "10. 成绩分析" << endl;
        cout << "0. 退出" << endl;
        cout << "======================" << endl;
    }

    void run() {
        int choice;
        bool dataSaved = false;

        do {
            showMenu();
            cout << "请输入操作编号：";
            cin >> choice;
            cin.ignore();

            switch (choice) {
            case 1:
                addStudent();
                cout << "按任意键返回菜单" << endl;
                getchar();
                break;
            case 2:
                deleteStudent();
                cout << "按任意键返回菜单" << endl;
                getchar();
                getchar();
                break;
            case 3:
                changeStudentScore();
                cout << "按任意键返回菜单" << endl;
                getchar();
                getchar();
                break;
            case 4:
                int i;
                cout << "0.通过姓名查找  1.通过学号查找" << endl;
                cin >> i;
                if(i==0)
                searchStudentByName();
                if (i == 1)
                searchStudentByID();
                cout << "按任意键返回菜单" << endl;
                getchar();
                break;
            case 5:
                sortByTotalScore();
                cout << "按任意键返回菜单" << endl;
                getchar();
                break;
            case 6:
                sortBySubjectScore();
                cout << "按任意键返回菜单" << endl;
                getchar();
                getchar();
                break;
            case 7:
                viewAllStudents();
                cout << "按任意键返回菜单" << endl;
                getchar();
                break;
            case 8:
                exportToFile();
                dataSaved = true;
                cout << "按任意键返回菜单" << endl;
                getchar();
                break;
            case 9: {
                string filename;
                cout << "请输入要导入成绩单的文件名：";
                getline(cin, filename);
                importFromFile(filename);
                cout << "按任意键返回菜单" << endl;
                getchar();
                break;
            }
            case 10:
                scoreManager.calculateScore();
                cout << "按任意键返回菜单" << endl;
                getchar();
                break;
            case 0:
                if (!dataSaved) {
                    char saveChoice;
                    cout << "数据尚未保存到文件，是否保存？(Y/N): ";
                    cin >> saveChoice;
                    if (saveChoice == 'Y' || saveChoice == 'y') {
                        cin.ignore();
                        exportToFile();
                    }
                }
                cout << "退出学生成绩管理系统。" << endl;
                break;
            default:
                cout << "无效的操作编号，请重新输入。" << endl;
            }
        } while (choice != 0);
    }

    void addStudent() {
        string name;
        int studentID, chinese, math, english;
        while (1) {
            cout << "请输入学生姓名：";
            getline(cin, name);

            while (1) {
                cout << "请输入学生学号：";
                cin >> studentID;
                cin.ignore();
                if (scoreManager.studentIDs.find(studentID) == scoreManager.studentIDs.end())
                    break;
                cout << "学号 " << studentID << " 已存在，请重新输入学号。" << endl;
            }

            while (1) {
                cout << "请输入语文成绩：";
                cin >> chinese;
                if (chinese >= 0 && chinese <= 100) 
                    break;
                cout << "成绩不合法，请输入0到100之间的成绩。" << endl;
            }

            while (1) {
                cout << "请输入数学成绩：";
                cin >> math;
                if (math >= 0 && math <= 100) 
                    break;
                cout << "成绩不合法，请输入0到100之间的成绩。" << endl;
            }

            while (1) {
                cout << "请输入英语成绩：";
                cin >> english;
                if (english >= 0 && english <= 100) 
                    break;
                cout << "成绩不合法，请输入0到100之间的成绩。" << endl;
            }

            Student newStudent(name, studentID, chinese, math, english);
            scoreManager.addStudent(newStudent);
            cout << "已添加学生：" << name << endl;

            int continueInput;
            cout << "是否继续输入？1 是，0 否：";
            cin >> continueInput;
            cin.ignore();
            if (continueInput == 0) {
                break;
            }
        }
    }

    void deleteStudent() {
        int studentID;
        cout << "请输入要删除的学生学号：";
        cin >> studentID;
        scoreManager.deleteStudent(studentID);
    }

    void changeStudentScore() {
        int id;
        cout << "请输入要修改成绩的学生学号：";
        cin>>id;

        auto it = find_if(scoreManager.students.begin(), scoreManager.students.end(), [&](Student& s) {
            return s.getStudentID() == id;
            });

        if (it != scoreManager.students.end()) {
            cout << "当前学生 " << id << " 的成绩：" << endl;
            cout << Student::Head() << endl;
            cout << it->toString() << endl;

            int index;
            while (true) {
                cout << "请选择要修改的科目（0: 语文, 1: 数学, 2: 英语）：";
                cin >> index;
                if (index >= 0 && index < 3) break;
                cout << "无效的科目索引，请重新输入。" << endl;
            }

            int newScore;
            while (true) {
                cout << "请输入新的成绩：";
                cin >> newScore;
                if (newScore >= 0 && newScore <= 100) break;
                cout << "成绩不合法，请输入0到100之间的成绩。" << endl;
            }

            cin.ignore();
            scoreManager.changeStudentScore(id, index, newScore);
        }
        else {
            cout << "未找到学生：" << id << endl;
        }
    }

    void searchStudentByName() {
        string name;
        cout << "请输入要查询的学生姓名：";
        cin >> name;
        cin.ignore();
        scoreManager.searchByName(name);
    }

    void searchStudentByID(){
        int studentID;
        cout << "请输入要查询的学生学号：";
        cin >> studentID;
        cin.ignore();
        scoreManager.searchByStudentID(studentID);
    }

    void sortByTotalScore() {
        scoreManager.sortByTotalScore();
    }

    void sortBySubjectScore() {
        int index;
        cout << "请输入要按第几科成绩排序（0: 语文, 1: 数学, 2: 英语）：";
        cin >> index;
        scoreManager.sortBySubjectScore(index);
    }

    void viewAllStudents() {
        scoreManager.viewAllStudents();
    }

    void exportToFile() {
        string filename;
        cout << "请输入要导出成绩单的文件名：";
        getline(cin, filename);
        scoreManager.exportToFile(filename);
    }

    void importFromFile(const string& filename) {
        scoreManager.importFromFile(filename);
    }
};

int main() {
    Score scoreManager;
    UI ui(scoreManager);
    ui.run();
    return 0;
}