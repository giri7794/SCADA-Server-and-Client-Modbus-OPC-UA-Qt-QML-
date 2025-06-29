// scada_viewmodel.h
#pragma once
#include <QObject>
#include "client_data.h"

class ScadaViewModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(int analog0 READ analog0 NOTIFY valuesChanged)
    Q_PROPERTY(int analog1 READ analog1 NOTIFY valuesChanged)
    Q_PROPERTY(bool digital0 READ digital0 NOTIFY valuesChanged)
    Q_PROPERTY(bool digital1 READ digital1 NOTIFY valuesChanged)

public:
    ScadaViewModel(ClientData &data, QObject *parent = nullptr);

    int analog0() const;
    int analog1() const;
    bool digital0() const;
    bool digital1() const;

signals:
    void valuesChanged();

protected:
    void timerEvent(QTimerEvent*) override;

private:
    ClientData &data_;
};
