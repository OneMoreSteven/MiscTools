#ifndef __PYTHON_RUNNER__
#define __PYTHON_RUNNER__

#ifdef _DEBUG
#undef _DEBUG
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif

#include <memory>
#include <vector>
#include <string>

typedef std::shared_ptr<class CPythonObject> POH;


class CPythonObject
{
	bool m_bAutoRelease;
	PyObject *m_pObj;
public:
	CPythonObject(PyObject *pObj, bool bAutoRelease=true) : m_pObj(pObj), m_bAutoRelease(bAutoRelease) {};
	~CPythonObject()
	{
		if (m_bAutoRelease)
			Py_XDECREF(m_pObj);
	}
	POH GetAttr(const char* szAttr)
	{
		PyObject *pAttr = PyObject_GetAttrString(m_pObj, szAttr);
		std::shared_ptr<CPythonObject> po(new CPythonObject(pAttr));
		return po;
	}
	bool SetAttr(const char* szAttr, POH Value)
	{
		return PyObject_SetAttrString(m_pObj, szAttr, Value.get()->m_pObj) == 0;
	}
	long AsInt()
	{
		return PyInt_AsLong(m_pObj);
	}
	double AsFloat()
	{
		return PyFloat_AsDouble(m_pObj);
	}
	std::string ToString()
	{
		PyObject *pStr = PyObject_Str(m_pObj);
		std::string r(PyString_AsString(pStr));
		Py_XDECREF(pStr);
		return r;
	}
};


class CPythonRunner
{
public:
	CPythonRunner(char* szPyHome=NULL)
	{
		if (szPyHome)
			Py_SetPythonHome(szPyHome);
		Py_Initialize();
		TRACE("Python initialized\n");
	}
	virtual ~CPythonRunner()
	{
		Py_Finalize();
	}
	int RunString(const char* szCommand)
	{
		return PyRun_SimpleString(szCommand);
	}
	POH ImportModule(const char* szModule)
	{
		PyObject *pModule = PyImport_ImportModule(szModule);
		std::shared_ptr<CPythonObject> po(new CPythonObject(pModule));
		return po;
	}
	POH GetModule(const char* szModule)
	{
		PyObject *pModule = PyImport_AddModule(szModule);
		std::shared_ptr<CPythonObject> po(new CPythonObject(pModule));
		return po;
	}
	POH GetMainModule()
	{
		PyObject *pModule = PyImport_AddModule("__main__");
		std::shared_ptr<CPythonObject> po(new CPythonObject(pModule,false));
		return po;
	}
};


template<class T>
inline PyObject* PyObject_From(T v) {}

template<>
inline PyObject* PyObject_From(float v) { return PyFloat_FromDouble(v); }

template<>
inline PyObject* PyObject_From(int v) { return PyInt_FromLong(v); }

template<>
inline PyObject* PyObject_From(const char* v) { return PyString_FromString(v); }


template<class T>
inline POH VectorToList(std::vector<T>& v)
{
	PyObject* list = PyList_New(0);
	for (auto it = v.begin(); it != v.end(); it++)
	{
		PyObject* item = PyObject_From(*it);
		PyList_Append(list, item);
		Py_XDECREF(item);
	}
	std::shared_ptr<CPythonObject> po(new CPythonObject(list));
	return po;
}


#endif
