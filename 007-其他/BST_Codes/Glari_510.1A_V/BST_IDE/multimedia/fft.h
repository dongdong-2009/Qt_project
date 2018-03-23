#ifndef FFT_H
#define FFT_H

#include "global.h"
#include <iostream>
#include <fstream>
#include "math.h"
#include "string.h"
#include <QObject>

// �����ṹ��
typedef struct
{
    double Real;
    double Imag;
} Complex_Array;

class FFT : public QObject
{
    Q_OBJECT
public:
    FFT(QObject *parent = 0);
    ~FFT();
signals:
    void sFDComplexArray(Complex_Array *pFD, quint32 pNum);
    void sTDComplexArray(Complex_Array *pTD, quint32 pNum);
public slots:
    // һά FFT �任
    void slot_FFT1D(Complex_Array* pTD, Complex_Array* pFD, quint32 nGrade);
    // һά IFFT �任
    void slot_IFFT1D(Complex_Array* pFD, Complex_Array* pTD, quint32 nGrade);
};

#endif // FFT_H
