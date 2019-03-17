/*
 * Вспомогательные структуры для работы с процессами
 * других приложений и запуска этих приложений (реализация)
 *
 * Version 1.7
 *
 * Writed by Brylkin Dmitry. 01.02.2019
 *
 * Last changed by Brylkin Dmitry. 22.02.2019
 */

#include <QDir>
#include <QApplication>
#include <algorithm>
#include "processid.h"
#include "inidata.h"

// инициализация названий программ
void initProcessID(void)
{
    ProcessID.insert(Programs::IR, ProcessInfo(-1, "IR"));
    ProcessID.insert(Programs::SlagsConverter, ProcessInfo(-1, "SlagsConverter"));
    ProcessID.insert(Programs::SetFilesMaker, ProcessInfo(-1, "SetFilesMaker"));
    ProcessID.insert(Programs::StateComparison, ProcessInfo(-1, "StateComparison"));
}

// проверка существования процесса
// processExists implementations (platform dependency)
#ifdef _WIN32
// Windows implementation
#include <windows.h>

bool processExists(qint64 pid)
{
    // пытаемся создать обработчик процесса
    HANDLE hProcess = OpenProcess(SYNCHRONIZE, FALSE, pid);
    // если успешно - процесс существует - закрываем обработчик и возвращаем true
    if (hProcess != NULL) {
      CloseHandle(hProcess);
      return true;
    }
    // Если код ошибки - доступ запрещен, процесс существует, но мы не можем получить к нему доступ.
    return GetLastError() == ERROR_ACCESS_DENIED;
}

#elif defined __unix__
// Unix implementation
#include <sys/types.h>
#include <signal.h>

bool processExists(qint64 pid) { return 0 == kill(pid,0); }// отправка пустого сигнала

#else
// Unknown system determined
bool processExists(qint64 pid) { return false; }// возвращаем false

#endif

// статичский метод функтора Find_last_version: определяет версию программы из текста
QMap<int, int> Find_last_version::programNameToDigits(const QString& name) {
    // ЛОКАЛЬНЫЕ ПЕРЕМЕННЫЕ
    // предыдущий символ - число? (да, нет)
    bool prevDigit = false;
    // счетчик разделенных символами чисел
    int i = 0;
    // первый символ после идентификатора версии
    int lhs_ver_pos = name.indexOf(VER_IDENT) + QString(VER_IDENT).length() + 1;
    /*
     * словарь "номер числа в версии - число"
     *
     * Пример: IR-ver.2.8.3.10
     *
     *         1 - 2
     *         2 - 8
     *         3 - 3
     *         4 - 10
     */
    QMap<int, int> digits;

    // итерируемся до конца строки
    for (; lhs_ver_pos < name.size(); ++lhs_ver_pos) {
        // проверяем, является ли символ - числом
        if (name[lhs_ver_pos].isDigit()) {
            // если символ - число
            // увеличиваем порядок текущего числа и добавляем в конец новое
            digits[i] = digits[i]*10 + name[lhs_ver_pos].digitValue();
            // для следующего символа: предыдущий - число
            prevDigit = true;
        } else if (prevDigit) {
            // если не число
            // прибавляем счетчик номеров
            ++i;
            // предыдущий - не число
            prevDigit = false;
        } // иначе на следующую итерацию
    }

    // возвращаем словарь "номер числа в версии - число"
    return digits;
}

// перегруженный operator() - функция сравнения двух названий приложения
// для выбора приложения с максимальной версией
bool Find_last_version::operator()(const QString &lhs, const QString &rhs)
{
    // если имя левого или правого операндов точно совпадает с заданным именем программы - оно в приоритете
    if (lhs == processInfo.programName || lhs == processInfo.programName + ".exe") return true;
    else if (rhs == processInfo.programName || rhs == processInfo.programName + ".exe") return false;
    // проверяем наличие идентификатора версии
    if (lhs.contains(VER_IDENT) && rhs.contains(VER_IDENT)) {
        // если у обоих операндов есть идентификатор
        // пробуем найти максимальную версию
        // парсим имя файла на номер версии для каждого операнда
        QMap<int,int> lhs_digits = programNameToDigits(lhs);
        QMap<int,int> rhs_digits = programNameToDigits(rhs);

        // если чисел после версии нет - приоритет опускаем
        if (rhs_digits.empty()) return true;
        else if (lhs_digits.empty()) return false;
        // итерируемся одновременно по двум словарям, сравнивая числы версий
        // если находим большее число - то выбираем эту версию
        // если все числа были одинаковыми, а в одном словаре они закончились,
        // то выбираем этот операнд
        auto lit = lhs_digits.begin();
        auto rit = rhs_digits.begin();
        while (lit != lhs_digits.end() || rit != rhs_digits.end()) {
            if (lit.value() > rit.value()) {
                return true;
            } else if (lit.value() < rit.value()) {
                return false;
            } else if (++lit == lhs_digits.end()) {
                return true;
            } else if (++rit == rhs_digits.end()) {
                return false;
            }
        }
        // сравнение по символам
        return lhs < rhs;
    } else if (lhs.contains(VER_IDENT)) {
        // если у правого операнда нет идентификатора версии, а у левого есть
        return true;
    } else if (rhs.contains(VER_IDENT)) {
        // если у правого операнда есть идентификатор версии, а у левого нет
        return false;
    } else {
        // если у обоих операндов нет идентификатора версии
        return lhs < rhs;
    }
}

// запуск нового процесса
bool startNewProcess(ProcessInfo& processInfo, QWidget * parent)
{
    // проверяем pid
    if (processInfo.pid != -1)
    {
        // если pid не -1 (программа с таким именем не запускалась),
        // то проверяем, существует ли процесс с указанным pid-ом
        if (processExists(processInfo.pid))
        {
            // если он существует - выдаем предупреждение о том, что нельзя запускать 2 копии одной программы и выходим
            QMessageBox::warning(parent, QMessageBox::tr("Warning!"),
                                 QMessageBox::tr("Program %1 is already running. " \
                                                 "If you need to run a new copy of this program - " \
                                                 "use one more instance of IR-tools.")
                                                 .arg(processInfo.programName),
                                 QMessageBox::Ok);
            return false;
        }
    }
    // проверяем правильность загрузки
    if (!IniData.TruePath)
    {
        // если загрузка неверная - выдаем предупреждение об этом
        QMessageBox::warning(parent, QMessageBox::tr("Warning!"),
                             QMessageBox::tr("The Fuel Load is not selected. The program will be opened in the IR-tools path."),
                             QMessageBox::Ok);
    }
    // если загрузка указана, но директория не существует
    if (IniData.TruePath && !QDir(IniData.Path).exists())
    {
        // то указываем, что загрузка неверная, ставим новый путь и выдаем предупреждение
        IniData.TruePath = false;
        IniData.Path = "./";
        QMessageBox::warning(parent, QMessageBox::tr("Warning!"),
                             QMessageBox::tr("The currently selected path to fuel load doesn't exist. The program will be opened in the IR-tools path."),
                             QMessageBox::Ok);
    }
    // если запускаемая программа - ИР
    if (processInfo.programName.contains("IR"))
    {
        // проверяем наличие всех необходимых для его запуска файлов и директорий
        QPair<DoesntExist::DoesntExistEnum, QString> checkPathIRresult = checkPathIR();
        // проверяем где должен находиться отсутствующий файл (по соответствующему полю перечисления DoesntExist)
        // если отсутствующего файла нет - продолжаем работу, иначе выводим соответствующее сообщение и выходим
        if (checkPathIRresult.first == DoesntExist::DLL) {
            // если он должен быть в директории IR-tools
            QMessageBox::critical(parent, QMessageBox::tr("Error!"),
                                  QMessageBox::tr("The IR-tools path doesn't contain %1, necessary for IR. Add %1 to the IR-tools path and run application again.")
                                  .arg(checkPathIRresult.second),
                                  QMessageBox::Ok);
            return false;
        } else if (checkPathIRresult.first == DoesntExist::SET) {
            // если он должен быть в директории загрузки
            QMessageBox::critical(parent, QMessageBox::tr("Error!"),
                                  QMessageBox::tr("The currently selected path to fuel load doesn't contain %1, necessary for IR. Try to change path to fuel load or add %1.")
                                  .arg(checkPathIRresult.second),
                                  QMessageBox::Ok);
            return false;
        }
    }
    // пытаемся записать .ini файл
    if (!writeIni()) {
        // если не вышло - выдаем сообщение об ошибке и выходим
        QMessageBox::critical(parent, QMessageBox::tr("Error!"),
                              QMessageBox::tr("Error during writing .ini file. It is locked by another process." \
                                              "Try to execute program again when this file will be unlocked by that process."),
                              QMessageBox::Ok);
        return false;
    }
    // формируем списох файлов, которые имеют начало названия такое же, какое должно быть у программы
    QStringList executables = QDir("./").entryList(QStringList(processInfo.programName+"*"), QDir::Files | QDir::NoSymLinks);

    // удаляем те, которые содержат имя текущей программы
    executables.erase(std::remove_if(
                          executables.begin() ,
                          executables.end(),
                          [] (const QString& filename) {
                              return filename.toLower().contains(QApplication::applicationName().toLower());
                          }),
                      executables.end());
    // если список пуст - выдаем сообщение об ошибке и выходим
    if (executables.empty()) {
        QMessageBox::critical(parent, QMessageBox::tr("Error!"),
                              QMessageBox::tr("Error during executing program %1. " \
                                              "Check existance of the executable file of this application in IRtools directory.")
                                              .arg(processInfo.programName),
                              QMessageBox::Ok);
        return false;
    }
    // если список не пуст - сортируем его с помощью функтора Find_last_version
    std::sort(executables.begin(), executables.end(), Find_last_version(processInfo));

    // итерируемся по списку и пытаемся запустить каждый файл по очереди
    // выходим из цикла, когда это удалось или по окончанию списка
    qint64 new_pid = -1;
    bool ProcessExists = false;
    for (auto it = executables.begin(); it != executables.end() && !ProcessExists; ++it) {
        new_pid = -1;
        bool ProcessCreated = QProcess::startDetached(*it,QStringList(),"./",&new_pid);
        ProcessExists = false;
        if (ProcessCreated) ProcessExists = processExists(new_pid);
    }
    // проверяем, удалось ли запустить программу
    if (ProcessExists)
    {
        // если удалось - сохраняем pid
        processInfo.pid = new_pid;
        return true;
    } else {
        // если не удалось - выводим сообщение об ошибке и выходим
        QMessageBox::critical(parent, QMessageBox::tr("Error!"),
                              QMessageBox::tr("Error during executing program %1. Try to start program by its executable file.")
                              .arg(processInfo.programName),
                              QMessageBox::Ok);
        return false;
    }
}
