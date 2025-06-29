// scada_viewmodel.cpp
#include "scada_viewmodel.h"

ScadaViewModel::ScadaViewModel(ClientData &data, QObject *parent)
    : QObject(parent), data_(data) {
    startTimer(500);
}

int ScadaViewModel::analog0() const { return data_.analog[0]; }
int ScadaViewModel::analog1() const { return data_.analog[1]; }
bool ScadaViewModel::digital0() const { return data_.digital[0]; }
bool ScadaViewModel::digital1() const { return data_.digital[1]; }

void ScadaViewModel::timerEvent(QTimerEvent*) {
    emit valuesChanged();
}
