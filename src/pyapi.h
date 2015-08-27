#ifndef PYAPI_H
#define PYAPI_H

/********************
 ** API for python **
 ********************/

#include <QObject>
#include <Python.h>

class QNetworkReply;
class QTimer;

//////define get_url function for Python
class GetUrl : public QObject
{
    Q_OBJECT
public:
    explicit GetUrl(QObject *parent = 0);
    void start(const char *url, PyObject *callback, PyObject *_data);
    inline bool hasTask(){return callbackFunc != NULL;}
private:
    PyObject *callbackFunc;
    PyObject *data;
    QNetworkReply *reply;
    QTimer *timer;
private slots:
    void onFinished(void);
    void onTimeOut(void);
};
extern GetUrl *geturl_obj;
extern PyObject *exc_GetUrlError;

///////Module
extern PyObject *apiModule;
void initAPI(void);
void call_py_func_vsi(PyObject *func, const char *first, int second);

#define OPT_QL_HIGH  (1)
#define OPT_QL_SUPER (1 << 1)
#define OPT_DOWNLOAD  (1 << 2)
#define LIBTYPE_TV 0
#define LIBTYPE_MOVIE 1

#endif // PYAPI_H
