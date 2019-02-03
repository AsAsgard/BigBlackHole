#include <QDir>
#include "processid.h"
#include "inidata.h"

void InitProcessID(void)
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
        QPair<bool, QString> checkPathIRresult = checkPathIR();
        if (!checkPathIRresult.first)
        {
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
    qint64 new_pid = -1;
    bool ProcessCreated = QProcess::startDetached(processInfo.programName,QStringList(),"./",&new_pid);
    bool ProcessExists = false;
    if (ProcessCreated) ProcessExists = processExists(new_pid);
    if (ProcessExists)
    {
        processInfo.pid = new_pid;
        return true;
    } else {
        QMessageBox::critical(parent, QMessageBox::tr("Error!"),
                              QMessageBox::tr("Error during executing program %1. See the log file for details.")
                              .arg(processInfo.programName),
                              QMessageBox::Ok);
        return false;
    }
}
