#ifndef COMPARISONFIELD_H
#define COMPARISONFIELD_H

#include <QLabel>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QMainWindow>
#include "ui_comparisonfield.h"
#include "kv_distribution.h"
#include "renderarea.h"
#include "filebrowser.h"
#include "cf_colorchanger.h"

// класс формы - основное окно с картограммой
class ComparisonField : public QMainWindow
{
	Q_OBJECT

public:
	// конструктор и деструктор
	explicit ComparisonField(QWidget *parent, const cDataState &rState1, const cDataState &rState2,const QRect &ScreenParameters);
	~ComparisonField();

signals:
	// изменить режим просмотра
	void ViewModeChange(bool);
	// изменить состояния
	void ChangeStatesCF(const cDataState& State1, const cDataState& State2);
	// выбор новой ТВС для гистограммы Kv
	void select_FA(int);
	// необходимо перезадать параметры оси X гистограммы Kv
	void resetAxisNeeded();
	// необходимо спрятать линию слоя на гистограмме Kv
	void hideLayerLine();
	// отправить указатель на поле рисования
	void sendRenderAreaPointer(QWeakPointer<RenderArea>);

protected:
    void resizeEvent(QResizeEvent *) override;

private slots:
	// выбрать другие состояния
	void on_otherStates_triggered();
	// активировать гистограмму Kv
	void on_Kv_diagramActivator_triggered();
	// сделать скрин картограммы
	void on_Screenshot_triggered();
	// выбор показа активного параметра
	void on_activateKq_triggered();
	void on_activateBurn_triggered();
	void on_activateKv_triggered();
	// показать максимальное отклонение
	void on_MaxShow_triggered();
	// открыть окно настройки картограммы
	void on_ChangeColors_triggered();
	// изменить режим просмотра
	void on_ChangeViewMode_toggled(bool checked);
	// включить/выключить цвета
	void on_ColorFont_toggled(bool checked);
	// изменение значения слоя Kv
    void on_spinBox_valueChanged(int);
	// ответ на запрос активности гистограммы Kv
	bool KvDiagramAvaliability() const;
	// Гистограмма Kv была закрыта
	void KvDiagramClosed(const QPen& Kv1PenToSave, const QPen& Kv2PenToSave, const AutoAxis::AutoAxisEnum& KvAutoAxisToSave);
	// окно настройки картограммы было закрыто
	void ColorChangerClosed();
	// окно выбора файлов состояний было закрыто
	void FileBrowserClosed();
	// ответ на запрос активности окна с картограммой
	bool ComparisonFieldAvaliability() const;
	// пришел сигнал с изменением состояний
	void StatesChanged(const cDataState &rState1, const cDataState &rState2);
	// перекрасить поле рисования
	void RecolorAll();

private:
	// ПОЛЯ
	Ui::ComparisonFieldClass ui;
	// форма гистограммы Kv
	QScopedPointer<Kv_Distribution> KvDistrib;
	// область рисования
	QSharedPointer<RenderArea> renderArea;
	// дополнитльный Label в статус-бар
	QScopedPointer<QLabel> statusLabel;
	// форма настройки параметров картограммы
	QScopedPointer<CF_ColorChanger> ColorChanger;
	// форма выбора состояний
	QScopedPointer<FileBrowser> fileBrowser;

	// сохраненные параметры рисования для гистограммы Kv
	QPen Kv1SavedPen;
	QPen Kv2SavedPen;
	AutoAxis::AutoAxisEnum KvSavedAutoAxis;

	// МЕТОДЫ
	// обновить статус-бар
	void StatusBarUpdate(void);
};

#endif // COMPARISONFIELD_H
