#ifndef PROCESSID_H
#define PROCESSID_H

#include <QMessageBox>
#include <QWidget>
#include <QProcess>
#include <QMap>

namespace Programs {
    enum ProgramsEnum {
        IR,
        SlagsConverter,
        SetFilesMaker,
        StateComparison
    };
}

struct ProcessInfo {
    ProcessInfo() : pid(-1), programName("") {}
    explicit ProcessInfo(const qint64& _pid, const QString& _programName)
             : pid(_pid)
             , programName(_programName) {}
    qint64 pid;
    QString programName;
};

class Find_last_version {
public:
    explicit Find_last_version(const ProcessInfo& _processInfo) : processInfo(_processInfo) {}
    bool operator()(const QString& lhs, const QString& rhs);

private:
    const ProcessInfo& processInfo;
};

extern QMap<Programs::ProgramsEnum, ProcessInfo> ProcessID;
extern QString thisProgramName;

void InitProcessID(void);
bool processExists(const qint64& pid);
bool startNewProcess(ProcessInfo& processInfo, QWidget * parent);

#define VER_IDENT "-ver."

#endif // PROCESSID_H
