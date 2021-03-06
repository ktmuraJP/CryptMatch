#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<Python.h>
#define GETODD 1
#define GETEVN 2

typedef struct _string_data
{
    int lines;
    int strlen;
    int *columns;
    char *string;
    char **split;
}strdata_t;

char *filestring(const char *filename)
{
    FILE *fp;
    char *fstr;
    int fsize;

    fp = fopen(filename,"r");
    if(fp == NULL){
        fprintf(stderr,"file input error\n");
        exit(EXIT_FAILURE);
    }
    fseek(fp,0,SEEK_END);
    fsize = ftell(fp);
    fclose(fp);
    fp = fopen(filename,"r");
    fstr = (char *)malloc(sizeof(char)*fsize);
    for(int i = 0; i < fsize; i++){ 
        fstr[i] = fgetc(fp);
    }
    fclose(fp);

    return fstr;
}

void filewrite(const char *filename,const char *fstring)
{
    FILE *fp;
    
    fp = fopen(filename,"w");
    if(fp == NULL){
        fprintf(stderr,"file output error\n");
        exit(EXIT_FAILURE);
    }
    fputs(fstring,fp);
    fclose(fp);
}

void SetStringData(strdata_t *strdata)
{

    int index;
    int len,nl;
    int *column_data;
    char tgt;
    char **str;

    tgt = '\n';
    len = strlen(strdata->string);
    nl = 0;
    index = 0;

    for(int i = 0; i < len; i++){
        (strdata->string[i] == tgt) ? nl++  : 0; 
    }
    column_data = (int*)malloc(sizeof(int)*(nl));
    nl = 0;
    
    for(int i = 0; i < len; i++){

        if(strdata->string[i-1] == tgt){
            column_data[nl] = index;
            nl++;
            index = 0;
        }
        index++;
    }

    str = (char **)malloc(sizeof(char *)*(nl));
    index = 0; 

    for(int i = 0; i < nl; i++){
        str[i] = (char *)malloc(sizeof(char)*(column_data[i]));
        for(int j = 0; j < column_data[i]; j++){
            str[i][j] = strdata->string[index];
            index++;
        }
        if(column_data[i]!=1){
            str[i] = strtok(str[i],"\n");
        }
    }

    strdata->strlen = len;
    strdata->lines = nl;
    strdata->columns = column_data;
    strdata->split = str;
}

/*
   >>query encrypt function<<
*/

char *QueryEncrypt(char *input)
{
    int len;
    char *str;
    
    len = strlen(input);
    str = (char *)malloc(sizeof(char)*len);
    
    for(int i = 0; i < len; i++){
        /*
           decryption algorithm scope
        */
        str[i] = ~input[i];
    }
    return str;
}

/*
   >>query decrypt function<<
*/

char *QueryDecrypt(char *input)
{
    int len;
    char *str;
    
    len = strlen(input);
    str = (char *)malloc(sizeof(char)*len);

    for(int i = 0; i < len; i++){        
        /*
           decryption algorithm scope
        */
        str[i] = ~input[i];
    }
     return str;
}

PyObject *Qencrypt(PyObject *self, PyObject *args)
{
    char *input, *rets;
    if(!PyArg_ParseTuple(args,"s",&input))
        return NULL;
    rets = QueryEncrypt(input);
    return Py_BuildValue("s",rets);
} 

PyObject *Qdecrypt(PyObject *self, PyObject *args)
{
    char *input, *rets;
    if(!PyArg_ParseTuple(args,"s",&input))
        return NULL;
    rets = QueryDecrypt(input);
    return Py_BuildValue("s",rets);
}

static PyMethodDef QCryptMethods[] = {
    {"encode",Qencrypt,METH_VARARGS,"Encryption"},
    {"decode",Qdecrypt,METH_VARARGS,"Decryption"},
    {NULL}
};

static struct PyModuleDef QCryptModule = {
    PyModuleDef_HEAD_INIT,
    "QCrypt",
    NULL,
    -1,
    QCryptMethods
};

PyMODINIT_FUNC PyInit_QCrypt(void)
{
    return PyModule_Create(&QCryptModule);
}

int main(int argc,char *argv[])
{
    wchar_t *program = Py_DecodeLocale(argv[0],NULL);
    if(program==NULL){
        fprintf(stderr,"Fatal Error argv[0]\n");
        exit(1);
    }
    PyImport_AppendInittab("QCrypt",PyInit_QCrypt);
    Py_SetProgramName(program);
    Py_Initialize();
    PyImport_ImportModule("QCrypt");
    PyMem_Free(program);
    return 0;
}
