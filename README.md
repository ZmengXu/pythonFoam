# pythonFoam
In OpenFOAM, call python function

## How to use it
### compile the code
```shell
git clone git@github.com:ZmengXu/pythonFoam.git
cd CODE
source $HOME/OpenFOAM/OpenFOAM-8/etc/bashrc
wmake
```

### run the case
make your own `pythonFileName.py` file in the `CASE` folder
```shell
cd ../CASE
pythonFoam
```
