#ifndef PROCESSID_H
#define PROCESSID_H

/*
 * Вспомогательные структуры для работы с процессами
 * других приложений и запуска этих приложений
 *
 * Version 1.5
 *
 * Writed by Brylkin Dmitry. 01.02.2019
 *
 * Last changed by Brylkin Dmitry. 22.02.2019
 */


#include <QMessageBox>
#include <QWidget>
#include <QProcess>
#include <QMap>

/*
 * Перечисление программ, входящих в комплекс
 */
namespace Programs {
    enum ProgramsEnum {
        IR,
        SlagsConverter,
        SetFilesMaker,
        StateComparison
    };
}

/*
 * Структура ProcessInfo
 *
 * Содержит информацию о названии программ, которые будут выполняться
 * и о работающих в данный момент процессах
 *
 */
struct ProcessInfo {
    ProcessInfo() : pid(-1), programName("") {}
    explicit ProcessInfo(const qint64& _pid, const QString& _programName)
             : pid(_pid)
             , programName(_programName) {}
    qint64 pid;
    QString programName;
};

/*
 * Сlass Find_last_version
 *
 * Функтор для сравнения версий программ - выбирает максимальную
 */
class Find_last_version {
public:
    // конструктор принимает ссылку на информацию о программе
    explicit Find_last_version(const ProcessInfo& _processInfo) : processInfo(_processInfo) {}
    // перегруженный operator() - функция сравнения двух названий приложения
    // для выбора приложения с максимальной версией
    bool operator()(const QString& lhs, const QString& rhs);

private:
    //ПОЛЯ
    // информация о программе
    const ProcessInfo& processInfo;

    // МЕТОДЫ
    // определяет версию программы из текста
    static QMap<int, int> programNameToDigits(const QString& name);
};

// ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ
// сопоставление перечисления программ и информации о них
extern QMap<Programs::ProgramsEnum, ProcessInfo> ProcessID;

// ФУНКЦИИ
// инициализация названий программ
void initProcessID(void);
// проверка существования процесса
bool processExists(const qint64& pid);
// запуск нового процесса
bool startNewProcess(ProcessInfo& processInfo, QWidget * parent);

// Макрос VER_IDENT - текстовая замена на указанный идентификатор наличия версии в названии приложения
#define VER_IDENT "-ver."

#endif // PROCESSID_H
