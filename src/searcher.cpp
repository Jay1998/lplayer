#include "searcher.h"
#include "utils.h"
#include <QDir>
#ifdef Q_OS_WIN
#include "settings_player.h"
#endif

/************************
 ** Initialize plugins **
 ************************/
int n_searchers = 0;
Searcher **searchers = NULL;

void initSearchers()
{
    //load plugins
    static Searcher *array[128];
    searchers = array;
#ifdef Q_OS_WIN
    QDir pluginsDir = QDir(Settings::path);
    pluginsDir.cd("plugins");
    QStringList list = pluginsDir.entryList(QDir::Files, QDir::Name);
#else
    QDir pluginsDir = QDir("/usr/share/lplayer/plugins");
    QStringList list = pluginsDir.entryList(QDir::Files, QDir::Name);
    pluginsDir = QDir::home();
    pluginsDir.cd(".lplayer");
    pluginsDir.cd("plugins");
    list += pluginsDir.entryList(QDir::Files, QDir::Name);
#endif
    while (!list.isEmpty())
    {
        QString filename = list.takeFirst();
        if (filename.startsWith("searcher_") && filename.endsWith(".py"))
        {
            array[n_searchers] = new Searcher(filename.section('.', 0, 0));
            n_searchers++;
        }
    }
}

/*********************
 ** Searcher object **
 ********************/
Searcher::Searcher(const QString &moduleName)
{
    //load module
    module = PyImport_ImportModule(moduleName.toUtf8().constData());
    if (module == NULL)
    {
        PyErr_Print();
        exit(-1);
    }

    // Get name
    PyObject *_name = PyObject_GetAttrString(module, "searcher_name");
    if (_name == NULL)
    {
        PyErr_Print();
        exit(EXIT_FAILURE);
    }
    name = PyString_AsQString(_name);
    Py_DecRef(_name);
    _name = NULL;

    //get search() function
    searchFunc = PyObject_GetAttrString(module, "search");
    if (searchFunc == NULL)
    {
        PyErr_Print();
        exit(-1);
    }
}

void Searcher::search(const QString &kw, int page)
{
    PyObject *result = PyObject_CallFunction(searchFunc, "si", kw.toUtf8().constData(), page);
    if (result)
        Py_DecRef(result);
    else
        PyErr_Print();
}
