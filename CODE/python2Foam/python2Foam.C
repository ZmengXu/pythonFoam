/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2024 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Application
    pythonFoam

Description
    see details in https://zhuanlan.zhihu.com/p/450318119
\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include <Python.h> 
#include <iostream>
//#include <vector>
//#include <numpy/arrayobject.h>
using namespace std;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{

    //python link
    Py_Initialize(); 

    double rbInitial = 0.0215e-6;
    double rbDotInitial = 0.1; //adjustment
    double CL_ = 1300.0;
    double Pv_ = 1160000;
    double pAmbient = 116000;
    double sigma = 0.25;
    double n_ = 1.0;
    double mu = 0.24e-3;
    double kappa_ = 8.0e-6;
    double time = 1.5e-7; //adjustment
    //double rb = 0.0;
    double rho = 595.59;

    //python link
    Py_Initialize();


    PyObject* pModule = NULL;
    PyObject* pFunc = NULL;
    PyObject* pName = NULL;

    Pout << "this is in update " << endl;
    PyRun_SimpleString("import sys");
    Pout << "this is in update sys" << endl;
    PyRun_SimpleString("sys.path.append('./')");
    Pout << "this is in update sys.path" << endl;

    Pout << "this is in update PyRun_SimpleString" << endl;
    //pModule = PyImport_ImportModule("pythonFileName");
    pModule = PyImport_ImportModule("KMEquation");
    Pout << "this is in update PyRun_ImportModule" << endl;
    if( pModule == NULL )
    {
        std::cout <<"module or script file not found" << endl;
    }

    //the ode solving function call from python
    pFunc = PyObject_GetAttrString(pModule, "solve");
    if(pFunc == NULL)
    {
        std::cout << "no function is called" << endl;
    }

    double initial_conditions[] = {rbInitial, rbDotInitial};
    
    PyObject* pyInitialConditions = Py_BuildValue("(dd)", initial_conditions[0], initial_conditions[1]);

    PyObject* pyList = PyList_New(2);
    PyList_SetItem(pyList, 0, PyFloat_FromDouble(0.0));
    PyList_SetItem(pyList, 1, PyFloat_FromDouble(time));
    PyObject* pyArgs = PyTuple_Pack(13, pyInitialConditions, PyFloat_FromDouble(rbInitial), PyFloat_FromDouble(CL_), PyFloat_FromDouble(Pv_), PyFloat_FromDouble(pAmbient), PyFloat_FromDouble(pAmbient), PyFloat_FromDouble(sigma), PyFloat_FromDouble(n_), PyFloat_FromDouble(mu), PyFloat_FromDouble(kappa_), PyFloat_FromDouble(rho), pyList, PyFloat_FromDouble(time));

    PyObject* pReturn = PyEval_CallObject(pFunc, pyArgs);

    if (pReturn == NULL)
    {
        if (PyErr_Occurred())
                PyErr_Print();
        std::cout << "no python back" << endl;
    }

    double rb;

    PyArg_Parse(pReturn, "d", &rb);

    std::cout << "rb" << rb << endl;

    Py_Finalize();

    return 0;
}

// ************************************************************************* //

