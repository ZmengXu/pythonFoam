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
#include<Python.h> 
#include <iostream>
using namespace std;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    Py_Initialize(); //1、初始化python接口
    
    //初始化使用的变量
    PyObject* pModule = NULL;
    PyObject* pFunc = NULL;
    PyObject* pName = NULL;
    
    //2、初始化python系统文件路径，保证可以访问到 .py文件
    // ./ 表示运行算例的根目录
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");
    
    //3、调用python文件名。当前的测试python文件名是 pythonFileName.py
    // 在使用这个函数的时候，只需要写文件的名称就可以了。不用写后缀。
    pModule = PyImport_ImportModule("pythonFileName");
	if( pModule == NULL )
    {
		cout <<"module or script file not found" << endl;
		return 1;
    }

    //4、调用函数
    {
        //4a. 无参函数
	    PyObject* pFunc = PyObject_GetAttrString(pModule, "say");
	    if( !pFunc || !PyCallable_Check(pFunc))
        {
	    	cout <<"not found function add_num" << endl;
		    return 0;
	    }
        PyObject_CallObject(pFunc, NULL);
        Info << "This is in OpenFOAM, calling say() function" << endl;
    }

    //4b. 含参函数
    {
        //函数在pythonFileName.py的def AdditionFc(a, b): 中
        pFunc = PyObject_GetAttrString(pModule, "AdditionFc");
        // 给python传参数
        // 函数调用的参数传递均是以元组的形式打包的,2表示参数个数
        // 如果AdditionFc中只有一个参数时，写1就可以了
        PyObject* pArgs = PyTuple_New(2);
        // 0：第一个参数，传入 int 类型的值 2
        PyTuple_SetItem(pArgs, 0, Py_BuildValue("i", 2)); 
        // 1：第二个参数，传入 int 类型的值 4
        PyTuple_SetItem(pArgs, 1, Py_BuildValue("i", 4)); 
        // 使用C++的python接口调用该函数
        PyObject* pReturn = PyEval_CallObject(pFunc, pArgs);
        // 接收python计算好的返回值
        int nResult;
        // i表示转换成int型变量。
        // 在这里，最需要注意的是：PyArg_Parse的最后一个参数，必须加上“&”符号
        PyArg_Parse(pReturn, "i", &nResult);
        Info << "This is in OpenFOAM, calling AdditionFc(a, b) and the return result is " << nResult << endl;
    }    
    //5、结束python接口初始化
    Py_Finalize();

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    return 0;
}


// ************************************************************************* //
