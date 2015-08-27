#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QStringList>
#include <Python.h>

//Convert python's string(include unicode) to QString
QString PyString_AsQString(PyObject *pystr);
QStringList PyList_AsQStringList(PyObject *tuple);

QString secToTime(int second, bool use_format = false);

//Read .xspf playlists
void readXspf(const QByteArray& xmlpage, QStringList& result);

#endif // MP_UTILS_H
