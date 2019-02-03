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

extern QMap<Programs::ProgramsEnum, ProcessInfo> ProcessID;

void InitProcessID(void);
bool processExists(const qint64& pid);
bool startNewProcess(ProcessInfo& processInfo, QWidget * parent);

#endif // PROCESSID_H
