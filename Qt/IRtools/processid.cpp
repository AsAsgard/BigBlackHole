#include <QDir>
#include <algorithm>
#include "processid.h"
#include "inidata.h"

void initProcessID(void)
{
    ProcessID.insert(Programs::IR, ProcessInfo(-1, "IR"));
    ProcessID.insert(Programs::SlagsConverter, ProcessInfo(-1, "SlagsConverter"));
    ProcessID.insert(Programs::SetFilesMaker, ProcessInfo(-1, "SetFilesMaker"));
    ProcessID.insert(Programs::StateComparison, ProcessInfo(-1, "StateComparison"));
}

// processExists implementations (platform dependency)
#ifdef _WIN32
// Windows implementation
#include <windows.h>

bool processExists(const qint64& pid)
{
    HANDLE hProcess = OpenProcess(SYNCHRONIZE, FALSE, pid);
    if (hProcess != NULL) {
      CloseHandle(hProcess);
      return true;
    }
    // If the error code is access denied, the process exists but we don't have access to open a handle to it.
    return GetLastError() == ERROR_ACCESS_DENIED;
}

#elif defined __unix__
// Unix implementation
#include <sys/types.h>
#include <signal.h>

bool processExists(qint64 pid) { return 0 == kill(pid,0);}

#else
// Unknown system determined
bool processExists(qint64 pid) { return false;}

#endif


QMap<int, int> Find_last_version::programNameToDigits(const QString& name) {
    bool prevDigit = false;
    int i = 0;
    int lhs_ver_pos = name.indexOf(VER_IDENT) + QString(VER_IDENT).length() + 1;
    QMap<int, int> digits;

    for (; lhs_ver_pos < name.size(); ++lhs_ver_pos) {
        if (name[lhs_ver_pos].isDigit()) {
            digits[i] = digits[i]*10 + name[lhs_ver_pos].digitValue();
            prevDigit = true;
        } else if (prevDigit) {
            ++i;
            prevDigit = false;
        }
    }

    return digits;
}


bool Find_last_version::operator()(const QString &lhs, const QString &rhs)
{
    if (lhs == processInfo.programName || lhs == processInfo.programName + ".exe") return true;
    else if (rhs == processInfo.programName || rhs == processInfo.programName + ".exe") return false;
    if (lhs.contains(VER_IDENT) && rhs.contains(VER_IDENT)) {
        // пробуем найти максимальную версию
        QMap<int,int> lhs_digits = programNameToDigits(lhs);
        QMap<int,int> rhs_digits = programNameToDigits(rhs);

        if (rhs_digits.empty()) return true;
        else if (lhs_digits.empty()) return false;
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
        return lhs < rhs;
    } else if (lhs.contains(VER_IDENT)) {
        return true;
    } else if (rhs.contains(VER_IDENT)) {
        return false;
    } else {
        return lhs < rhs;
    }
}


bool startNewProcess(ProcessInfo& processInfo, QWidget * parent)
{
    if (processInfo.pid != -1)
    {
        if (processExists(processInfo.pid))
        {
            QMessageBox::warning(parent, QMessageBox::tr("Warning!"),
                                 QMessageBox::tr("Program %1 is already running. " \
                                                 "If you need to run a new copy of this program - " \
                                                 "use one more instance of IR-tools.")
                                                 .arg(processInfo.programName),
                                 QMessageBox::Ok);
            return false;
        }
    }
    if (!IniData.TruePath)
    {
        QMessageBox::warning(parent, QMessageBox::tr("Warning!"),
                             QMessageBox::tr("The Fuel Load is not selected. The program will be opened in the IR-tools path."),
                             QMessageBox::Ok);
    }
    if (IniData.TruePath && !QDir(IniData.Path).exists())
    {
        IniData.TruePath = false;
        IniData.Path = "./";
        QMessageBox::warning(parent, QMessageBox::tr("Warning!"),
                             QMessageBox::tr("The currently selected path to fuel load doesn't exist. The program will be opened in the IR-tools path."),
                             QMessageBox::Ok);
    }
    if (processInfo.programName.contains("IR"))
    {
        QPair<DoesntExist::DoesntExistEnum, QString> checkPathIRresult = checkPathIR();
        if (checkPathIRresult.first == DoesntExist::DLL) {
            QMessageBox::critical(parent, QMessageBox::tr("Error!"),
                                  QMessageBox::tr("The IR-tools path doesn't contain %1, necessary for IR. Add %1 to the IR-tools path and run application again.")
                                  .arg(checkPathIRresult.second),
                                  QMessageBox::Ok);
            return false;
        } else if (checkPathIRresult.first == DoesntExist::SET) {
            QMessageBox::critical(parent, QMessageBox::tr("Error!"),
                                  QMessageBox::tr("The currently selected path to fuel load doesn't contain %1, necessary for IR. Try to change path to fuel load or add %1.")
                                  .arg(checkPathIRresult.second),
                                  QMessageBox::Ok);
            return false;
        }
    }
    if (!writeIni()) {
        QMessageBox::critical(parent, QMessageBox::tr("Error!"),
                              QMessageBox::tr("Error during writing .ini file. It is locked by another process." \
                                              "Try to execute program again when this file will be unlocked by that process."),
                              QMessageBox::Ok);
        return false;
    }
    QStringList executables = QDir("./").entryList(QStringList(processInfo.programName+"*"), QDir::Files | QDir::NoSymLinks);

    executables.erase(std::remove_if(
                          executables.begin() ,
                          executables.end(),
                          [] (const QString& filename) {
                              return filename.toLower().contains(thisProgramName.toLower());
                          }),
                      executables.end());
    if (executables.empty()) {
        QMessageBox::critical(parent, QMessageBox::tr("Error!"),
                              QMessageBox::tr("Error during executing program %1. " \
                                              "Check existance of the executable file of this application in IRtools directory.")
                                              .arg(processInfo.programName),
                              QMessageBox::Ok);
        return false;
    }

    std::sort(executables.begin(), executables.end(), Find_last_version(processInfo));

    qint64 new_pid = -1;
    bool ProcessExists = false;
    for (auto it = executables.begin(); it != executables.end() && !ProcessExists; ++it) {
        new_pid = -1;
        bool ProcessCreated = QProcess::startDetached(*it,QStringList(),"./",&new_pid);
        ProcessExists = false;
        if (ProcessCreated) ProcessExists = processExists(new_pid);
    }
    if (ProcessExists)
    {
        processInfo.pid = new_pid;
        return true;
    } else {
        QMessageBox::critical(parent, QMessageBox::tr("Error!"),
                              QMessageBox::tr("Error during executing program %1. Try to start program by its executable file.")
                              .arg(processInfo.programName),
                              QMessageBox::Ok);
        return false;
    }
}
